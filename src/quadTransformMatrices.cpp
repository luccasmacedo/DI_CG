/*
  Name:        quad_transform_matrices.cpp
  Copyright:   Version 0.1
  Author:      Rodrigo Luis de Souza da Silva
  Date:        16/09/2004
  Description: Transformations using matrices instead of OpenGL commands
*/

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float angle = 0, scale = 1.0;
float xtrans = 0, ytrans = 0, ztrans = 0;
int enableMenu = 0;

void display(void);
void init (void);
void desenhaEixos();
void showMenu();
void mouse(int button, int state, int x, int y);

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE|GLUT_DEPTH|GLUT_RGB);
   glutInitWindowSize (300, 300);
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("hello");
   glutMouseFunc( mouse );
   init ();
   printf("Posicione as janelas e clique na janela do OpenGL para habilitar o menu.\n");
   glutDisplayFunc(display);
   glutMainLoop();

   return 0;
}

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

void desenhaEixos()
{
   glColor3f (0.0, 1.0, 0.0);
   glBegin(GL_LINES);
      glVertex2f (0.0, -100.0);
      glVertex2f (0.0, 100.0);
      glVertex2f (-100.0, 0.0);
      glVertex2f (100.0, 0.0);
   glEnd();
}

void localTranslate()
{
   /* Translation Matrix
   1   0   0   0
   0   1   0   0
   0   0   1   0
   dx  dy  dz  1};
   */

   float translateMatrix[16];

   // Load Identity
   for(int m = 0, i = 0; i < 4; i++)
      for(int j = 0; j < 4; j++, m++)
         translateMatrix[m] = (i==j) ? 1.0f : 0.0f;

   // Setting translation parameters
   translateMatrix[12] = xtrans; // dx
   translateMatrix[13] = ytrans; // dy
   translateMatrix[14] = 0.0f;   // dy

   glMultMatrixf(translateMatrix);
}

void localRotate()
{
   /* Rotation Matrix in Z Axis
   cos(a)  sen(a)   0      0
   -sen(a)  cos(a)   0      0
     0       0      1      0
     0       0      0      1
   */

   float rotMatrix[16];
   float rad = angle * 3.1415 / 180; // Degree to rad

   // Load Identity
   for(int m = 0, i = 0; i < 4; i++)
      for(int j = 0; j < 4; j++, m++)
         rotMatrix[m] = (i==j) ? 1.0f : 0.0f;

   // Setting Rotation Parameters
   rotMatrix[0] =  cos(rad);       rotMatrix[1] = sin(rad);
   rotMatrix[4] = -sin(rad);       rotMatrix[5] = cos(rad);

   glMultMatrixf(rotMatrix);
}


void localScale()
{
   /* Scale Matrix
   ex  0   0   0
   0   ey  0   0
   0   0   ez  0
   0   0   0   1
   */

   float scaleMatrix[16];

   // Load Scale Matrix
   for(int m = 0, i = 0; i < 4; i++)
      for(int j = 0; j < 4; j++, m++)
         scaleMatrix[m] = (i==j) ? scale : 0.0f;

   // Fix Last Parameter
   scaleMatrix[15] = 1;

   glMultMatrixf(scaleMatrix);
}

void display(void)
{
   // Limpar todos os pixels
   glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   glLoadIdentity();	// Inicializa com matriz identidade

   desenhaEixos();

   glColor3f (1.0, 0.0, 0.0);

   glPushMatrix();
      localTranslate();  // glTranslatef(xtrans, ytrans, 0.0);
      localRotate();     // glRotatef(angle, 0.0, 0.0, 1.0);
      localScale();      // glScalef(scale, scale, scale);
      glutWireCube(10);
   glPopMatrix();

   glutSwapBuffers ();
   glutPostRedisplay();

   if(enableMenu) showMenu();
}

void init (void)
{
   // selecionar cor de fundo (preto)
   glClearColor (0.0, 0.0, 0.0, 0.0);

   // inicializar sistema de viz.
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-100.0, 100.0, -100.0, 100.0, -100.0, 100.0);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}
