/*
  Name:        05_viewport
  Copyright:   Version 0.1
  Author:      Rodrigo Luis de Souza da Silva
  Date:        15/10/2013
  Description: Testing more than one viewports
*/

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float altura = 400, largura = 400;

void  DesenhaEixos();
void  DesenhaObjetos1();
void  display(void);
void  keyboard(unsigned char key, int x, int y);
void  init(void);

//////////////////////////////////////////////////
// Main Function
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    glutInitWindowSize ((int)largura,(int)altura);
    glutInitWindowPosition (20,20);
    glutCreateWindow ("Viewport Test");
    init ();
    glutDisplayFunc(display);  // Eh quem ira chamar todo o restante das funçoes
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}

//////////////////////////////////////////////////
// Functions

// Inicializa opengl
void init(void)
{
    glClearColor(0.0,0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);
}

// Eixos coordenados
void DesenhaEixos()
{
    glBegin(GL_LINES);
        glColor3f (1.0, 0.0, 0.0);
        glVertex3f(-10.0, 0.0, 0.0);
        glVertex3f( 10.0, 0.0, 0.0);
        glColor3f (  0.0,   1.0, 0.0);
        glVertex3f(0.0, -10.0, 0.0);
        glVertex3f(0.0,  10.0, 0.0);
    glEnd();
}

void DesenhaObjetos()
{
    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
        glVertex3f(2.0, 2.0, 0.0);
        glVertex3f(8.0, 2.0, 0.0);
        glVertex3f(2.0, 8.0, 0.0);
    glEnd();

    glColor3f(0.0, 1.0, 1.0);
    glBegin(GL_LINE_STRIP);
        glVertex3f(2.0, -2.0, 0.0);
        glVertex3f(8.0, -2.0, 0.0);
        glVertex3f(2.0, -8.0, 0.0);
        glVertex3f(2.0, -2.0, 0.0);
    glEnd();

    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
        glVertex3f(-2.0, -2.0, 0.0);
        glVertex3f(-8.0, -2.0, 0.0);
        glColor3f(0.5, 0.5, 0.5);
        glVertex3f(-8.0, -2.0, 0.0);
        glVertex3f(-2.0, -8.0, 0.0);
        glColor3f(1.0, 1.0, 1.0);
        glVertex3f(-2.0, -8.0, 0.0);
        glVertex3f(-8.0, -8.0, 0.0);
    glEnd();
}

///////////////////////////////////////////////////////////
// Glut and image functions

void desenhaCantoSuperiorEsquerdo(){
    glViewport ((int) 0, (int) 205, (int) 195, (int) 195);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glOrtho(-10.0, 10.0, -10.0, 10.0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    DesenhaEixos();
    DesenhaObjetos();
}
void display(void)
{
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Define viewport do canto superior esquerdo
    desenhaCantoSuperiorEsquerdo();

    // Define viewport do canto inferior esquerdo
    glViewport ((int) 0, (int) 0, (int) 195, (int) 195);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glOrtho(-10.0, 1, -10.0, 1, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    DesenhaEixos();
    DesenhaObjetos();

    // Define viweport superior direita
    glViewport ((int) 205, (int) 205, (int) 195, (int) 195);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glOrtho(-1, 10, -1, 10.0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    DesenhaEixos();
    DesenhaObjetos();

    glViewport ((int) 205, (int) 0, (int) 195, (int) 195);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glOrtho(-1, 10, -10, 1, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    DesenhaEixos();
    DesenhaObjetos();
    glColor3f(0.0,0.4,0.3);

    glutSwapBuffers();
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

