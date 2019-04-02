/*
  Name:          zbuffer_backface.cpp
  Copyright:     Version 0.1
  Author:        Rodrigo Luis de Souza da Silva
  Last Update:	 24/11/2013
  First Release: 03/11/2006
  Description:
      Programa para verificar funcionamento do Z-Buffer e BackFace Culling.
      Obs.: Para testar o Backface Culling mantenha a iluminação ligada e zbuffer desligado.
*/

#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

float rotationX = 0.0, rotationY = 0.0;
int   last_x, last_y;
bool light     = true;
bool zbuffer   = true;
bool backface  = true;
int width = 500, height = 500;

int distOrigem = 30;

void init(void)
{
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_SMOOTH);

    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);           // Utiliza cor do objeto como material
    glColorMaterial(GL_FRONT, GL_DIFFUSE);

    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);               // Habilita Z-buffer
    glEnable(GL_CULL_FACE);                // Habilita Backface-Culling

    printf("\nMenu");
    printf("\n====");
    printf("\nPressione 'L' para ligar/desligar iluminação");
    printf("\nPressione 'Z' para ligar/desligar z-buffer");
    printf("\nPressione 'B' para ligar/desligar Backface Culling");
    printf("\nPressione '+' ou '-' para efeitos de zoom\n");
}

void display(void)
{
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(60.0, (GLfloat) width/(GLfloat) height, 1.0, 200.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity ();
    gluLookAt (0.0, 0.0, distOrigem, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glPushMatrix();
        glRotatef( rotationY, 0.0, 1.0, 0.0 );
        glRotatef( rotationX, 1.0, 0.0, 0.0 );

        // Esfera amarela inicialmente a frente
        glTranslatef(0, 0, 10);
        glColor3f (0.80, 0.60, 0.0);
        glutSolidSphere(7, 20, 20);

        // Esfera azulada inicialmente atrás (desligando zbuffer, fica na frente)
        glTranslatef(0, 0, -20);
        glColor3f (0.283, 0.456, 0.518);
        glutSolidSphere(7, 20, 20);
    glPopMatrix();

    glutSwapBuffers();
}

void idle ()
{
    glutPostRedisplay();
}

void reshape (int w, int h)
{
    width = w;
    height = h;
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
}

void keyboard (unsigned char key, int x, int y)
{
    switch (tolower(key))
    {
        case 'l'	:
            light = !light;
            (!light) ? glDisable(GL_LIGHTING)        : glEnable(GL_LIGHTING);
            (light) ? printf("Iluminacao ligada.\n") : printf("Iluminacao desligada.\n");
            break;
        case 'z'	:
            zbuffer = !zbuffer;
            (!zbuffer) ? glDisable(GL_DEPTH_TEST)   : glEnable(GL_DEPTH_TEST)     ;
            (zbuffer) ? printf("Z-Buffer ligado.\n"): printf("Z-Buffer desligado.\n");
            break;
        case 'b'	:
            backface = !backface;
            (!backface) ? glDisable(GL_CULL_FACE) : glEnable(GL_CULL_FACE)  ;
            (backface) ? printf("Backface Culling ligado.\n")   : printf("Backface Culling desligado.\n");
            break;
        case '+' :
            distOrigem--;
            if(distOrigem<20)
                distOrigem=20;
            break;
        case '-' :
            distOrigem++;
            if(distOrigem>180)
                distOrigem=180;
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
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (width, height);
    glutInitWindowPosition (100, 100);
    glutCreateWindow (argv[0]);
    init ();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc( mouse );
    glutMotionFunc( motion );
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
