#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <list>
#include <string.h>
#include <vector>
#include <fstream>


using namespace std;

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

///Variáveis globais:
float zdist = 30.0;
float rotationX = 0.0, rotationY = 0.0;
int  last_x, last_y;
int  width, height;
bool fullSreen = false;

//Qual estrutura vai armazenar os conjuntos de vertices?

/// Functions
//Funcoes para setar iluminação e materiais
void initLight(int width, int height);

void setMaterials(void);

void init(void)
{
//    initLight(width, height); // Função extra para tratar iluminação.
//    setMaterials();
}

//Função Draw Triangle para desenhar somente um triangulo:
void drawTriangle(vertice v1, vertice v2, vertice v3);

void showInfoOnTitle(int group,float height)
{
    static char title[256] = {0};
    char aux1[32];
    sprintf(aux1,"Grupo atual: %.1d, ",group);
    char aux2[32];
    sprintf(aux2,"Altura do ponto: %.1f",height);

    strcpy(title, "Modelagem 2D-3D, " );
    strcat(title,aux1);
    strcat(title,aux2);
    strcat(title," - Pressione ESC para sair");

    glutSetWindowTitle(title);
}

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

//Função que le os arquivos ply para desenha-los
void readPlyFiles();

void display();

void idle ()
{
    glutPostRedisplay();
}

void reshape (int w, int h)
{
    width = w;
    height = h;
}

void keyboard (unsigned char key, int x, int y);

void motion(int x, int y );

void mouse(int button, int state, int x, int y);

void specialKeysPress(int key, int x, int y);

void exibeMenu();

int main(int argc, char** argv)
{
    exibeMenu();
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
