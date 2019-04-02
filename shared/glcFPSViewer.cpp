/*
  Name:        glcFPSViewer.h
  Copyright:   Version 0.1
  Author:      Rodrigo Luis de Souza da Silva
  Date:        08/10/2007
  Description: Compute and show FPS of an graphic application
*/

#include "glcFPSViewer.h"
#include <stdio.h>
#include <string.h>
#include <GL/glut.h>

// Empty constructor
glcFPSViewer::glcFPSViewer()
{
    this->setSuffix((char*) "");
    this->setPrefix((char*) "");
}

// Constructor setting suffix and prefix
glcFPSViewer::glcFPSViewer(char *suf, char *pre)
{
    this->setSuffix(suf);
    this->setPrefix(pre);
}

// Setting Suffix
void glcFPSViewer::setSuffix(char *suf)
{
    strcpy(this->suffix, suf);
}

// Setting Prefix
void glcFPSViewer::setPrefix(char *pre)
{
    strcpy(this->prefix, pre);
}

// Compute and show FPS
void glcFPSViewer::drawFPS()
{
    char aux[16];
    static char fpsBuf[256] = {0};
    static int frames = 0;
    static int t0 = 0;
    frames++;
    GLint t = glutGet(GLUT_ELAPSED_TIME);
    if (t - t0 >= 1000)
    {
        GLfloat seconds = (t - t0) / 1000.0;
        GLfloat fps = frames / seconds;
        sprintf(aux, "FPS: %.1f", fps);

        strcpy(fpsBuf, this->suffix);
        strcat(fpsBuf, aux);
        strcat(fpsBuf, this->prefix);

        t0 = t;
        frames = 0;
    }
    glutSetWindowTitle(fpsBuf);
}
