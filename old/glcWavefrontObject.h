/*
  Name:         glcShader.h
  Author:       Rodrigo Luis de Souza da Silva
  Date:         06/12/2017
  Description:  GL class to help dealing with wavefront objects. It's a simplified version
                of the well-known glm file reader developed by Nate Robins.
*/

#ifndef _GLC_WAVEFRONT_OBJECT_H
#define _GLC_WAVEFRONT_OBJECT_H

#include <math.h>
#include <stdio.h>
#include <GL/glut.h>

using namespace std;

#define ONLY_VERTICES   (0)            /* render with only vertices */
#define FLAT_SHADING    (1 << 0)       /* render with facet normals */
#define SMOOTH_SHADING  (1 << 1)       /* render with vertex normals */
#define USE_TEXTURE     (1 << 2)       /* render with texture coords */
#define USE_COLOR       (1 << 3)       /* render with colors */
#define USE_MATERIAL    (1 << 4)       /* render with materials */

#define T(x) (model->triangles[(x)])

// GLMmaterial: Structure that defines a material in a model.
typedef struct _GLMmaterial
{
    char* name;                   /* name of material */
    GLfloat diffuse[4];           /* diffuse component */
    GLfloat ambient[4];           /* ambient component */
    GLfloat specular[4];          /* specular component */
    GLfloat emmissive[4];         /* emmissive component */
    GLfloat shininess;            /* specular exponent */
} GLMmaterial;

// GLMtriangle: Structure that defines a triangle in a model.
typedef struct _GLMtriangle
{
    GLuint vindices[3];           /* array of triangle vertex indices */
    GLuint nindices[3];           /* array of triangle normal indices */
    GLuint tindices[3];           /* array of triangle texcoord indices*/
    GLuint findex;                /* index of triangle facet normal */
} GLMtriangle;

// GLMgroup: Structure that defines a group in a model.
typedef struct _GLMgroup
{
    char*             name;           /* name of this group */
    GLuint            numtriangles;   /* number of triangles in this group */
    GLuint*           triangles;      /* array of triangle indices */
    GLuint            material;       /* index to material for group */
    struct _GLMgroup* next;           /* pointer to next group in model */
} GLMgroup;

// GLMmodel: Structure that defines a model.
typedef struct _GLMmodel
{
    char*    pathname;            /* path to this model */
    char*    mtllibname;          /* name of the material library */

    GLuint   numvertices;         /* number of vertices in model */
    GLfloat* vertices;            /* array of vertices  */

    GLuint   numnormals;          /* number of normals in model */
    GLfloat* normals;             /* array of normals */

    GLuint   numtexcoords;        /* number of texcoords in model */
    GLfloat* texcoords;           /* array of texture coordinates */

    GLuint   numfacetnorms;       /* number of facetnorms in model */
    GLfloat* facetnorms;          /* array of facetnorms */

    GLuint       numtriangles;    /* number of triangles in model */
    GLMtriangle* triangles;       /* array of triangles */

    GLuint       nummaterials;    /* number of materials in model */
    GLMmaterial* materials;       /* array of materials */

    GLuint       numgroups;       /* number of groups in model */
    GLMgroup*    groups;          /* linked list of groups */

    GLfloat position[3];          /* position of the model */

} GLMmodel;

class glcWavefrontObject
{
    public:
        glcWavefrontObject();
        ~glcWavefrontObject();
        void  SetNumberOfObjects(int );
        int   GetNumberOfObjects();
        void  SelectObject(int);

        void ReadObject(char *filename);
        float Unitize();
        void FacetNormal();
        void VertexNormals(GLfloat angle);
        void Scale(GLfloat scale);

        void Draw(GLuint mode);

    private:
        GLMgroup* FindGroup(char* name);
        GLMgroup* AddGroup(char* name);
        GLuint FindMaterial(char* name);

        float Dot(GLfloat* u, GLfloat* v);
        float Max(GLfloat a, GLfloat b);
        float Abs(GLfloat f);
        void  Cross(GLfloat* u, GLfloat* v, GLfloat* n);

        void FirstPass(FILE *file);
        void SecondPass(FILE *file);
        char* DirName(char* path);
        void  ReadMTL(char* name);
        void  Normalize(GLfloat* v);

        // Attributes
        GLMmodel* modelList;
        GLMmodel* model; // Current Object
        int numberOfObjects;
        //int selectedObject;
};

#endif


