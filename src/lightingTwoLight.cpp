/*
  Name:        iluminacao_two_lights
  Copyright:   Version 0.1
  Author:      Rodrigo Luis de Souza da Silva
  Last Update:	18/12/2014
  Date:        23/06/2008
  Description: Codigo para simular as diferentes componentes de luz em um ambiente.
*/

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int width         = 800;
int height        = 800;

float rotationX = -30.0, rotationY = 0.0;
int   last_x, last_y;
float distOrigem = 0;

// Controlam se a componente estara ou nao visivel
bool difusa     = true;
bool ambiente   = true;
bool especular  = true;

// Guarda qual luz esta ativa e selecionada
int lightEnable[2] = {1,1};
int activeLight = 0;

// Material do object (material branco)
GLfloat object_ambient[]   = { 0.3, 0.3, 0.3, 1.0 };
GLfloat object_difusa[]    = { 1.0, 1.0, 1.0, 1.0 };
GLfloat object_especular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat object_emissao[] =   { 0.0, 0.0, 0.0, 0.0 };
GLfloat object_brilho[]    = { 128.0 };

// Define cor para zerar emissao e luz ambiente
GLfloat sem_cor[]          = { 0.0, 0.0, 0.0, 1.0};

// Defini cor da luz ambiente
GLfloat cor_luz_amb[]      = { 0.1, 0.1, 0.1, 1.0};

// Modifica a cor da luz que esta ativa
GLfloat cor_luz_ativa[]    = { 1.0, 0.0, 0.0, 1.0};

// Cor e posicao da luz azul
GLfloat posicao_luz0[]     = { 5.0, 5.0, 8.0, 1.0};
GLfloat cor_luz0[]         = { 0.0, 0.0, 0.8, 1.0};

// Cor e posicao da luz amarela
GLfloat posicao_luz1[]     = { -5.0, 5.0, 8.0, 1.0};
GLfloat cor_luz1[]         = { 0.8, 0.8, 0.0, 1.0};

void init(void)
{
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);               // Habilita Z-buffer
    glEnable(GL_LIGHTING);                 // Habilita luz
    glEnable(GL_LIGHT0);                   // habilita luz 0
    glEnable(GL_LIGHT1);                   // habilita luz 1

    // Define parametros da luz 0
    glLightfv(GL_LIGHT0, GL_AMBIENT, cor_luz_amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, cor_luz0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, cor_luz0);
    glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz0);

    // Define parametros da luz 1
    glLightfv(GL_LIGHT1, GL_AMBIENT, cor_luz_amb);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, cor_luz1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, cor_luz1);
    glLightfv(GL_LIGHT1, GL_POSITION, posicao_luz1);

    printf("\nMenu");
    printf("\n====\n\n");
    printf("Keyboard:\n");
    printf("  'd' para habilitar/desabilitar luz DIFUSA.\n");
    printf("  'e' para habilitar/desabilitar luz ESPECULAR.\n");
    printf("  'a' para habilitar/desabilitar luz AMBIENT.\n");
    printf("  '1' ou '2' para selecionar as fontes de luz.\n");
    printf("  'F1' ou 'F2' para ligar/desligar as fontes de luz.\n");
    printf("  Direcionais do teclado movem a luz selecionada em X e Y.\n");
    printf("  'PageUp' e 'PageDown' movem a luz selecionada em Z.\n");
    printf("  Use o botao esquerdo para rotacionar o objeto e scroll para aproximar/afastar.\n");
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
    gluLookAt (0.0, 0.0, 25.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // Define luz 0
    if(lightEnable[0])
    {
        // Posiciona esfera que representa a fonte de luz 0 no mundo
        glPushMatrix();
            glTranslatef(posicao_luz0[0],posicao_luz0[1],posicao_luz0[2]);
            glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz0);
            // Informa que a superficie abaixo sera de emissao (fonte de luz) e defini cor
            if(activeLight==0)
                glMaterialfv(GL_FRONT, GL_EMISSION, cor_luz_ativa);
            else
                glMaterialfv(GL_FRONT, GL_EMISSION, cor_luz0);
            glutSolidSphere(0.2,30,30);
        glPopMatrix();
    }

    // Define luz 1
    if(lightEnable[1])
    {
        glPushMatrix();
            glTranslatef(posicao_luz1[0],posicao_luz1[1],posicao_luz1[2]);
            glLightfv(GL_LIGHT1, GL_POSITION, posicao_luz1);
            if(activeLight==1)
                glMaterialfv(GL_FRONT, GL_EMISSION, cor_luz_ativa);
            else
                glMaterialfv(GL_FRONT, GL_EMISSION, cor_luz1);
            glutSolidSphere(0.2,30,30);
            glMaterialfv(GL_FRONT, GL_EMISSION, sem_cor);
        glPopMatrix();
    }

    // Define os parametros da superficie a ser iluminada
    if(ambiente)
        glMaterialfv(GL_FRONT, GL_AMBIENT, object_ambient);
    else
        glMaterialfv(GL_FRONT, GL_AMBIENT,  sem_cor);

    if(difusa)
        glMaterialfv(GL_FRONT, GL_DIFFUSE, object_difusa);
    else
        glMaterialfv(GL_FRONT, GL_DIFFUSE,  sem_cor);

    if(especular)
        glMaterialfv(GL_FRONT, GL_SPECULAR, object_especular);
    else
        glMaterialfv(GL_FRONT, GL_SPECULAR, sem_cor);

    glMaterialfv(GL_FRONT, GL_EMISSION, object_emissao);
    glMaterialfv(GL_FRONT, GL_SHININESS, object_brilho);

    glPushMatrix();
        glTranslatef(0.0, 0.0, distOrigem);
        glRotatef( rotationY, 0.0, 1.0, 0.0 );
        glRotatef( rotationX, 1.0, 0.0, 0.0 );
        glutSolidTorus(2.0, 8.0, 100, 100);
        //glutSolidTeapot(5.0);
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
            (!activeLight)? posicao_luz0[0]-=0.2 : posicao_luz1[0]-=0.2;
            break;
        case GLUT_KEY_RIGHT:
            (!activeLight)? posicao_luz0[0]+=0.2 : posicao_luz1[0]+=0.2;
            break;
        case GLUT_KEY_UP:
            (!activeLight)? posicao_luz0[1]+=0.2 : posicao_luz1[1]+=0.2;
            break;
        case GLUT_KEY_DOWN:
            (!activeLight)? posicao_luz0[1]-=0.2 : posicao_luz1[1]-=0.2;
            break;
        case GLUT_KEY_PAGE_DOWN:
            (!activeLight)? posicao_luz0[2]-=0.2 : posicao_luz1[2]-=0.2;
            break;
        case GLUT_KEY_PAGE_UP:
            (!activeLight)? posicao_luz0[2]+=0.2 : posicao_luz1[2]+=0.2;
            break;
        case GLUT_KEY_F1:
            (lightEnable[0]) ? glDisable(GL_LIGHT0) : glEnable(GL_LIGHT0);
            lightEnable[0] = !lightEnable[0];
            break;
        case GLUT_KEY_F2:
            (lightEnable[1]) ? glDisable(GL_LIGHT1) : glEnable(GL_LIGHT1);
            lightEnable[1] = !lightEnable[1];
            break;
    }
    glutPostRedisplay();
}

void keyboard (unsigned char key, int x, int y)
{
    switch (tolower(key))
    {
        case 'd' :
            difusa    = !difusa;
            break;
        case 'e' :
            especular = !especular;
            break;
        case 'a' :
            ambiente  = !ambiente;
            break;
        case '1' :
            activeLight = 0;
            break;
        case '2' :
            activeLight = 1;
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
    if(button == 3) // Scroll up
    {
        distOrigem+=0.3;
        if(distOrigem>180) distOrigem=180;
    }
    if(button == 4) // Scroll Down
    {
        distOrigem-=0.3;
        if(distOrigem<-20) distOrigem=-20;
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
    glutKeyboardFunc(keyboard);
    glutMouseFunc( mouse );
    glutMotionFunc( motion );
    glutSpecialFunc(special);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
