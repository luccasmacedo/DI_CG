/*
  Name:        animationBase.cpp
  Copyright:   Version 0.1
  Author:      Rodrigo Luis de Souza da Silva
  Last Update:	23/08/2016
  Date:        23/08/2016
  Description: Arquivo base para o exercício da auila de animação
*/

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "extras.h"
#include <math.h>

// Globals
int width  = 1000;
int height = 500;
float varX = 0.0f;
float varY = 0.0f;
float dir = 1.0f;
float desiredFPS = 60;
float rotationX = 0.0, rotationY = 0.0;
int   last_x, last_y;

void idle()
{
    float t, desiredFrameTime, frameTime;
    static float tLast = 0.0;

    // Get elapsed time
    t = glutGet(GLUT_ELAPSED_TIME);
    // convert milliseconds to seconds
    t /= 1000.0;

    // Calculate frame time
    frameTime = t - tLast;
    // Calculate desired frame time
    desiredFrameTime = 1.0 / (float) (desiredFPS);

    // Check if the desired frame time was achieved. If not, skip animation.
    if( frameTime <= desiredFrameTime)
        return;

    // **  UPDATE ANIMATION VARIABLES ** //
    float step = 1; // Speed of the animation
    if(varX <= -0.001 || varX >= 1000)
        dir *= -1;
    varX+=dir* ((step / desiredFPS) + 1) ; // Variation computed considering the FPS
    // Update tLast for next time, using static local variable
    varY = varX*varX;
    tLast = t;
    glutPostRedisplay();
}

void init(void)
{
    glClearColor (0.5, 0.5, 0.5, 0.0);
    glShadeModel (GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);               // Habilita Z-buffer
    initLight(width, height);
}

void display(void)
{
    float sphereSize = 30.0;
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glOrtho(0.0, width, 0.0, height, -sphereSize, sphereSize);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity ();

    setMaterial();

    glTranslatef(sphereSize/2.0f + varX, sphereSize/2.0f + varY, 0.0); // Posicionamento inicial da esfera
    glutSolidSphere(20.0, 100, 100);

    glutSwapBuffers();
}

void reshape (int w, int h)
{
    width = w;
    height = h;
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
}

void keyboard (unsigned char key, int x, int y)
{
    if(tolower(key) == 27) exit(0);
}

// Motion callback
void motion(int x, int y )
{
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (width, height);
    glutInitWindowPosition (100, 100);
    glutCreateWindow("Animation Base");
    init ();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMotionFunc( motion );
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
