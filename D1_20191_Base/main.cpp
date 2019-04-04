#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <list>
#include "extras.h"
#include <../shared/glcFPSViewer.h>

using namespace std;

/// Estruturas iniciais para armazenar vertices
//  Você poderá utilizá-las adicionando novos métodos (de acesso por exemplo) ou usar suas próprias estruturas.
class vertice
{
    public:
        float x,y,z;
};

class triangle
{
    public:
        vertice v[3];
};

/// Globals
float zdist = 5.0;
//Coordenada z do ponto criado
float alturaZPonto = 1.0;
float rotationX = 0.0, rotationY = 0.0;
int   last_x, last_y;
int   width, height;
//Mostra informaçoes na barra de titulo
glcFPSViewer *fpsViewer = new glcFPSViewer((char*) "Modelagem 2D-3D. ", (char*) " - Press ESC to Exit");//Lista de listas de vertices

std::list<list<vertice>> grupos;
list<vertice> grupoAtual;

/// Functions
void init(void)
{
    initLight(width, height); // Função extra para tratar iluminação.
    setMaterials();
}

/* Exemplo de cálculo de vetor normal que são definidos a partir dos vértices do triângulo;
  v_2
  ^
  |\
  | \
  |  \       'vn' é o vetor normal resultante
  |   \
  +----> v_1
  v_0
*/
void CalculaNormal(triangle t, vertice *vn)
{
    vertice v_0 = t.v[0],
            v_1 = t.v[1],
            v_2 = t.v[2];
    vertice v1, v2;
    double len;

    /* Encontra vetor v1 */
    v1.x = v_1.x - v_0.x;
    v1.y = v_1.y - v_0.y;
    v1.z = v_1.z - v_0.z;

    /* Encontra vetor v2 */
    v2.x = v_2.x - v_0.x;
    v2.y = v_2.y - v_0.y;
    v2.z = v_2.z - v_0.z;

    /* Calculo do produto vetorial de v1 e v2 */
    vn->x = (v1.y * v2.z) - (v1.z * v2.y);
    vn->y = (v1.z * v2.x) - (v1.x * v2.z);
    vn->z = (v1.x * v2.y) - (v1.y * v2.x);

    /* normalizacao de n */
    len = sqrt(pow(vn->x,2) + pow(vn->y,2) + pow(vn->z,2));

    vn->x /= len;
    vn->y /= len;
    vn->z /= len;
}

void drawObject()
{
    vertice vetorNormal;
    vertice v[4] = {{-1.0f, -1.0f,  0.0f},
                    { 1.0f, -1.0f,  0.0f},
                    {-1.0f,  1.0f,  0.0f},
                    { 1.0f,  1.0f, -0.5f}};

    triangle t[2] = {{v[0], v[1], v[2]},
                     {v[1], v[3], v[2]}};

    glBegin(GL_TRIANGLES);
        for(int i = 0; i < 2; i++) // triangulos
        {
            CalculaNormal(t[i], &vetorNormal); // Passa face triangular e endereço do vetor normal de saída
            glNormal3f(vetorNormal.x, vetorNormal.y,vetorNormal.z);
            for(int j = 0; j < 3; j++) // vertices do triangulo
                glVertex3d(t[i].v[j].x, t[i].v[j].y, t[i].v[j].z);
        }
    glEnd();
}

void DesenhaEixos2D(){
    glBegin(GL_LINES);
        glColor3f(0.0, 0.0, 1.0);
        glVertex3f(-10.0, 0.0, 0.0);
        glVertex3f( 10.0, 0.0, 0.0);

        glVertex3f(0.0, -10.0, 0.0);
        glVertex3f(0.0,  10.0, 0.0);
    glEnd();
}

void display(void)
{
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ///Desenha primeira ViewPort
    glViewport ((int) 0, (int) 0, (int) width/2, (int) height);

    //Define cor de fundo da primeira viewport
    glScissor((int) 0, (int) 0, (int) width/2, (int) height);
    glEnable(GL_SCISSOR_TEST);
    glClearColor(1.0, 1.0, 1.0, 1.0 );
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glOrtho(-10.0, 10.0, -10.0, 10.0, -1, 1);

    /*desenha eixos 2D*/
    DesenhaEixos2D();

    ///Desenha segunda ViewPort
    glViewport ((int)  width/2, (int) 0, (int)  width/2, (int) height);

    //Define cor de fundo da segunda viewport
    glScissor((int) width/2, (int) 0, (int) width/2, (int) height);
    glEnable(GL_SCISSOR_TEST);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(60.0, (GLfloat) width/(GLfloat) height, 1.0, 200.0);
    gluLookAt (0.0, 0.0, zdist, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    /*desenha Objetos 3D*/
    glPushMatrix();
        glRotatef( rotationY, 0.0, 1.0, 0.0 );
        glRotatef( rotationX, 1.0, 0.0, 0.0 );
        drawObject();
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
}

void keyboard (unsigned char key, int x, int y)
{

    switch (tolower(key))
    {
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

        //adiciona novo vertice ao grupo atual
        vertice *novo = new vertice();
        novo->x = x;
        novo->y = y;
        novo->z = alturaZPonto;
        grupoAtual.push_back(*novo);
    }
    if ( button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN )
    {
        //Exclui ultimo ponto criado no grupo atual
    }
    if(button == 3) // Scroll up
    {
        zdist+=1.0f;
    }
    if(button == 4) // Scroll Down
    {
        zdist-=1.0f;
    }
}

void specialKeysPress(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_UP:
            //Aumenta altura do ponto criado
            break;
        case GLUT_KEY_DOWN:
            //Diminui altura do ponto criado
            break;
        case GLUT_KEY_RIGHT:
            //Troca grupo e cria se necessário
            break;
        case GLUT_KEY_LEFT:
            //troca grupo
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

/// Main
int main(int argc, char** argv)
{
    //Definição do primeiro grupo(padrao)
    list<vertice> grupo1;
    grupoAtual = grupo1;
    grupos.push_back(grupo1);

    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (800, 600);
    glutInitWindowPosition (100, 100);
    glutCreateWindow (argv[0]);
    init ();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc( mouse );
    glutMotionFunc( motion );
    glutSpecialFunc(specialKeysPress);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
