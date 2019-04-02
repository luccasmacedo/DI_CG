/*
  Name:        mousekeyboard.cpp
  Author:      Rodrigo Luis de Souza da Silva
  Release:     30/10/2013
  Updates:	   18/12/2014 - Scroll up and down added
               10/04/2014 - Passive motion added
  Description: Simple OpenGL program that captures mouse and keyboard
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

int windowSizeX = 640;
int windowSizeY = 640;
float objectPosX = windowSizeX/2;
float objectPosY = windowSizeY/2;

int objectRadius = 50;

int speed = 3;
int moveX = 0;
int moveY = 0;
bool passiveMotion = false;

// Special Keys callback
void specialKeysPress(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_UP:
            moveY = speed;
            break;
        case GLUT_KEY_DOWN:
            moveY = -speed;
            break;
        case GLUT_KEY_RIGHT:
            moveX = speed;
            break;
        case GLUT_KEY_LEFT:
            moveX = -speed;
            break;
        default:
            printf("\nPressionou outra tecla especial não mapeada!");
            break;
    }
    glutPostRedisplay();
}

// Special Keys callback
void specialKeysRelease(int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_LEFT :
        case GLUT_KEY_RIGHT :
            moveX = 0;
            break;
        case GLUT_KEY_UP :
        case GLUT_KEY_DOWN :
            moveY = 0;
            break;
    }
    glutPostRedisplay();
}


// Motion callback
void motion(int x, int y )
{
    // Inverte mouse para que origem fique no canto inferior esquerdo da janela
    // (por default, a origem é no canto superior esquerdo)
    y = windowSizeY - y;

    objectPosX = x;
    objectPosY = y;
}

// Motion callback
void noMotion(int x, int y )
{
    // Cancel passive motion

}

// Mouse callback
void mouse(int button, int state, int x, int y)
{
    // Inverte mouse para que origem fique no canto inferior esquerdo da janela
    // (por default, a origem é no canto superior esquerdo)
    y = windowSizeY - y;

    if ( button == GLUT_LEFT_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            printf("\nBotao esquerdo pressionado na posicao [%d, %d].", x, y);
            objectPosX = x;
            objectPosY = y;
        }
        else
            printf("\nBotao esquerdo solto na posicao [%d, %d].", x, y);  // GLUT_UP
    }
    if ( button == GLUT_RIGHT_BUTTON)
    {
        printf("\nBotao direito do mouse pressionado.");
    }
    if(button == 3) // Scroll up
    {
        printf("\nScroll up.");
    }
    if(button == 4) // Scroll Down
    {
        printf("\nScroll down.");
    }
}


// Keyboard callback
void keyboardPress(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27:
            exit(0);
            break;
        case '+':
            if(objectRadius<=100)
            {
                printf("\nAumentando raio da esfera (%d)", objectRadius);
                objectRadius++;
            }
            else
                printf("\nRaio máximo atingido (%d)", objectRadius);
            break;
        case '-':
            if(objectRadius>10)
            {
                printf("\nDiminuindo raio da esfera (%d)", objectRadius);
                objectRadius--;
            }
            else
                printf("\nRaio mínimo atingido (%d)", objectRadius);
            break;
        case 'w':
            moveY = speed;
            break;
        case 's':
            moveY = -speed;
            break;
        case 'd':
            moveX = speed;
            break;
        case 'a':
            moveX = -speed;
            break;
        case 'm':
            passiveMotion = !passiveMotion;

            if(passiveMotion)
            {
                printf("\n\"Passive Motion LIGADO");
                glutPassiveMotionFunc( motion );
            }
            else
            {
                printf("\n\"Passive Motion DESLIGADO");
                glutPassiveMotionFunc( NULL );
            }
            break;
        default:
            printf("\nUma tecla não mapeada foi pressionada");
            break;
    }
    glutPostRedisplay();
}

// Keyboard callback
void keyboardRelease(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'w' :
        case 's' :
            moveY = 0;
            break;
        case 'd' :
        case 'a' :
            moveX = 0;
            break;
    }
    glutPostRedisplay();
}

void display(void)
{
    glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // Clean z-buffer

    // Change object position if some key is pressed
    objectPosX+=moveX;
    objectPosY+=moveY;

    glPushMatrix();
    glColor3f(1.0f, 1.0f, 0.0f);
    glTranslatef(objectPosX, objectPosY, 0.0f);
    glutWireSphere(objectRadius, 10, 10);
    glPopMatrix();

    glutSwapBuffers ();  // Required when double buffer is initialized
    glutPostRedisplay();
}


void init (void)
{
    glClearColor (0.0, 0.0, 0.0, 0.0);

    // inicializar sistema de viz.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, windowSizeX, 0.0, windowSizeY, -500.0, 500.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Main Function
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (windowSizeX, windowSizeY);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("hello");
    init ();

    // Modifique aqui para '0' para aumentar o delay
    glutIgnoreKeyRepeat(1);
    glutMouseFunc( mouse );

    glutKeyboardFunc( keyboardPress );
    glutKeyboardUpFunc( keyboardRelease );

    glutSpecialFunc( specialKeysPress );
    glutSpecialUpFunc( specialKeysRelease );

    glutDisplayFunc(display);
    glutMotionFunc( motion );
    glutPassiveMotionFunc( NULL );

    printf("Programa para teste das callbacks de mouse e teclado do glut.\n\n");
    printf("* Pressione as teclas '+' e '-' para aumentar e diminuir o raio da esfera.\n");
    printf("* Pressione as setas para mover o objeto na direcao pressionada.\n");
    printf("* Mova o objeto tambem com as teclas 'w', 's', 'a', 'd'.\n");
    printf("* Com o mouse, clique na tela para alterar a posicao do objeto.\n");
    printf("* Clique e arraste para mover o objeto.\n");
    printf("* Pressione 'm' para que o objeto mova sem o clique do mouse (passive motion).\n");
    printf("* Pressione ESC para sair!\n\n");
    printf("-------------------------------------------------------------------------\n");

    glutMainLoop();

    return 0;
}
