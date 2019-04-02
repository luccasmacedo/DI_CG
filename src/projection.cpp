/*
  Name:        	    projection.cpp
  Author:      	    Rodrigo Luis de Souza da Silva
  Last Update:		20/05/2016
  First Release:	03/11/2006
  Description:
    Código para demonstrar a diferença entre projeção perspectiva e ortogonal em OpenGL.
*/

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

float angle = 0, raio = 0.8;
int enableMenu = 0;
int dist = 25;
int proj = 0;
int width = 640;
int height = 640;
float viewangle = 40.0f;

float rotationX = 38.0, rotationY = 22.0;
int   last_x, last_y;

void showMenu()
{
    system("cls");
    printf("\nPROJECTION\n==========\n\n");
    printf("Mouse\n");
    printf("* Clique com o botao esquerdo para mover camera ou objeto.\n");
    printf("* Clique com o botao direito para alterar modo de projecao (Ortogonal por padrao).\n");
    printf("Teclado\n");
    printf("* ESC - Para sair\n");
}

//Reshape Func
void reshape( int w, int h)
{
    width = w;
    height = h;
    glViewport(0,0,w,h);

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

void desenhaEixos()
{
    glDisable(GL_LIGHTING);
    glColor3f (0.0, 1.0, 0.0);
    glBegin(GL_LINES);
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f (-dist, 0.0, 0.0);
        glVertex3f ( dist, 0.0, 0.0);
        glColor3f(0.0, 1.0, 0.0);
        glVertex3f (0.0, -dist, 0.0);
        glVertex3f (0.0,  dist, 0.0);
        glColor3f(0.0, 0.0, 1.0);
        glVertex3f (0.0, 0.0, -dist);
        glVertex3f (0.0, 0.0,  dist);
    glEnd();
    glEnable(GL_LIGHTING);
}

// Motion callback
void motion(int x, int y )
{
    rotationY += (float) (y - last_y);
    rotationX += (float) (x - last_x);

    last_x = x;
    last_y = y;

    glutPostRedisplay();
}

// Mouse callback
void mouse(int button, int state, int x, int y)
{
    if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
    {
        last_x = x;
        last_y = y;
    }
    if ( button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN )
    {
        proj= !proj;
        if(!proj) printf("Projecao Ortogonal.\n");
        else      printf("Projecao Perspectiva.\n");
    }
}

void display(void)
{
    // Limpar todos os pixels
    glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    float w = width;
    float h = height;

    // inicializar sistema de projeção
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    int ortho = 10;
    if(!proj)
    {
        // glOrtho(-orho, orho, -orho, orho, -200, 200);
        if (width <= height)
            glOrtho (-ortho, ortho, -ortho*h/w, ortho*h/w, -100.0, 100.0);
        else
            glOrtho (-ortho*w/h, ortho*w/h, -ortho, ortho, -100.0, 100.0);
    }
    else
        gluPerspective(40.0f,(GLfloat)width/(GLfloat)height,0.1f,200.0f);	// Calculate The Aspect Ratio Of The Window

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt (0.0, 0.0, 30.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glPushMatrix();
        // Rotação dos objetos. Luz esta fixa na posição (0, 0, 5)
        glRotatef( rotationY, 1.0, 0.0, 0.0 );
        glRotatef( rotationX, 0.0, 1.0, 0.0 );

        glColor3f (1.0, 1.0, 1.0);
        glDisable(GL_LIGHTING);
        glutWireCube(10);
        desenhaEixos();
        glEnable(GL_LIGHTING);

        glColor3f (0.283, 0.456, 0.518);
        glutSolidCube(5);

        glTranslatef(0, 0, 5);
        glColor3f (1.0, 1.0, 1.0);
        glutSolidSphere(2, 50, 50);
    glPopMatrix();

    glutSwapBuffers ();
    glutPostRedisplay();
}


void init (void)
{
    // selecionar cor de fundo (preto)
    glClearColor (0.0, 0.0, 0.0, 0.0);

    glEnable(GL_LIGHT0);                   // habilita luz 0
    glEnable(GL_COLOR_MATERIAL);           // Utiliza cor do objeto como material
    glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);

    GLfloat light0_position[] = {2.0f, 2.0f, 5.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

    glEnable(GL_LIGHTING);                 // Habilita luz
    glEnable(GL_DEPTH_TEST);               // Habilita Z-buffer
    glEnable(GL_CULL_FACE);                // Habilita Backface-Culling
}


int main(int argc, char** argv)
{
    showMenu();
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE|GLUT_DEPTH|GLUT_RGB);
    glutInitWindowSize (width, height);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("Projeções em OpenGL");
    init ();
    glutMouseFunc( mouse );
    glutKeyboardFunc(keyboard);
    glutMotionFunc( motion );
    glutReshapeFunc( reshape );
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}

