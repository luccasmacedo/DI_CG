/*
  Name:        glcMouse.h
  Copyright:   Version 0.1
  Author:      Rodrigo Luis de Souza da Silva
  Date:        31/10/2008
  Description: Mouse class for OpenGL Programs
*/

#ifndef _GLC_MOUSE_H
#define _GLC_MOUSE_H

class glcMouse
{
    public:
        glcMouse();
        ~glcMouse();
        void idle ();
        void GetMouseEvent(int button, int state, int x, int y);
        void GetMotionEvent(int x, int y ) ;
        void GetZoom(double *);
        void GetPan(double *);
        void GetRotation(double *);


    protected:
        float rotationX,rotationY ;
        int   last_x, last_y;
        double   lastzoom ;
        int width, height ;
        double zoom ;
        int clickedbutton ; // 0 - left 1 - right
        double panx, pany  ;
};

#endif
