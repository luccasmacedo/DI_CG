/****************************************************
 ***    Project:  GL Raytracer
 ***    File:     rayTracerSceneReader.cpp
 ***    Author:   Daniele Donghi
 ***           d.donghi AT gmail.com
 ***    Created on November 1, 2011
 ****************************************************/

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include "rayTracingSceneReader.h"

using namespace std;


double* dbReader::getBackgroundColor()
{
    return background;
}

double* dbReader::getAmbientLight()
{
    return ambientLight;
}

int* dbReader::getWindowDimensions()
{
    return windowDimensions;
}

light* dbReader::getLight()
{
    return lightSource;
}

vector<triangle*> dbReader::getTriangleList()
{
    return triangleList;
}

vector<sphere*> dbReader::getSphereList()
{
    return sphereList;
}

void dbReader::read(char* fN)
{
    ifstream fileIn;
    fileIn.open(fN);

    if(!fileIn.is_open())
    {
        printf("Error %s does not exist.\n", fN);
        exit(1);
    }

    string ln;

    // read database file line-by-line
    while(getline(fileIn, ln))
    {

        if (ln.length() < 1) // Avoid empty lines
            continue;

        string firstChar = ln.substr(0,1);
        if (!firstChar.compare("#")) // Avoid comment lines
            continue;

        string section = ln.substr(0,ln.find(":")); // Get first string, before first ':'

        /** CG SETTINGS **/
        if ( section.compare("CG_SETTINGS") == 0 )
        {
            getline(fileIn, ln);

            string newln = ln.substr(0,3);

            while (newln.compare("END"))
            {
                string args = ln;
                string field = args.substr(0, args.find(":"));
                string values = args.substr(args.find(":")+2,args.find("\t")-(args.find(":")+2));

                if (field.find("WindowDimensions")!= field.npos)
                {
                    string w = values.substr(0,values.find(","));
                    values = values.substr(values.find(",")+1);
                    string h = values.substr(0,values.find(","));

                    //cout << r << ","<< g  << ","<< b << endl;
                    windowDimensions[0] = atof(w.c_str());
                    windowDimensions[1] = atof(h.c_str());
                }
                if (field.find("PRP")!= field.npos)
                {
                    string px = values.substr(0,values.find(","));
                    values = values.substr(values.find(",")+1);
                    string py = values.substr(0,values.find(","));
                    values = values.substr(values.find(",")+1);
                    string pz = values.substr(0,values.find(","));

                    PRP = new point3D(atof(px.c_str()),
                                      atof(py.c_str()),
                                      atof(pz.c_str()));
                }
                if (field.find("VRP")!= field.npos)
                {
                    string vx = values.substr(0,values.find(","));
                    values = values.substr(values.find(",")+1);
                    string vy = values.substr(0,values.find(","));
                    values = values.substr(values.find(",")+1);
                    string vz = values.substr(0,values.find(","));

                    VRP = new point3D(atof(vx.c_str()),
                                      atof(vy.c_str()),
                                      atof(vz.c_str()));
                }
                if (field.find("UV")!= field.npos)
                {
                    string u1 = values.substr(0,values.find(","));
                    values = values.substr(values.find(",")+1);
                    string u2 = values.substr(0,values.find(","));
                    values = values.substr(values.find(",")+1);
                    string v1 = values.substr(0,values.find(","));
                    values = values.substr(values.find(",")+1);
                    string v2 = values.substr(0,values.find(","));

                    uMin = atof(u1.c_str());
                    uMax = atof(u2.c_str());
                    vMin = atof(v1.c_str());
                    vMax = atof(v2.c_str());
                }
                getline(fileIn, ln);
                newln = ln.substr(0,3);
            }
        }

        /** WORLD SETTINGS **/
        if ( section.compare("WORLD_SETTINGS") == 0 )
        {
            getline(fileIn, ln);

            string newln = ln.substr(0,3);

            while (newln.compare("END"))
            {
                string args = ln;
                string field = args.substr(0, args.find(":"));
                string values = args.substr(args.find(":")+2,args.find("\t")-(args.find(":")+2));

                if (field.find("Background")!= field.npos)
                {
                    string r = values.substr(0,values.find(","));
                    values = values.substr(values.find(",")+1);
                    string g = values.substr(0,values.find(","));
                    values = values.substr(values.find(",")+1);
                    string b = values.substr(0,values.find(","));

                    //cout << r << ","<< g  << ","<< b << endl;
                    background[0] = atof(r.c_str());
                    background[1] = atof(g.c_str());
                    background[2] = atof(b.c_str());
                }

                else if (field.find("Ia")!= field.npos)
                {
                    string r = values.substr(0,values.find(","));
                    values = values.substr(values.find(",")+1);
                    string g = values.substr(0,values.find(","));
                    values = values.substr(values.find(",")+1);
                    string b = values.substr(0,values.find(","));

                    //cout << r << ","<< g  << ","<< b << endl;
                    ambientLight[0] = atof(r.c_str());
                    ambientLight[1] = atof(g.c_str());
                    ambientLight[2] = atof(b.c_str());
                }
                getline(fileIn, ln);
                newln = ln.substr(0,3);
            }
        }

        /** LIGHT SOURCE **/
        else if ( section.compare("LIGHT_SOURCE") == 0 )
        {

            getline(fileIn, ln);

            string newln = ln.substr(0,3);

            while (newln.compare("END"))
            {
                string args = ln;
                string field = args.substr(0, args.find(":"));
                string values = args.substr(args.find(":")+2,args.find("\t")-(args.find(":")+2));

                if (field.find("position")!= field.npos)
                {
                    string x = values.substr(0,values.find(","));
                    values = values.substr(values.find(",")+1);
                    string y = values.substr(0,values.find(","));
                    values = values.substr(values.find(",")+1);
                    string z = values.substr(0,values.find(","));
                    lightSource = new light(new point3D(atof(x.c_str()),atof(y.c_str()),atof(z.c_str())));
                    //cout << lightSource->location()->x() << "," <<lightSource->location()->y() << "," <<lightSource->location()->z() << endl;
                }

                else if (field.find("Is")!= field.npos)
                {
                    string r = values.substr(0,values.find(","));
                    values = values.substr(values.find(",")+1);
                    string g = values.substr(0,values.find(","));
                    values = values.substr(values.find(",")+1);
                    string b = values.substr(0,values.find(","));
                    lightSource->setColor(atof(r.c_str()), atof(g.c_str()),atof(b.c_str()));
                    //cout << lightSource->red() << ","<< lightSource->green()   << ","<< lightSource->blue() << endl;
                }

                getline(fileIn, ln);
                newln = ln.substr(0,3);
            }
        }

        /** TRIANGLE **/
        else if ( section.compare("TRIANGLE_OBJECT") == 0 )
        {

            getline(fileIn, ln);

            string newln = ln.substr(0,3);

            triangle* t = new triangle();

            while (newln.compare("END"))
            {
                string args = ln;
                string field = args.substr(0, args.find(":"));
                string values = args.substr(args.find(":")+2,args.find("\t")-(args.find(":")+2));

//                    cout << field << endl;
                if (field.find("point1")!= field.npos)
                {
                    string x = values.substr(0,values.find(","));
                    values = values.substr(values.find(",")+1);
                    string y = values.substr(0,values.find(","));
                    values = values.substr(values.find(",")+1);
                    string z = values.substr(0,values.find(","));

                    t->setPoint1(new point3D(atof(x.c_str()),atof(y.c_str()),atof(z.c_str())));
                    // cout << t->point1()->x()<< ","<< t->point1()->y()<< ","<< t->point1()->z() <<endl;
                }

                else if (field.find("point2")!= field.npos)
                {
                    string x = values.substr(0,values.find(","));
                    values = values.substr(values.find(",")+1);
                    string y = values.substr(0,values.find(","));
                    values = values.substr(values.find(",")+1);
                    string z = values.substr(0,values.find(","));

                    t->setPoint2(new point3D(atof(x.c_str()),
                                             atof(y.c_str()),
                                             atof(z.c_str())));
                }

                else if (field.find("point3")!= field.npos)
                {
                    string x = values.substr(0,values.find(","));
                    values = values.substr(values.find(",")+1);
                    string y = values.substr(0,values.find(","));
                    values = values.substr(values.find(",")+1);
                    string z = values.substr(0,values.find(","));

                    t->setPoint3(new point3D(atof(x.c_str()),atof(y.c_str()),atof(z.c_str())));
                }

                else if (field.find("Ka")!= field.npos)
                {
                    string r = values.substr(0,values.find(","));
                    values = values.substr(values.find(",")+1);
                    string g = values.substr(0,values.find(","));
                    values = values.substr(values.find(",")+1);
                    string b = values.substr(0,values.find(","));
                    t->setAmbientProperty(atof(r.c_str()), atof(g.c_str()),atof(b.c_str()));
                }

                else if (field.find("Kd")!= field.npos)
                {
                    string r = values.substr(0,values.find(","));
                    values = values.substr(values.find(",")+1);
                    string g = values.substr(0,values.find(","));
                    values = values.substr(values.find(",")+1);
                    string b = values.substr(0,values.find(","));
                    t->setDiffuseProperty(atof(r.c_str()), atof(g.c_str()),atof(b.c_str()));
//                        cout << t->getDiffuseProperty(0) << "," << t->getDiffuseProperty(1) << "," << t->getDiffuseProperty(2) << endl;

                }

                else if (field.find("Ks")!= field.npos)
                {
                    string r = values.substr(0,values.find(","));
                    values = values.substr(values.find(",")+1);
                    string g = values.substr(0,values.find(","));
                    values = values.substr(values.find(",")+1);
                    string b = values.substr(0,values.find(","));
                    t->setSpecularProperty(atof(r.c_str()), atof(g.c_str()),atof(b.c_str()));
//                        cout << t->getSpecularProperty(0) << "," << t->getSpecularProperty(1) << "," << t->getSpecularProperty(2) << endl;
                }

                else if (field.find("n")!= field.npos && field.length() < 4)
                {
                    t->setSpecularExponent(atof(values.c_str()));
//                       cout << t->getSpecularExponent()<< endl;
                }

                else if (field.find("reflection")!= field.npos )
                {
                    t->setReflection(atof(values.c_str()));
//                       cout << t->getReflection()<< endl;
                }

                else if (field.find("transmission")!= field.npos )
                {
                    t->setTransmission(atof(values.c_str()));
//                       cout << t->getTransmission()<< endl;
                }

                else if (field.find("refraction_index")!= field.npos )
                {
                    t->setIndexOfRefraction(atof(values.c_str()));
//                       cout << t->getIndexOfRefraction()<< endl;
                }

                getline(fileIn, ln);
                newln = ln.substr(0,3);
            }
            triangleList.push_back(t);
        }

        /** SPHERE **/
        else if ( section.compare("SPHERE_OBJECT") == 0 )
        {

            getline(fileIn, ln);

            string newln = ln.substr(0,3);

            sphere* s = new sphere();

            while (newln.compare("END"))
            {
                string args = ln;
                string field = args.substr(0, args.find(":"));
                string values = args.substr(args.find(":")+2,args.find("\t")-(args.find(":")+2));

                if (field.find("center")!= field.npos)
                {
                    string x = values.substr(0,values.find(","));
                    values = values.substr(values.find(",")+1);
                    string y = values.substr(0,values.find(","));
                    values = values.substr(values.find(",")+1);
                    string z = values.substr(0,values.find(","));

                    s->setCenter(new point3D(atof(x.c_str()),atof(y.c_str()),atof(z.c_str())));
                }

                else if (field.find("radius")!= field.npos)
                {
                    s->setRadius(atof(values.c_str()));
//                       cout << s->radius()<< endl;
                }

                else if (field.find("Ka")!= field.npos)
                {
                    string r = values.substr(0,values.find(","));
                    values = values.substr(values.find(",")+1);
                    string g = values.substr(0,values.find(","));
                    values = values.substr(values.find(",")+1);
                    string b = values.substr(0,values.find(","));
                    s->setAmbientProperty(atof(r.c_str()), atof(g.c_str()),atof(b.c_str()));
                }

                else if (field.find("Kd")!= field.npos)
                {
                    string r = values.substr(0,values.find(","));
                    values = values.substr(values.find(",")+1);
                    string g = values.substr(0,values.find(","));
                    values = values.substr(values.find(",")+1);
                    string b = values.substr(0,values.find(","));
                    s->setDiffuseProperty(atof(r.c_str()), atof(g.c_str()),atof(b.c_str()));
//                        cout << s->getDiffuseProperty(0) << "," << s->getDiffuseProperty(1) << "," << s->getDiffuseProperty(2) << endl;
                }

                else if (field.find("Ks")!= field.npos)
                {
                    string r = values.substr(0,values.find(","));
                    values = values.substr(values.find(",")+1);
                    string g = values.substr(0,values.find(","));
                    values = values.substr(values.find(",")+1);
                    string b = values.substr(0,values.find(","));
                    s->setSpecularProperty(atof(r.c_str()), atof(g.c_str()),atof(b.c_str()));
//                        cout << t->getSpecularProperty(0) << "," << t->getSpecularProperty(1) << "," << t->getSpecularProperty(2) << endl;
                }

                else if (field.find("n")!= field.npos && field.length() < 4)
                {
                    s->setSpecularExponent(atof(values.c_str()));
//                       cout << s->getSpecularExponent()<< endl;
                }

                else if (field.find("reflection")!= field.npos )
                {
                    s->setReflection(atof(values.c_str()));
//                       cout << s->getReflection()<< endl;
                }

                else if (field.find("transmission")!= field.npos )
                {
                    s->setTransmission(atof(values.c_str()));
//                       cout << s->getTransmission()<< endl;
                }

                else if (field.find("refraction_index")!= field.npos )
                {
                    s->setIndexOfRefraction(atof(values.c_str()));
//                       cout << s->getIndexOfRefraction()<< endl;
                }
                getline(fileIn, ln);
                newln = ln.substr(0,3);
            }
            sphereList.push_back(s);
        }
    } // End while of parse

    /*
    //
    //
    // Sample code to add a new set of spheres in the scene directly
    //
    //
    * /
    float radius = 0.05;
    float ka[3] = {0.05,0.05,0.05};
    float kd[3] = {0.15,0.15,0.15};
    float ks[3] = {0.5,0.5,0.5};
    float n = 29.4; //			# specular exponent
    float reflection = 0.01;//		# reflection coefficient
    float transmission = 0.99;//		# transmission coefficient
    float refraction_index = 1.05;//	# index of refraction

    for(int i = 0; i <= 10; i++)
    {
        sphere* s = new sphere();
        point3D *center = new point3D(-0.5+(float)i/10.0, -0.2+(float)i/20.0, -0.2);
        s->setCenter(center);
        s->setRadius(radius);
        s->setAmbientProperty(ka[0], ka[1], ka[2]);
        s->setDiffuseProperty(kd[0], kd[1], kd[2]);
        s->setSpecularProperty(ks[0], ks[1], ks[2]);
        s->setSpecularExponent(n);
        s->setReflection(reflection);
        s->setTransmission(transmission);
        s->setIndexOfRefraction(refraction_index);
        sphereList.push_back(s);
    }
    */
}
