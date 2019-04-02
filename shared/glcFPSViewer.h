/*
  Name:        glcFPSViewer.h
  Copyright:   Version 0.1
  Author:      Rodrigo Luis de Souza da Silva
  Date:        08/10/2007
  Description: Compute and show FPS of an graphic application

   Examples of use:

	--------------------------------------------------------------
	--> Include this file
	#include "glcFPSViewer.h"

	--------------------------------------------------------------
	--> Create (globally) an instance of this class

	// Create the glcFPSViewer object
	glcFPSViewer *fpsViewer = new glcFPSViewer();

	or

	// Create the glcFPSViewer setting suffix and prefix
	// It will print for example: " Prog - FPS: 132 - Press ESC to Exit");
	glcFPSViewer *fpsViewer = new glcFPSViewer("Prog - ", " - Press ESC to Exit");

	--------------------------------------------------------------
	--> Put the line above on your "draw" callback
   // FPS will be printed in the glut's title bar
	fpsViewer->drawFPS();
*/

#ifndef FPS_VIEWER
#define FPS_VIEWER

class glcFPSViewer
{
    private:
        char prefix[64];
        char suffix[64];
    public:
        glcFPSViewer();
        glcFPSViewer(char *suf, char *pre);
        void setSuffix(char *);
        void setPrefix(char *);
        void drawFPS();
};

#endif
