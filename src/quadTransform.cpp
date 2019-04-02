/*
  Name:        quad_transform.cpp
  Copyright:   Version 0.1
  Author:      Rodrigo Luis de Souza da Silva
  Date:        16/09/2004
  Description: Transformations using of OpenGL commands
*/

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

float angle = 0, scale = 1.0;
float xtrans = 0, ytrans = 0, ztrans = 0;
int enableMenu = 0;

void display(void);
void init (void);
void desenhaTabuleiro();
void showMenu();
void mouse(int button, int state, int x, int y);
void keyboardPress(unsigned char key, int x, int y);
void specialKeys(int key, int x, int y);

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE|GLUT_DEPTH|GLUT_RGB);
    glutInitWindowSize (600, 600);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("hello");
    //glutMouseFunc( mouse );
    //glutKeyboardFunc(keyboardPress);
    //glutSpecialFunc(specialKeys);
    init ();
    printf("Posicione as janelas e clique na janela do OpenGL para habilitar o menu.\n");
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}

//KeyboardPress
void keyboardPress(unsigned char key, int x, int y){
    switch(key){
        case '+':
            scale += 0.2;
            break;
        case '-':
            scale -= 0.2;
            break;
        case 'r':
            angle += 30;
            break;
        case 'R':
            angle += -30;
            break;

    }
}

//Special keys pressed
/*
void specialKeys(int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_LEFT:
            xtrans += ;
            break;
        case GLUT_KEY_RIGHT :
            xtrans += ;
            break;
        case GLUT_KEY_UP :
            ytrans += ;
            break;
        case GLUT_KEY_DOWN :
            ytrans += ;
            break;
    }
    glutPostRedisplay();
}
*/
// Mouse callback
void mouse(int button, int state, int x, int y)
{
    if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        enableMenu = 1;
}

void showMenu()
{
    int op;
    system("cls");
    printf("\n=== MENU ===");
    printf("\n1 - Translacao");
    printf("\n2 - Rotacao");
    printf("\n3 - Escala");
    printf("\n4 - Sair");
    printf("\nOpcao: ");

    scanf("%d", &op);
    switch(op)
    {
        case 1:
            printf("\n\nInforme o vetor de translacao (entre -100.0 e 100.0)");
            printf("\nX : ");
            scanf("%f", &xtrans);
            printf("Y : ");
            scanf("%f", &ytrans);
            break;
        case 2:
            printf("\n\nInforme o angulo de rotacao (em graus): ");
            scanf("%f", &angle);
            break;
        case 3:
            printf("\n\nInforme o fator de escala: ");
            scanf("%f", &scale);
            break;
        case 4:
            exit(1);
            break;
        default:
            printf("\n\nOpcao invalida\n\n");
            break;
    }
}

void desenhaTabuleiro()
{
    glPushMatrix();
    xtrans = -2.5;
    ytrans = -2.5;
    glTranslatef(xtrans, ytrans, 0.0);
    glRotatef(angle, 0.0, 0.0, 1.0);
    glScalef(scale, scale, scale);
    glutSolidCube(1);
    glPopMatrix();

    while(xtrans <= 3){
        glTranslatef(xtrans, ytrans, 0.0);
        glRotatef(angle, 0.0, 0.0, 1.0);
        glScalef(scale, scale, scale);
        glutSolidCube(1);
        glPopMatrix();
        xtrans +=2;
    }
}

void display(void)
{
    // Limpar todos os pixels
    glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glLoadIdentity(); // Inicializa com matriz identidade

    desenhaTabuleiro();

    glColor3f (1.0, 1.0, 1.0);

    glPushMatrix();
    glTranslatef(xtrans, ytrans, 0.0);
    glRotatef(angle, 0.0, 0.0, 1.0);
    glScalef(scale, scale, scale);
    glutWireCube(10);
    glPopMatrix();

    glutSwapBuffers ();
    glutPostRedisplay();

    if(enableMenu)
        showMenu();
}

void init (void)
{
    // selecionar cor de fundo (preto)
    glClearColor (0.0, 0.0, 0.0, 0.0);

    // inicializar sistema de viz.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-3, 3.0, -3.0, 3.0, -3.0, 3.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
