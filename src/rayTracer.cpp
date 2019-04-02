/****************************************
 ***    Project:  GL Raytracer
 ***    File:     rayTracer.cpp
 ***    Author:   Daniele Donghi
 ***           d.donghi AT gmail.com
 ***    Created on November 1, 2011
 ****************************************/

#include <stdio.h>
#include <cmath>
#include "rayTracer.h"

using namespace std;

// constructor
glraytracer::glraytracer()
{
   reader = NULL;

   //PRP = new point3D(0.0, 0.0,-0.5);    // Projection Reference Point (centro de projeção)
   //VRP = new point3D(0.0, 0.0, 0.0);    // View Reference Point       (posição do plano de projeção)

   uMin = -1.0;
   uMax = 1.0;
   vMin = -0.5;
   vMax =  0.5;
}

// constructor
glraytracer::glraytracer(int w, int h)
{
//   width = w;
//   height = h;
//   reader = NULL;
//
//   PRP = new point3D(0.0, 0.0,-0.5);    // Projection Reference Point (centro de projeção)
//   VRP = new point3D(0.0, 0.0, 0.0);    // View Reference Point       (posição do plano de projeção)
//
//   uMin = -1.0;
//   uMax = 1.0;
//   vMin = -0.5;
//   vMax =  0.5;
}

// read database text file
void glraytracer::readDatabase(char* fileName)
{
    // free memory if previously allocated
    if(reader != NULL) delete reader;

    // create new reader
    reader = new dbReader();
    reader->read(fileName);

    int *window = reader->getWindowDimensions();
    width = window[0];
    height = window[1];

    PRP = new point3D(reader->PRP);
    VRP = new point3D(reader->VRP);

    uMin = reader->uMin;
    uMax = reader->uMax;
    vMin = reader->vMin;
    vMax = reader->vMax;
}

// Main function
void glraytracer::draw()
{
    glBegin(GL_POINTS);

    for (int i=0; i < this->height; i++)
        for (int j=0; j < this->width; j++)
        {
            glColor3f(buffer[j][i].r,buffer[j][i].g,buffer[j][i].b);
            glVertex2f(j,i);
        }
    glEnd();
}

void glraytracer::setWindow(int window)
{
	this->window = window;
}

void glraytracer::changeWindowTitle(int line)
{
	char windowTitle[64];
	float val = line * 100/(float)height;
	if(val>100) val=100;
	sprintf(windowTitle, "Processing Scene %.1f%%", val);

	glutSetWindow(this->window); // Set main window as current
	glutSetWindowTitle(windowTitle); // change title
}

void glraytracer::raytrace()
{
    double incrY = (vMax-vMin)/height;
    double incrX = (uMax-uMin)/width;
    int X = 0;
    int Y = 0;
    for (double y=vMin; y<vMax; y+=incrY)
    {
		X=0;
		for (double x=uMin; x<uMax; x+=incrX)
		{
			if (X >= this->width || Y >= this->height ) continue;

			point3D start = point3D(PRP->x(),PRP->y(),PRP->z());
			point3D direction = point3D(x - PRP->x(), y - PRP->y(), VRP->z() - PRP->z());
			direction = normalize(direction);
			ray viewRay = {start, direction, false, 1}; // current index of refraction = 1 : air
			buffer[X][Y] = RT_trace(viewRay, 0); // start ray tracing recursive algorithm
			X++;
		}
		Y++;
		this->changeWindowTitle(Y);
    }
}

color glraytracer::RT_trace(ray viewRay, int depth)
{

    // Looking for the closest intersection
    double t = 100000.0;
    int currentSphere= -1;
    int currentTriangle= -1;
    color c1 = {0.0, 0.0, 0.0};

    // Type of intersection
    int INTERSECTION_TYPE = 0;

    // Check spheres
    for (unsigned int i = 0; i < reader->getSphereList().size(); i++)
    {
        if (hitSphere(viewRay, reader->getSphereList().at(i), &t))
        {
            currentSphere = i;
            INTERSECTION_TYPE = SPHERE;
        }
    }

    // Check triangles
    for (unsigned int i = 0; i < reader->getTriangleList().size(); i++)
    {
        if (hitTriangle(viewRay, reader->getTriangleList().at(i), &t))
        {
            // Triangle hit
            currentTriangle = i;
            currentSphere = -1;
            INTERSECTION_TYPE = TRIANGLE;
        }
    }

    /**  NO HIT **/
    if (INTERSECTION_TYPE == 0)
    {
        //if (depth > 0) cout << "hererfe" << endl;
        color c = {reader->getBackgroundColor()[0],reader->getBackgroundColor()[1],reader->getBackgroundColor()[2]};
        return c;
    }

    /** HIT **/
    // Calculate intersection point, that will be new starting point of the ray
    point3D intersection = point3D(viewRay.start.x() + t * viewRay.dir.x(),
                                   viewRay.start.y() + t * viewRay.dir.y(),
                                   viewRay.start.z() + t * viewRay.dir.z());

    if ( INTERSECTION_TYPE == SPHERE)
    {
        return RT_shade(INTERSECTION_TYPE, currentSphere, viewRay, intersection, depth);
    }
    else if (INTERSECTION_TYPE == TRIANGLE)
    {
        return RT_shade(INTERSECTION_TYPE, currentTriangle, viewRay, intersection, depth);
    }
    return c1;
}

color glraytracer::RT_shade(int INTERSECTION_TYPE, int obj_index, ray viewRay, point3D intersection, int depth)
{
    color rColor = {0,0,0};
    color tColor = {0,0,0};
    color ray_color = {0,0,0};

    /** AMBIENT + DIFFUSE + SPECULAR **/
    if ( INTERSECTION_TYPE == SPHERE )
    {
        // Current sphere
        sphere* s = reader->getSphereList().at(obj_index);

        ray_color.r = reader->getAmbientLight()[0] * s->getAmbientProperty(0);
        ray_color.g = reader->getAmbientLight()[1] * s->getAmbientProperty(1);
        ray_color.b = reader->getAmbientLight()[2] * s->getAmbientProperty(2);

        setDiffuseAndSpecular_Sphere(viewRay, s, intersection, &ray_color);
    }
    else if ( INTERSECTION_TYPE == TRIANGLE)
    {
        triangle* tri = reader->getTriangleList().at(obj_index);

        ray_color.r = reader->getAmbientLight()[0] * tri->getAmbientProperty(0); //ambient Ia * Ka
        ray_color.g = reader->getAmbientLight()[1] * tri->getAmbientProperty(1);
        ray_color.b = reader->getAmbientLight()[2] * tri->getAmbientProperty(2);

        setDiffuseAndSpecular_Triangle(viewRay, tri, intersection, &ray_color);
    }
    /** END AMBIENT + DIFFUSE + SPECULAR **/

    if (depth < MAX_RAYTRACING_DEPTH)
    {
        if (INTERSECTION_TYPE == SPHERE)
        {
            sphere* s = reader->getSphereList().at(obj_index);
            if (s->getReflection() > 0)
            {
                rColor = setReflection_Sphere(viewRay, intersection, s, depth);
                ray_color.r += rColor.r*s->getReflection();
                ray_color.g += rColor.g*s->getReflection();
                ray_color.b += rColor.b*s->getReflection();
            }

            if (s->getTransmission() > 0)
            {
                tColor = setTransmission_Sphere(viewRay, intersection, s, depth);
                ray_color.r += tColor.r*s->getTransmission();
                ray_color.g += tColor.g*s->getTransmission();
                ray_color.b += tColor.b*s->getTransmission();
            }
        }
        else if (INTERSECTION_TYPE == TRIANGLE)
        {
            triangle* tri = reader->getTriangleList().at(obj_index);
            if (tri->getReflection() > 0)
            {
                rColor = setReflection_Triangle(viewRay, intersection, tri, depth);
                ray_color.r += rColor.r*tri->getReflection();
                ray_color.g += rColor.g*tri->getReflection();
                ray_color.b += rColor.b*tri->getReflection();
            }

            if (tri->getTransmission() > 0)
            {
                tColor = setTransmission_Triangle(viewRay, intersection, tri, depth);
            }
        }
    }
    return ray_color; // Return color of ray
}

color glraytracer::setTransmission_Triangle(ray &viewRay, point3D newStart, triangle* tri, int depth)
{
   color sample;
    // Not present in this project
   return sample;
}

color glraytracer::setTransmission_Sphere(ray &viewRay, point3D newStart, sphere* s, int depth)
{

    // NORMAL VECTOR //
    point3D norm = point3D((newStart.x() - s->center()->x())/s->radius(),
                           (newStart.y() - s->center()->y())/s->radius(),
                           (newStart.z() - s->center()->z())/s->radius());

    // N
    point3D N = norm;

    // V
    point3D V = normalize(point3D(-viewRay.dir.x(),-viewRay.dir.y(),-viewRay.dir.z()));

    double sphere_index_of_refraction = s->getIndexOfRefraction();
    double  idx_refraction1, idx_refraction2; // from, to

    // Thomas version of raytrace
//    if (!viewRay.inside){
//        idx_refraction1 = viewRay.index_of_refraction;
//        idx_refraction2 = sphere_index_of_refraction;
//    }
//    else{
//        idx_refraction1 = sphere_index_of_refraction;
//        idx_refraction2 = viewRay.index_of_refraction;
//        N = point3D(-N.x(),-N.y(),-N.z());
//    }

    // My version
    if (!viewRay.inside)
    {
        idx_refraction1 = 1;
        idx_refraction2 = sphere_index_of_refraction;
    }
    else
    {
        idx_refraction1 = sphere_index_of_refraction;
        idx_refraction2 = 1;
        N = point3D(-N.x(),-N.y(),-N.z());
    }

    double nf = idx_refraction1 / idx_refraction2;
    double th1 = acos(dot(N,V));
    double th2 = asin( nf * sin(th1) );

    point3D trans_vect = point3D(0,0,0);

    point3D temp1 = const_mult_point(nf,V); // nf*V
    point3D temp2 = const_mult_point((nf*cos(th1) - cos(th2)), N); // (nf*cos(th1) - cos(th2))*N
    point3D temp3 = const_mult_point((nf*cos(th1) + cos(th2)), N); // (nf*cos(th1) + cos(th2))*N

    if ( dot(N,V) > 0 )
        trans_vect = point3D ( - temp1.x() + temp2.x(),
                               - temp1.y() + temp2.y(),
                               - temp1.z() + temp2.z());
    else
        trans_vect = point3D ( - temp1.x() + temp3.x(),
                               - temp1.y() + temp3.y(),
                               - temp1.z() + temp3.z());

    ray trans_ray = {newStart, normalize(trans_vect), !viewRay.inside, idx_refraction2};

    return RT_trace(trans_ray,depth + 1);

}

color glraytracer::setReflection_Triangle(ray &viewVect, point3D newStart, triangle * tri, int depth)
{

    point3D vect1 = min(*tri->point2(), *tri->point1()); // AB
    point3D vect2 = min(*tri->point3(), *tri->point1()); // AC

    // N
    point3D N = normalize(crossProduct(vect2,vect1)); // Normal to plane

    // V
    point3D V = normalize(point3D(-viewVect.dir.x(),-viewVect.dir.y(),-viewVect.dir.z()));

    // specular reflection
    double NdotV = dot(N,V);
    point3D R = point3D( const_mult_point(2*NdotV,N).x() - V.x(),
                         const_mult_point(2*NdotV,N).y() - V.y(),
                         const_mult_point(2*NdotV,N).z() - V.z());

    ray refl_ray = {newStart,R, viewVect.inside, viewVect.index_of_refraction};

    return RT_trace(refl_ray,depth + 1);
}

color glraytracer::setReflection_Sphere(ray &viewVect, point3D newStart, sphere* s, int depth)
{

    // NORMAL VECTOR //
    point3D norm = point3D((newStart.x() - s->center()->x())/s->radius(),
                           (newStart.y() - s->center()->y())/s->radius(),
                           (newStart.z() - s->center()->z())/s->radius());

    // N
    point3D N = norm;

    // V
    point3D V = normalize(point3D(-viewVect.dir.x(),-viewVect.dir.y(),-viewVect.dir.z()));

    // specular reflection
    double NdotV = dot(N,V);
    point3D R = point3D( const_mult_point(2*NdotV,N).x() - V.x(),
                         const_mult_point(2*NdotV,N).y() - V.y(),
                         const_mult_point(2*NdotV,N).z() - V.z());

    ray refl_ray = {newStart,R, viewVect.inside, viewVect.index_of_refraction};

    return RT_trace(refl_ray,depth + 1);

}

void glraytracer::setDiffuseAndSpecular_Triangle(ray &viewRay, triangle *tri, point3D newStart, color *c)
{

    point3D vect1 = min(*tri->point2(), *tri->point1()); // AB
    point3D vect2 = min(*tri->point3(), *tri->point1()); // AC

    // N
    point3D N = normalize(crossProduct(vect2,vect1)); // Normal to plane

    // LIGHT RAY //
    light* l = reader->getLight();

    // Light vector (not-normalized)
    point3D dist = point3D(-newStart.x() + l->location()->x(),
                           -newStart.y() + l->location()->y(),
                           -newStart.z() + l->location()->z());

    // L
    point3D L = normalize(dist); // light vector normalized

    // V
    point3D V = normalize(viewRay.dir);

    // diffuse factor
    double diffuse = dot(N,L);

    // specular reflection
    double NdotL = dot(N,L);
    point3D R = point3D(- const_mult_point(2*NdotL,N).x() + L.x(),
                        - const_mult_point(2*NdotL,N).y() + L.y(),
                        - const_mult_point(2*NdotL,N).z() + L.z());
    double specular = dot(R,V);
    if (specular < 0 ) specular = 0;

    c->r +=  diffuse * l->red() * tri->getDiffuseProperty(0) + // diffuse
             pow(specular,tri->getSpecularExponent()) * l->red() * tri->getSpecularProperty(0); // specular

    c->g += diffuse * l->green() * tri->getDiffuseProperty(1) +
            pow(specular,tri->getSpecularExponent()) * l->green() * tri->getSpecularProperty(1);

    c->b += diffuse * l->blue() * tri->getDiffuseProperty(2) +
            pow(specular,tri->getSpecularExponent()) * l->blue() * tri->getSpecularProperty(2);

}

void glraytracer::setDiffuseAndSpecular_Sphere(ray &viewRay, sphere *s, point3D newStart, color *c)
{

    // NORMAL VECTOR //
    point3D norm = point3D((newStart.x() - s->center()->x())/s->radius(),
                           (newStart.y() - s->center()->y())/s->radius(),
                           (newStart.z() - s->center()->z())/s->radius());

    // N
    point3D N = norm;

    // LIGHT RAY //
    light* l = reader->getLight();

    // Light vector (not-normalized)
    point3D dist = point3D(-newStart.x() + l->location()->x(),
                           -newStart.y() + l->location()->y(),
                           -newStart.z() + l->location()->z());

    // L
    point3D L = normalize(dist); // normalized

    // V
    point3D V = normalize(viewRay.dir);

    // diffuse factor
    double diffuse = dot(N,L);

    // specular reflection
    double NdotL = dot(N,L);
    point3D R = point3D(- const_mult_point(2*NdotL,N).x() + L.x(),
                        - const_mult_point(2*NdotL,N).y() + L.y(),
                        - const_mult_point(2*NdotL,N).z() + L.z());
    double specular = dot(R,V);
    if (specular < 0 ) specular = 0;

    c->r +=  diffuse * l->red() * s->getDiffuseProperty(0) + // diffuse
             pow(specular,s->getSpecularExponent()) * l->red() * s->getSpecularProperty(0); // specular

    c->g += diffuse * l->green() * s->getDiffuseProperty(1) +
            pow(specular,s->getSpecularExponent()) * l->green() * s->getSpecularProperty(1);

    c->b += diffuse * l->blue() * s->getDiffuseProperty(2) +
            pow(specular,s->getSpecularExponent()) * l->blue() * s->getSpecularProperty(2);
}

bool glraytracer::hitTriangle(ray &r, triangle *tri, double *t)
{

    point3D vect1 = min(*tri->point2(), *tri->point1()); // AB
    point3D vect2 = min(*tri->point3(), *tri->point1()); // AC

    point3D N = crossProduct(vect1,vect2); // Normal to plane

    double A = N.x();
    double B = N.y();
    double C = N.z();

    double D = - A * tri->point1()->x() - B * tri->point1()->y() - C * tri->point1()->z();

    double t_tri = - ( A * r.start.x() + B * r.start.y() + C * r.start.z() + D) /
                   ( A * r.dir.x() + B * r.dir.y() + C * r.dir.z());

    bool retval = false;
    if (t_tri > 0.01 && t_tri < *t)
    {

        // Calculate intersection point, to check if point is in triangle
        point3D intersection = point3D(r.start.x() + t_tri * r.dir.x(),
                                       r.start.y() + t_tri * r.dir.y(),
                                       r.start.z() + t_tri * r.dir.z());

        if ( isInTriangle(intersection,*tri->point1(),*tri->point2(),*tri->point3()) )
        {
            *t = t_tri;
            retval = true;
        }
    }
    return retval;

}


bool glraytracer::hitSphere(ray &r, sphere *s, double *t)
{
    double A = r.dir.x()*r.dir.x() +
               r.dir.y()*r.dir.y() +
               r.dir.z()*r.dir.z();
    double B = r.dir.x()*(r.start.x() - s->center()->x()) +
               r.dir.y()*(r.start.y() - s->center()->y()) +
               r.dir.z()*(r.start.z() - s->center()->z());
    double C = pow((r.start.x() - s->center()->x()),2) +
               pow((r.start.y() - s->center()->y()),2) +
               pow(r.start.z() - s->center()->z(),2) -
               pow(s->radius(),2);

    double y0 = (-B - sqrt(pow(B,2)-(A*C)))/A;
    double y1 = (-B + sqrt(pow(B,2)-(A*C)))/A;

    bool retvalue = false;
    if ((y0 > 0.01) && (*t > y0))
    {
        // cout << "true" << endl;
        *t = y0;
        retvalue = true;
    }
    if ((y1 > 0.01) && (*t > y1))
    {
        // cout << "true" << endl;
        *t = y1;
        retvalue = true;
    }
    return retvalue;
}

point3D glraytracer::const_mult_point(double c, point3D p)
{
    return point3D(p.x()*c,p.y()*c,p.z()*c);
}

double glraytracer::dot (point3D p1, point3D p2 )
{
    return p1.x()*p2.x() + p1.y()*p2.y() + p1.z()*p2.z();
}

point3D glraytracer::normalize(point3D p)
{
    double x = p.x() / sqrt(mod(p));
    double y = p.y() / sqrt(mod(p));
    double z = p.z() / sqrt(mod(p));
    return point3D(x,y,z);
}

// Square mod
double glraytracer::mod(point3D p)
{
    return (p.x()*p.x() + p.y()*p.y() + p.z()*p.z());
}

// Cross product
point3D glraytracer::crossProduct(point3D v1, point3D v2)
{
    return point3D ( v1.y()*v2.z() - v1.z()*v2.y(),
                    -v1.x()*v2.z() + v1.z()*v2.x(),
                     v1.x()*v2.y() - v1.y()*v2.x()
                   );
}

// Create a vector
point3D glraytracer::min(point3D p2, point3D p1)
{
    return point3D (p2.x() - p1.x(),
                    p2.y() - p1.y(),
                    p2.z() - p1.z());
}

bool glraytracer::sameSide(point3D p1, point3D p2, point3D A, point3D B)
{

    point3D cross_prod1 = crossProduct(min(B,A),min(p1,A));
    point3D cross_prod2 = crossProduct(min(B,A),min(p2,A));

    if (dot(cross_prod1,cross_prod2) >= 0) return true;
    else return false;
}

bool glraytracer::isInTriangle(point3D p, point3D A, point3D B, point3D C)
{
    if ( sameSide(p,A,B,C) && sameSide(p,B,A,C) && sameSide(p,C,A,B) ) return true;
    else return false;
}

