/*
  Name:        shading
  Copyright:   Version 0.1
  Author:      Rodrigo Luis de Souza da Silva
  Last Update: 02/12/2013
  Date:        02/12/2013
  Description: Codigo para visualização dos métodos de shading (flat e gouraud)
*/

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// Controla modo de shading
bool smooth = false;
int resolution = 20;
bool sphere = true;

// Material do object (material branco)
GLfloat object_ambient[]   = { 0.3, 0.0, 0.0, 1.0 };
GLfloat object_difusa[]    = { 1.0, 0.0, 0.0, 1.0 };
GLfloat object_especular[] = { 1.0, 0.9, 0.9, 1.0 };
GLfloat object_emissao[] =   { 0.0, 0.0, 0.0, 0.0 };
GLfloat object_brilho[]    = { 64.0 };

// Informações sobre a luz
GLfloat cor_luz_ativa[]    = { 1.0, 1.0, 0.0, 1.0};
GLfloat cor_luz0[]         = { 1.0, 1.0, 1.0, 1.0};
GLfloat cor_luz_amb[]      = { 0.1, 0.1, 0.1, 1.0};
GLfloat posicao_luz0[]     = { 8.0, 8.0, 15.0, 1.0};

int width         = 640;
int height        = 640;
float rotationX 	= 0.0;
float rotationY 	= 0.0;
int   last_x, last_y;

void init(void)
{
    glClearColor (0.4, 0.4, 0.4, 0.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Define parametros da luz 0
    glLightfv(GL_LIGHT0, GL_AMBIENT, cor_luz_amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, cor_luz0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, cor_luz0);
    glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz0);

    printf("\nMenu");
    printf("\n====\n\n");
    printf("Keyboard:\n");
    printf("  's' altera modo de renderização.\n");
    printf("  'c' para mudar o objeto.\n");
    printf("  '+' e '-' para mudar a resolução do objeto renderizado.\n");
    printf("  Direcionais do teclado movem a luz selecionada em X e Y.\n");
    printf("  'PageUp' e 'PageDown' movem a luz selecionada em Z.\n");
    printf("Pressione ESC para sair.\n");
}

void display(void)
{
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(60.0, (GLfloat) width/(GLfloat) height, 1.0, 200.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity ();
    gluLookAt (0.0, 0.0, 30.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glPushMatrix();
        glTranslatef(posicao_luz0[0],posicao_luz0[1],posicao_luz0[2]);
        glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz0);
        glMaterialfv(GL_FRONT, GL_EMISSION, cor_luz_ativa);
        glutSolidSphere(0.1,100,100);
    glPopMatrix();

    (smooth) ? glShadeModel (GL_SMOOTH) : glShadeModel (GL_FLAT);

    glMaterialfv(GL_FRONT, GL_AMBIENT, object_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, object_difusa);
    glMaterialfv(GL_FRONT, GL_SPECULAR, object_especular);
    glMaterialfv(GL_FRONT, GL_EMISSION, object_emissao);
    glMaterialfv(GL_FRONT, GL_SHININESS, object_brilho);

    glPushMatrix();
        glRotatef( rotationY, 0.0, 1.0, 0.0 );
        glRotatef( rotationX, 1.0, 0.0, 0.0 );

        glRotatef(135.0f, 1.0f, 0.0f, 0.0f);
        (sphere) ? glutSolidSphere(10.0f, (float) resolution, (float) resolution) :
        glutSolidTorus(4.0, 10.0, (float) resolution, (float) resolution);
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

void special(int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_LEFT:
            posicao_luz0[0]-=0.2;
            break;
        case GLUT_KEY_RIGHT:
            posicao_luz0[0]+=0.2;
            break;
        case GLUT_KEY_UP:
            posicao_luz0[1]+=0.2;
            break;
        case GLUT_KEY_DOWN:
            posicao_luz0[1]-=0.2;
            break;
        case GLUT_KEY_PAGE_DOWN:
            posicao_luz0[2]-=0.2;
            break;
        case GLUT_KEY_PAGE_UP:
            posicao_luz0[2]+=0.2;
            break;
    }
    glutPostRedisplay();
}

void keyboard (unsigned char key, int x, int y)
{
    switch (tolower(key))
    {
        case 's' :
            smooth    = !smooth;
            break;
        case 'c' :
            sphere    = !sphere;
            break;
        case '+' :
            if (resolution<=100)
                resolution++;
            else
                printf("\nResolução Máxima atingida (100).\n");
            break;
        case '-' :
            if (resolution>=10)
                resolution--;
            else
                printf("\nResolução Mínima atingida (10).\n");
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
    glutCreateWindow ("Shading Example");
    init ();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc( mouse );
    glutMotionFunc( motion );
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
