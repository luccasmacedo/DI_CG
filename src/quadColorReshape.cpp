/*
  Name:        quad_color_reshape.cpp
  Copyright:   Version 0.1
  Author:      Rodrigo Luis de Souza da Silva
  Last Update: 18/10/2013
  Release:     28/10/2004
  Description: Colored quad with reshape function enabled
*/

#include <stdio.h>
#include <GL/glut.h>

void display(void);
void reshape(int, int);
void init (void);
void keyboard(unsigned char key, int x, int y);

bool fullScreen = false;
int width = 250, height = 250;

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize (width, height);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("Quad Reshape Test");
    init ();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    printf("Pressione 'f' para habilitar/desabilitar o modo fullscreen.\n");
    printf("Pressione ESC para fechar.\n");

    glutMainLoop();

    return 0;
}

// É a rotina chamada automaticamente sempre que a
// janela ou parte dela precisa ser redesenhada
void display(void)
{
    // Limpar todos os pixels
    glClear (GL_COLOR_BUFFER_BIT);

    // Desenhar um polígono colorido (retângulo)
    glBegin(GL_POLYGON);
        glColor3f (0.0, 0.0, 1.0);
        glVertex3f (-0.5, -0.5, 0.0);
        glColor3f (0.0, 1.0, 0.0);
        glVertex3f (0.5, -0.5, 0.0);
        glColor3f (1.0, 0.0, 0.0);
        glVertex3f (0.5, 0.5, 0.0);
        glColor3f (1.0, 1.0, 1.0);
        glVertex3f (-0.5, 0.5, 0.0);
    glEnd();

    glutSwapBuffers ();
}

void reshape (int w, int h)
{
    // Guarda informações de altura e largura do modo janela
    if(!fullScreen)
    {
        width = w;
        height = h;
    }

    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();

    if (w <= h)
        glOrtho (-1, 1, -1*(GLfloat)h/(GLfloat)w, 1*(GLfloat)h/(GLfloat)w, -10.0, 10.0);
    else
        glOrtho (-1*(GLfloat)w/(GLfloat)h, 1*(GLfloat)w/(GLfloat)h, -1, 1, -10.0, 10.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void init (void)
{
    // selecionar cor de fundo (preto)
    glClearColor (0.0, 0.0, 0.0, 0.0);
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27:
            exit(0);
            break;
        case 'f':
            fullScreen = !fullScreen;
            if (fullScreen)
            {
                glutFullScreen();
            }
            else
            {
                glutReshapeWindow(width, height);
            }
            break;
    }
    glutPostRedisplay();
}
