/*
  Raytracing Project
  Copyright:   Version 0.1
  Author:      Rodrigo Luis de Souza da Silva
  Last Update: 15/06/2018
  Release:     23/10/2004
  Description: Simple Raytracing Project
               This project is an adaptation of the source code available here:
               https://code.google.com/p/glraytracer/
*/
#include <stdio.h>
#include <string.h>
#include "rayTracer.h"

using namespace std;
char cenario[512] = {"../data/rayTracingScene01.txt"};

void init();
void display();
void keyboard(unsigned char key, int x, int y);
void specialKey(int key, int x, int y) {};
void mouseButton(int button, int state, int x, int y) {};
void idleFunc();

// Globals
glraytracer *glProject = NULL;

int main(int argc, char** argv)
{
    // Use arguments of main function to load the scenario
    if(argc>1)
    {
        strcpy(cenario, argv[1]);
    }
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    /* create project and read input file */
    glProject = new glraytracer();
    glProject->readDatabase(cenario);

    int *w = glProject->getReader()->getWindowDimensions();

    // set up the physical window size
    glutInitWindowSize(w[0], w[1]);

    // set the name of the window and try to create it
    int window = glutCreateWindow("GL Raytracer");

    // Create Orthographic projection Matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w[0], 0, w[1]);

    // Compute raytrace only once
    glProject->setWindow(window);
    glProject->raytrace();

    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKey);
    glutMouseFunc(mouseButton);
    glutDisplayFunc(display);
    glutIdleFunc(idleFunc);

    glutMainLoop();
    return (0);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glProject->draw();

    glutSwapBuffers();
}

void idleFunc()
{
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
            exit(0);
        break;
    }
    glutPostRedisplay();
}
