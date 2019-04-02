/*
  Name:        quad.cpp
  Copyright:   Version 0.1
  Author:      Rodrigo Luis de Souza da Silva
  Last Update: 03/09/2014
  Release:     18/09/2004
  Description: Simple opengl program
*/

#include <stdio.h>
#include <GL/glut.h>
#include <math.h>

void display(void);
void init (void);
void keyboard(unsigned char key, int x, int y);
bool aPressed = false;
bool bPressed = false;
bool cPressed = false;


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize (400, 400);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("Quad Test");
    init ();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    printf("Pressione ESC para fechar.\n");

    glutMainLoop();

    return 0;
}

void display(void)
{
    // Limpar todos os pixels
    glClear (GL_COLOR_BUFFER_BIT);

    // Desenha os eixos
    glBegin(GL_LINES);
        glColor3f(1.0, 1.0, 1.0);
        glVertex2f(-10.0,0.0);
        glVertex2f(390.0,0.0);
        glVertex2d(0.0,-2.0);
        glVertex2d(0.0, 2.0);
    glEnd();


    //Identifica a tecla pressionada e desenha a figura associada
    if(aPressed){
        float parte = 360/6;
        glColor3f(0.0, 1.0, 0.0);

        glBegin(GL_LINE_STRIP);
        for(float x = 0.0; x <= 360; x += parte){
            glVertex2d(x,sin((x*M_PI)/180));
        }
        glEnd();
    }else{
        if(bPressed){
            float parte = 360/12;
            glColor3f(0.0, 1.0, 0.0);

            glBegin(GL_LINE_STRIP);
            for(float x = 0.0; x <= 360; x += parte){
                glVertex2d(x,sin((x*M_PI)/180));
            }
            glEnd();
        }else{
            if(cPressed){
                float parte = 360/180;
                glColor3f(0.0, 1.0, 0.0);

                glBegin(GL_LINE_STRIP);
                for(float x = 0.0; x <= 360; x += parte){
                    glVertex2d(x,sin((x*M_PI)/180));
                }
                glEnd();
            }
        }
    }


    glutSwapBuffers ();
}


void init (void)
{
    // selecionar cor de fundo (preto)
    glClearColor (0.0, 0.0, 0.0, 0.0);

    // inicializar sistema de viz.
    glMatrixMode(GL_PROJECTION);              // Seleciona Matriz de Projeção
    glLoadIdentity();
    glOrtho(-10.0, 390, -2, 2, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW); // Select The Modelview Matrix
    glLoadIdentity();           // Inicializa com matriz identidade
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'a':
            aPressed = true;
            bPressed = cPressed = false;
            break;
        case 'b':
            bPressed = true;
            aPressed = cPressed = false;
            break;
        case 'c':
            cPressed = true;
            aPressed = bPressed = false;
            break;
    }
    glutPostRedisplay();
}
