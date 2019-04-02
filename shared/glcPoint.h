/*
  Name:        glcPoint.h
  Copyright:   Version 0.2
  Author:      Rodrigo Luis de Souza da Silva
  Date:        15/01/2007
  Description: Geometry point class with some methods added
  Updates:
      * Mudança de nome de point.h para glcPoint.h e adição de opções de rendering
*/

#ifndef _GLC_POINT_H
#define _GLC_POINT_H

class glcPoint
{
    public:
        glcPoint();
        glcPoint(float a, float b, float c);

        // Set x, y, z with a, b, c respectively
        void Set(float a, float b, float c);

        // Get x, y and z
        void Get(float &x, float &y, float &z);

        // Add (sum) a, b and c to the x, y and z, respectively
        void Move(float a, float b, float c);

        // x = p2.x - p1.x, y = p2.y - p1.y, z = p2.z - p1.z
        void ComputeVector(glcPoint p2, glcPoint p1);

        // Cross produt of first and second points
        void ComputeCrossProduct(glcPoint, glcPoint second);

        // Compute dot produ
        float ComputeDotProduct(glcPoint second);

        // Vector length defined by x, y, z and the origin
        float ComputeVectorLength();

        // Angle between x, y, z and p.x, p.y, p.z with the same origin
        float ComputeAngle(glcPoint *p );

        // Print x, y and z
        void  Print();

        // Rendering options
        // Render a sphere translated to the x, y and z position
        void  RenderAsSphere(float radius, int resolution);

        // Same as previous with color
        void  RenderAsSphere(float radius, int resolution, float *color);

    public:
        float x, y, z;
};

#endif
