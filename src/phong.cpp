/*
  Name:        Shading with Shaders
  Copyright:   Version 0.1
  Author:      Rodrigo Luis de Souza da Silva
  Last Update:	12/12/2013
  Date:        12/12/2013
  Description: Code to see the flat, gouraud and Phong (with shaders) shading models
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <GL/glew.h>
#include <GL/glut.h>
#include "glcShader.h"

#define USE_PHONG_SHADING   1
#define USE_GOURAUD_SHADING 2
#define USE_FLAT_SHADING    3

int selectMode = 1;
float zoom = 5.0;

// Shader object
glcShader *shader = NULL;

// Object Material
GLfloat object_ambient[]   = { 0.1, 0.0, 0.0, 1.0 };
GLfloat object_difusa[]    = { 1.0, 0.0, 0.0, 1.0 };
GLfloat object_especular[] = { 1.0, 0.9, 0.9, 1.0 };
GLfloat object_emissao[] =   { 0.0, 0.0, 0.0, 0.0 };
GLfloat object_brilho[]    = { 64.0 };

// Light Info
GLfloat cor_luz[] = { 1.0, 1.0, 1.0, 1.0};
GLfloat pos_luz[] = { 1.0, 1.0, 1.0, 1.0};

int width         = 880;
int height        = 800;
float rotationX 	= 0.0;
float rotationY 	= 0.0;
int   last_x, last_y;

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(0.0,  1.0,  zoom,
              0.0,  0.0,  0.0,
              0.0f, 1.0f, 0.0f);

    glPushMatrix();
        glTranslatef(pos_luz[0],pos_luz[1],pos_luz[2]);
        glLightfv(GL_LIGHT0, GL_POSITION, pos_luz);
        glMaterialfv(GL_FRONT, GL_EMISSION, cor_luz);
        glutSolidSphere(0.01,20,20);
    glPopMatrix();

    glMaterialfv(GL_FRONT, GL_AMBIENT, object_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, object_difusa);
    glMaterialfv(GL_FRONT, GL_SPECULAR, object_especular);
    glMaterialfv(GL_FRONT, GL_EMISSION, object_emissao);
    glMaterialfv(GL_FRONT, GL_SHININESS, object_brilho);

    switch(selectMode)
    {
        case USE_PHONG_SHADING:
            shader->Use();
            break;
        case USE_GOURAUD_SHADING:
            shader->Use(0);
            glShadeModel (GL_SMOOTH);
            break;
        case USE_FLAT_SHADING:
            shader->Use(0);
            glShadeModel (GL_FLAT);
            break;
    }

    glPushMatrix();
        glRotatef( rotationY, 0.0, 1.0, 0.0 );
        glRotatef( rotationX, 1.0, 0.0, 0.0 );
        glutSolidTeapot(1);
    glPopMatrix();

    glutSwapBuffers();
}

void idle ()
{
    glutPostRedisplay();
}

void reshape (int w, int h)
{
    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if(h == 0) h = 1;

    float ratio = 1.0* w / h;

    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set the correct perspective.
    gluPerspective(45,ratio,1,1000);
    glMatrixMode(GL_MODELVIEW);
}

void special(int key, int x, int y)
{
    float step = 0.05;
    switch (key)
    {
        case GLUT_KEY_LEFT:
            pos_luz[0]-=step;
            break;
        case GLUT_KEY_RIGHT:
            pos_luz[0]+=step;
            break;
        case GLUT_KEY_UP:
            pos_luz[1]+=step;
            break;
        case GLUT_KEY_DOWN:
            pos_luz[1]-=step;
            break;
        case GLUT_KEY_PAGE_DOWN:
            pos_luz[2]-=step;
            break;
        case GLUT_KEY_PAGE_UP:
            pos_luz[2]+=step;
            break;
    }
}

void keyboard (unsigned char key, int x, int y)
{
    switch (tolower(key))
    {
        case '1'...'3':
            selectMode = key - 48; // View ASCII table
            break;
        case 27:
            exit(0);
            break;
    }
}

// Motion callback
void motion(int x, int y )
{
    rotationX += (float) (y - last_y);
    rotationY += (float) (x - last_x);

    last_x = x;
    last_y = y;
}

// Mouse callback
void mouse(int button, int state, int x, int y)
{
    if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
    {
        last_x = x;
        last_y = y;
    }
    if(button == 3) // Scroll up
    {
        if(zoom>=0.3)  zoom-=0.1f;
    }
    if(button == 4)   // Scroll down
    {
        if(zoom<=20.0) zoom+=0.1f;
    }
}

void init(void)
{
    glClearColor (0.4, 0.4, 0.4, 0.0);
    glEnable(GL_DEPTH_TEST);
    glewInit();
    if (glewIsSupported("GL_VERSION_2_0"))
        printf("Ready for OpenGL 2.0\n");
    else
    {
        printf("OpenGL 2.0 not supported\n");
        exit(1);
    }

    // Create shader object
    shader = new glcShader("../shaders/phong.vert", "../shaders/phong.frag");

    printf("\nMenu");
    printf("\n====\n\n");
    printf("Keyboard:\n");
    printf("  '1' for Phong shading.\n");
    printf("  '2' for Gouraud shading.\n");
    printf("  '3' for Flat shading.\n");
    printf("  Use arrow keys and 'PageUp' e 'PageDown' to change light position.\n");
    printf("Press ESC to exit.\n");

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Define parametros da luz 0
    glLightfv(GL_LIGHT0, GL_AMBIENT, cor_luz);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, cor_luz);
    glLightfv(GL_LIGHT0, GL_SPECULAR, cor_luz);
    glLightfv(GL_LIGHT0, GL_POSITION, pos_luz);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (width, height);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("Shading Example (Flat, Gouraud & Phong");
    init ();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc( mouse );
    glutMotionFunc( motion );
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
