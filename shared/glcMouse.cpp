#include "glcMouse.h"
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>

glcMouse::glcMouse()
{
    rotationX = rotationY = 0.0;
    lastzoom = 0;
    width =  height = 500;
    zoom = 20.0;
    clickedbutton = 0; // 0 - left 1 - right
    panx = 0.0f, pany = 0.0f;
}

glcMouse::~glcMouse()
{
}

void glcMouse::idle ()
{
    glutPostRedisplay();
}

// glcMouse callback
void glcMouse::GetMouseEvent(int button, int state, int x, int y)
{
    // Rotation
    if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
    {
        last_x = x;
        last_y = y;
        clickedbutton = 0;
    }
    // Zoom
    if ( button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN )
    {
        last_x = x;
        last_y = y;
        clickedbutton = 1;
    }
    // Pan
    if ( button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN )
        clickedbutton = 2;
}

void glcMouse::GetMotionEvent(int x, int y )
{
    switch(clickedbutton)
    {
        case 0:
            rotationX += (float) (y - last_y);
            rotationY += (float) (x - last_x);
            last_x = x;
            last_y = y;
            break;
        case 1:
            if(y < lastzoom)
                zoom+=0.5;
            else
                zoom-=0.5;
            lastzoom = y;
            break;
        case 2:
            double difx = abs(last_x - x);
            double dify = abs(last_y - y);
            if(difx>1)
                difx = 1;
            if(dify>1)
                dify = 1;
            panx += (x > last_x) ? -difx/5 : difx/5;
            pany += (y < last_y) ? -dify/5 : dify/5;
            last_x = x;
            last_y = y;
            break;
    }
}

void glcMouse::GetZoom(double *z)
{
    *z = this->zoom;
}

void glcMouse::GetPan(double *pan)
{
    pan[0] = this->panx;
    pan[1] = this->pany;
}

void glcMouse::GetRotation(double *rotation)
{
    rotation[0] = this->rotationX;
    rotation[1] = this->rotationY;
}

