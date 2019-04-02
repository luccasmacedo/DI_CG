#include <stdio.h>
#include <GL/glut.h>
#include "math.h"
#include "glcPoint.h"

//------------------------------------------------------------------------------------
glcPoint::glcPoint()
{
    this->Set(0.0, 0.0, 0.0);
}

//------------------------------------------------------------------------------------
glcPoint::glcPoint(float a, float b, float c)
{
    this->Set(a,b,c);
}

//------------------------------------------------------------------------------------
void glcPoint::Set(float a, float b, float c)
{
    this->x = a;
    this->y = b;
    this->z = c;
}

//------------------------------------------------------------------------------------
void glcPoint::Move(float a, float b, float c)
{
    this->x+=a;
    this->y+=b;
    this->z+=c;
}

//------------------------------------------------------------------------------------
void glcPoint::Print()
{
    printf("{ %.4f,  %.4f, %.4f }\n", this->x, this->y, this->z);
}

//------------------------------------------------------------------------------------
void glcPoint::RenderAsSphere(float radius, int resolution)
{
    glPushMatrix();
    glTranslatef(this->x, this->y, this->z);
    glutSolidSphere(radius, resolution, resolution);
    glPopMatrix();
}

//------------------------------------------------------------------------------------
void glcPoint::RenderAsSphere(float radius, int resolution, float *color)
{
    glColor3fv(color);
    this->RenderAsSphere(radius, resolution);
}

//------------------------------------------------------------------------------------
// Calcula um vetor a partir de dois pontos
void glcPoint::ComputeVector(glcPoint p2, glcPoint p1)
{
    this->x = p2.x - p1.x;
    this->y = p2.y - p1.y;
    this->z = p2.z - p1.z;
}

//------------------------------------------------------------------------------------
// Calcula o produto Vetorial
void glcPoint::ComputeCrossProduct(glcPoint first, glcPoint second)
{
    float X, Y,Z;
    X  = first.y * second.z;
    X -= first.z * second.y;
    this->x = X;
    Y  = first.z * second.x;
    Y -= first.x * second.z;
    this->y = Y;
    Z  = first.x * second.y;
    Z -= first.y * second.x;
    this->z = Z;
}

//------------------------------------------------------------------------------------
// Calcula comprimento do vetor
float glcPoint::ComputeVectorLength()
{
    float squarelen;
    float length;
    // compute vector length
    squarelen = this->x * this->x;
    squarelen += this->y * this->y;
    squarelen += this->z * this->z;
    length = (float)sqrt((float)squarelen);
    return length;
}

//------------------------------------------------------------------------------------
// Calcula o produto Escalar
float glcPoint::ComputeDotProduct(glcPoint second)
{
    // compute dot product
    float dotproduct  = this->x * second.x;
    dotproduct += this->y * second.y;
    dotproduct += this->z * second.z;
    return dotproduct;
}

//------------------------------------------------------------------------------------
// Compute Angle between two vectors (3D)
float glcPoint::ComputeAngle(glcPoint *second)
{
    glcPoint crossproduct;
    glcPoint *first = this;
    float lenproduct;
    float dotproduct;
    float radians, degrees;
    float lenfirst     = first->ComputeVectorLength();
    float lensecond    = second->ComputeVectorLength();

    if((lenfirst < 0.0001) || (lensecond < 0.0001))
    {
        degrees = 180;
    }
    else
    {
        // Calcula produto vetorial
        crossproduct.ComputeCrossProduct(*first, *second);
        // Calcula produto escalar
        dotproduct = first->ComputeDotProduct(*second);
        lenproduct   = crossproduct.ComputeVectorLength();
        // return the arctangent in the range -pi to +pi
        radians   = (float)atan2((float)lenproduct, (float)dotproduct);
        // Converte para graus
        degrees = radians * 180/M_PI;
    }
    return degrees;
}
