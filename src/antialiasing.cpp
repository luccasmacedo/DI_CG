/*
  Name:        antialiasing.cpp
  Copyright:   Version 0.1
  Author:      Rodrigo Luis de Souza da Silva
  Date:        16/09/2004
  Description: Compute several levels of antialiasing on a scene.
               One procedural 4x4 Jitter table was created.
               The avaiable modes for antialiasing are:
               OFF, 2x, 4x, 8x, 16x
               This code was based on the "RedBook" example
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

// Class Pixel
class Pixel
{
   public:
   double x, y;
   Pixel & operator=(const Pixel &p)
   {
      if (this == &p) return *this;
      this->x = p.x;
      this->y = p.y;
      return *this;
   }
};

// Global Vars -----------------------------------------------------------
// Main Jitter table
Pixel jitterTable[16];

// Adjust Jitter table
Pixel jitterUse[16];

int  	last_x, last_y;
int   globalButton;
int   width = 800;
int   height = 600;
float rotationX = 0.0;
float rotationY = 0.0;
float zoom = -4.0f;
int 	numInteractions = 1 ;

// Functions ---------------------------------------------------------------

// Antialising Frustum
void accFrustum(double left, double right, double bottom, double top,
 double nearvalue, double farvalue, double pixdx,double pixdy,
 double eyedx, double eyedy, double focus)
{
   double xwsize, ywsize;
   double dx, dy;

   GLint viewport[4];
   glGetIntegerv (GL_VIEWPORT, viewport);

   xwsize = right - left;
   ywsize = top - bottom;

   dx = -(pixdx*xwsize /(double) viewport[2]) + (eyedx*nearvalue)/focus;
   dy = -(pixdy*ywsize /(double) viewport[3]) + (eyedy*nearvalue)/focus;

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum (left + dx, right + dx, bottom + dy, top + dy, nearvalue, farvalue);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslatef (-eyedx, -eyedy, zoom);
}

// Antialising Perspective
void accPerspective(double fovy, double aspect, double nearvalue, double farvalue,
   double pixdx, double pixdy, double eyedx, double eyedy, double focus)
{
   double fov2,left,right,bottom,top;
   fov2 = ((fovy*M_PI) / 180.0) / 2.0;

   top = nearvalue / (cos(fov2) / sin(fov2));
   bottom = -top;
   right = top * aspect;
   left = -right;

   accFrustum (left, right, bottom, top, nearvalue, farvalue,pixdx, pixdy, eyedx, eyedy, focus);
}

// display objects
void displayObjects(void)
{
   GLfloat torus_diffuse[] = 	{ 0.7, 0.7, 0.0, 1.0 };
   GLfloat cube_diffuse[] = 	{ 0.0, 0.7, 0.7, 1.0 };
   GLfloat sphere_diffuse[] = { 0.7, 0.0, 0.7, 1.0 };

   glPushMatrix ();
      glRotatef( rotationY, 0.0, 1.0, 0.0 );
      glRotatef( rotationX, 1.0, 0.0, 0.0 );

      glPushMatrix ();
         glTranslatef (-0.80, 0.35, 0.0);
         glRotatef (100.0, 1.0, 0.0, 0.0);
         glMaterialfv(GL_FRONT, GL_DIFFUSE, torus_diffuse);
         glutSolidTorus (0.275, 0.475, 20, 20);
      glPopMatrix ();

      glPushMatrix ();
         glTranslatef (-0.75, -0.50, 0.0);
         glRotatef (45.0, 0.0, 0.0, 1.0);
         glRotatef (45.0, 1.0, 0.0, 0.0);
         glMaterialfv(GL_FRONT, GL_DIFFUSE, cube_diffuse);
         glutSolidCube (1.5);
      glPopMatrix ();

      glPushMatrix ();
         glTranslatef (0.75, 0.60, 0.0);
         glRotatef (30.0, 1.0, 0.0, 0.0);
         glMaterialfv(GL_FRONT, GL_DIFFUSE, sphere_diffuse);
         glutSolidSphere (1.0, 30, 30);
         glPopMatrix ();
      glPopMatrix ();

   glPopMatrix ();
}

// display callback
void display(void)
{
   glClear(GL_ACCUM_BUFFER_BIT);
   for (int jitter = 0; jitter < numInteractions; jitter++)
   {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      accPerspective(50.0, (double)width/ (double) height, 0.1f, 15.0f, jitterUse[jitter].x, jitterUse[jitter].y, 0.0, 0.0, 1.0);
      displayObjects ();
      glAccum(GL_ACCUM, 1.0/numInteractions);
   }
   glAccum (GL_RETURN, 1.0);
   glutSwapBuffers();
}

// Main Menu
void showMenu()
{
   printf("\n\n\n\n\n\n\nAntialiasing Example\n");
   printf(" -> Mouse: left button rotate and right button to zoom in and out\n");
   printf(" -> Keyboard: \n");
   printf("    * Press 1 to disable Antialiasing\n");
   printf("    * Press 2 to enable  2X Antialiasing\n");
   printf("    * Press 3 to enable  4X Antialiasing\n");
   printf("    * Press 4 to enable  8X Antialiasing\n");
   printf("    * Press 5 to enable 16X Antialiasing\n");
   printf("    * Press ESC to exit.\n");
}

// Create a 4 x 4 Jitter table with the following index
// 12	13	14	15
// 08	09 10	11
// 04	05	06	07
// 00 01 02 03
void createJitterTable()
{
   // Create Jitter Table
   float min = -0.44;	// Jitter Minimum Value
   float inc = 0.295;	// Step to add
   int size = 4;			// Number of lines and columns (4 means a 4x4 matrix)
   jitterTable[0].x = min;
   jitterTable[0].y = min;
   for(int i = 0; i < size; i++)
   {
      for(int j = 0; j < size; j++)
      {
         int index = i*size + j;
         if(j==0)
         {
            if(i==0) jitterTable[ 0 ].x     = min;
            else     jitterTable[ index ].x = jitterTable[index-1].x + inc;
            jitterTable[ index ].y = min;
         }
         else
         {
            jitterTable[ index ].x = jitterTable[ i*size ].x;
            jitterTable[ index ].y = jitterTable[ index-1 ].y + inc;
         }
      }
   }

   // Antialiasing is off in the beginning
   jitterUse[0].x = 0.0f;
   jitterUse[0].y = 0.0f;
}

void init(void)
{
   GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat light_position[] = { 0.0, 0.0, 10.0, 1.0 };
   GLfloat lm_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
   glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialf(GL_FRONT, GL_SHININESS, 50.0);
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lm_ambient);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glDepthFunc(GL_LEQUAL);
   glEnable(GL_DEPTH_TEST);
   glShadeModel (GL_FLAT);
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glClearAccum(0.0, 0.0, 0.0, 0.0);

   createJitterTable();
   showMenu();
}

// Keyboard callback
void keypress(unsigned char c, int a, int b)
{
   showMenu();
   if ( c == 27 ) exit(0);

   // Base Jitter Indexes
   // 12	13	14	15
   // 08	09 10	11
   // 04	05	06	07
   // 00 01 02 03
   int jitter2xIndex[2] = {5, 10};
   int jitter4xIndex[4] = {5, 10, 6, 9};
   int jitter8xIndex[8] = {5, 10, 6, 0, 15, 12, 3};

   if ( c == '1')
   {
      printf(" Selected:\n   Antialiasing OFF\n\n");
      jitterUse[0].x = jitterUse[0].y= 0.0f;
      numInteractions = 1;
   }
   if ( c == '2')
   {
      printf(" Selected:\n   Antialiasing 2x\n\n");
      numInteractions = 2;
      for(int i = 0; i < numInteractions; i++)
         jitterUse[i] = jitterTable[ jitter2xIndex[i] ];
   }
   if ( c == '3')
   {
      printf(" Selected:\n   Antialiasing 4x\n\n");
      numInteractions = 4;
      for(int i = 0; i < numInteractions; i++)
         jitterUse[i] = jitterTable[ jitter4xIndex[i] ];
   }
   if ( c == '4')
   {
      printf(" Selected:\n   Antialiasing 8x\n\n");
      numInteractions = 8;
      for(int i = 0; i < numInteractions; i++)
         jitterUse[i] = jitterTable[ jitter8xIndex[i] ];
   }
   if ( c == '5')
   {
      printf(" Selected:\n   Antialiasing 16x\n\n");
      numInteractions = 16;
      for(int i = 0; i < numInteractions; i++)
         jitterUse[i] = jitterTable[i];
   }
   glutPostRedisplay();
}

// Resize callback
void resize(int w, int h)
{
   width = w;
   height = h;
   glViewport(0, 0, w, h);
}

// Mouse callback
void mouse(int button, int state, int x, int y)
{
   if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
   {
      last_x = x;
      last_y = y;
   }
   globalButton = button;
}

// Motion callback
void motion(int x, int y )
{
   if(globalButton == GLUT_LEFT_BUTTON)
   {
      rotationX += (float) (y - last_y);
      rotationY += (float) (x - last_x);
   }
   else
   {
      if(y < last_y) zoom+=0.05;
      else          	zoom-=0.05;
   }
   last_x = x;
   last_y = y;

   glutPostRedisplay();
}

// Main Function
int main(int argc, char * argv[])
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_ACCUM);
   glutInitWindowSize(width,height);
   glutCreateWindow("Antialiasing Example");
   glutReshapeFunc(resize);
   glutKeyboardFunc(keypress);
   glutMotionFunc( motion );
   glutMouseFunc( mouse );
   glutDisplayFunc( display );
   init();
   glutMainLoop();
   return 0;
}
