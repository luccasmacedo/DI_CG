/*
  Name:        shadowSimple
  Copyright:   Version 0.1
  Author:      Rodrigo Luis de Souza da Silva
  Date:        15/04/2008
  Description: Calculates shadows of some objects over a plane
               This code was an adaptation of:
               http://www.devmaster.net/articles/shadowprojection/
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

float l[] = { 0.0,  20.0, 0.0 }; // the position of the light source
float n[] = { 0.0,  -1.0, 0.0 }; // is the normal vector of the plane.
float e[] = { 0.0,   0.0, 0.0 }; // point on within the plane on which the shadow is to be projected.
float shadow[] = {0.25f, 0.25f, 0.25f, 1.0f}; // Shadow color

int 	width = 800;
int 	height = 600;
float rotationX = 0.0, rotationY = 0.0;
float step = 0.5f;
int 	globalButton;
int 	zoom = 60;
int   last_x, last_y;

void init()
{
   glEnable(GL_NORMALIZE);
   glEnable(GL_LIGHTING);
   glEnable(GL_COLOR_MATERIAL);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_LIGHT0);

   printf("Instructions\n");
   printf("------------\n");
   printf(" Use the directionals keys and page up/down to move light source\n");
   printf(" Mouse can be used to rotate (left button) or zoom in and out (right button) the camera.\n\n");
}

/*
 * Multiply the current ModelView-Matrix with a shadow-projetion matrix.
 * Everything that is drawn after this call is "squashed" down
 * to the plane. Hint: Gray or black color and no lighting
 * looks good for shadows *g*
 * Details of the algorithm in:
 *    http://www.devmaster.net/articles/shadowprojection/
 */
void glShadowProjection(float * l, float * e, float * n)
{
   float d, c;
   float mat[16];

   // These are c and d (corresponding to the tutorial)
   d = n[0]*l[0] + n[1]*l[1] + n[2]*l[2];
   c = e[0]*n[0] + e[1]*n[1] + e[2]*n[2] - d;

   // Create the matrix. OpenGL uses column by column ordering
   mat[0]  = l[0]*n[0]+c;
   mat[4]  = n[1]*l[0];
   mat[8]  = n[2]*l[0];
   mat[12] = -l[0]*c-l[0]*d;

   mat[1]  = n[0]*l[1];
   mat[5]  = l[1]*n[1]+c;
   mat[9]  = n[2]*l[1];
   mat[13] = -l[1]*c-l[1]*d;

   mat[2]  = n[0]*l[2];
   mat[6]  = n[1]*l[2];
   mat[10] = l[2]*n[2]+c;
   mat[14] = -l[2]*c-l[2]*d;

   mat[3]  = n[0];
   mat[7]  = n[1];
   mat[11] = n[2];
   mat[15] = -d;

   // Finally multiply the matrices together *plonk*
   glMultMatrixf(mat);
}

// Draw colored and shadowed objects
void draw(int IsShadow)
{
   glPushMatrix();
      float dist = 15.0f;
      (!IsShadow) ? glColor3f(0.0,0.0,0.8) : glColor3fv(shadow);
      glTranslatef(-dist, 3.65f, -dist);
      glutSolidCube(7.30f);
      (!IsShadow) ? glColor3f(0.8,0.0,0.0) : glColor3fv(shadow);
      glTranslatef(dist, 0.0f, dist);
      glutSolidTeapot(5.0);
      (!IsShadow) ? glColor3f(0.0,0.8,0.0) : glColor3fv(shadow);
      glTranslatef(dist, 0.0f, dist);
      glutSolidSphere(3.65f, 30, 30);
   glPopMatrix();
}

void render()
{
   glClearColor(0.0,0.0,0.0,0.0);
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(40.0f, (float) width/height, 1.0, 1024.0);

   // Reset the coordinate system before modifying
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt (0.0, zoom/2, zoom, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   glEnable(GL_LIGHTING);
   glLightfv(GL_LIGHT0, GL_POSITION,l);

   glPushMatrix();
      glRotatef( rotationY, 0.0, 1.0, 0.0 );
      glRotatef( rotationX, 1.0, 0.0, 0.0 );

      glColor3f(1.0,1.0,0.0);
      glPushMatrix();
      glTranslatef(l[0],l[1],l[2]);
      glutSolidSphere(0.5f, 30, 30);
      glPopMatrix();

      // Draw the object that casts the shadow
      draw(0);

      glDisable(GL_LIGHTING);
      // Now, we draw the plane onto which the shadow should fall
      // The Y-Coordinate of the plane is reduced by 0.1 so the plane is
      // a little bit under the shadow. We reduce the risk of Z-Buffer
      // flittering this way.
      glColor3f(0.6,0.6,0.6);
      glBegin(GL_QUADS);
         glNormal3f(0.0,1.0,0.0);
         float size = 80.0f;
         glVertex3f(-size,e[1]-0.1, size);
         glVertex3f( size,e[1]-0.1, size);
         glVertex3f( size,e[1]-0.1,-size);
         glVertex3f(-size,e[1]-0.1,-size);
      glEnd();

      // Now we draw the shadow
      glShadowProjection(l,e,n);
      draw(1);
   glPopMatrix();

   glutSwapBuffers();
}

void keypress(unsigned char c, int a, int b)
{
   if ( c==27 ) exit(0);
   glutPostRedisplay();
}

void special(int c, int x, int y)
{
   if      ( c == GLUT_KEY_DOWN )       l[1]-=step;
   else if ( c == GLUT_KEY_UP )         l[1]+=step;
   else if ( c == GLUT_KEY_LEFT )       l[0]-=step;
   else if ( c == GLUT_KEY_RIGHT )      l[0]+=step;
   else if ( c == GLUT_KEY_PAGE_DOWN )  l[2]-=step;
   else if ( c == GLUT_KEY_PAGE_UP )    l[2]+=step;

   // Light Limits
   if(l[1] < 15.0f)  l[1] =  15.0f;
   if(l[0] < -45.0f) l[0] = -45.0f;
   if(l[0] >  45.0f) l[0] =  45.0f;
   if(l[2] < -45.0f) l[2] = -45.0f;
   if(l[2] >  45.0f) l[2] =  45.0f;

   glutPostRedisplay();
}

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
      if(y < last_y) zoom++;
      else           zoom--;
   }
   last_x = x;
   last_y = y;
   glutPostRedisplay();
}

int main(int argc, char * argv[])
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutCreateWindow("Shadow Example");
   glutReshapeFunc(resize);
   glutReshapeWindow(width,height);
   glutKeyboardFunc(keypress);
   glutSpecialFunc(special);
   glutMotionFunc( motion );
   glutMouseFunc( mouse );
   glutDisplayFunc(render);
   init();
   glutMainLoop();
   return 0;
}

