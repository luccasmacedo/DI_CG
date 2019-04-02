/*
  Name:        polygonTwoSide.cpp
  Author:      Rodrigo Luis de Souza da Silva
  Last Update: 03/06/2016
  Date:        03/06/2016
  Description:
    Código para visualização de uma pequena malha poligonal com renderização da face frontal
    e traseira com materias diferentes.
    Os principais cuidados que devemos ter para visualizar os dois lados de uma superfície
    são habilitar o comando glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE) e não habilitar
    o backface culling (CULL_FACE).
*/

#ifdef WIN32
#include <windows.h>
#endif
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>

typedef struct // Estrutura para armazenar vertices
{
    double x,y,z;
} vertice;

// Coordenadas base do Icosaedro
float X = 0.525;
float Z = 0.850;
float rotationX = 0.0, rotationY = 0.0;
int   last_x, last_y;

// Functions
void init(void)
{

    glClearColor (0.1, 0.1, 0.1, 0.0);
    glShadeModel (GL_SMOOTH);

    // É importante lembrar aqui de não habilitar o Backface Culling
    glEnable(GL_LIGHT0);                  // habilita luz 0
    glEnable(GL_LIGHTING);                // Habilita luz
    glEnable(GL_DEPTH_TEST);              // Habilita Z-buffer
    glEnable(GL_NORMALIZE);               // Normalização dos vetores normais

    // Cor da fonte de luz (RGBA)
    GLfloat cor_luz[]     = { 1.0, 1.0, 1.0, 1.0};
    // Posicao da fonte de luz. Ultimo parametro define se a luz sera direcional (0.0) ou tera uma posicional (1.0)
    GLfloat posicao_luz[] = { 50.0, 50.0, 50.0, 1.0};

    // Define parametros básicos da luz
    glLightfv(GL_LIGHT0, GL_AMBIENT, cor_luz);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, cor_luz);
    glLightfv(GL_LIGHT0, GL_SPECULAR, cor_luz);
    glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz);

    // Quando a opção "two_side lighting" está ativada, o opengl inverte a orientação das normais
    // permitindo que tanto as faces externas quanto internas sejam iluminadas.
    // Essa opção é mais lenta se comparada a renderização de apenas um dos lados
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
}

void setMaterials(void)
{
    // Parametros comuns para os dois lados da superfície
    GLfloat objeto_especular[] = { 0.626, 0.626, 0.626, 1.0 };
    GLfloat objeto_brilho[]    = { 90.0f };

    // Material da superfície externa
    GLfloat objeto_ambient[]   = { 0.1745, 0.01175, 0.011, 1.0 };
    GLfloat objeto_difusa[]    = { 0.6142, 0.04136, 0.041, 1.0 };

    glMaterialfv(GL_FRONT, GL_AMBIENT, objeto_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, objeto_difusa);
    glMaterialfv(GL_FRONT, GL_SPECULAR, objeto_especular);
    glMaterialfv(GL_FRONT, GL_SHININESS, objeto_brilho);

    // Material das faces internas (amarelo)
    GLfloat objeto_ambient_back[]   = { 0.1745, 0.174, 0.011, 1.0 };
    GLfloat objeto_difusa_back[]    = { 0.6144, 0.614, 0.041, 1.0 };

    glMaterialfv(GL_BACK, GL_AMBIENT, objeto_ambient_back);
    glMaterialfv(GL_BACK, GL_DIFFUSE, objeto_difusa_back);
    glMaterialfv(GL_BACK, GL_SPECULAR, objeto_especular);
    glMaterialfv(GL_BACK, GL_SHININESS, objeto_brilho);
}

// A função abaixo utiliza o produto vetorial de 2 vetores
// que são definidos a partir dos vértices do triângulo
void CalculaNormal(vertice * vert_temp, vertice *vn) // Vertice e numero de vertices face
{
    vertice v1, v2;
    double len;

    /* Encontra vetor v1 */
    v1.x = vert_temp[1].x - vert_temp[0].x;
    v1.y = vert_temp[1].y - vert_temp[0].y;
    v1.z = vert_temp[1].z - vert_temp[0].z;

    /* Encontra vetor v2 */
    v2.x = vert_temp[2].x - vert_temp[0].x;
    v2.y = vert_temp[2].y - vert_temp[0].y;
    v2.z = vert_temp[2].z - vert_temp[0].z;

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
    // ESTRUTURA: vertice face [numero de faces] [numero de vertices + 1]
    vertice vetorNormal;
    vertice object[8][3] = {{{0,Z,X},{-X,0,Z},{X,0,Z}},    //0
        {{-Z,X,0},{-X,0,Z},{0,Z,X}},   //1
        {{0,Z,-X},{-Z,X,0},{0,Z,X}},   //2
        {{0,Z,-X},{0,Z,X},{Z,X,0}},    //3
        {{Z,X,0},{0,Z,X},{X,0,Z}},	  //4
        {{Z,-X,0},{Z,X,0},{X,0,Z}},	  //5
        {{X,0,-Z},{Z,X,0},{Z,-X,0}},	  //6
        {{X,0,-Z},{0,Z,-X},{Z,X,0}}
    };  // 7

    setMaterials();
    for(int j = 0; j < 8; j++)
    {
        glBegin(GL_TRIANGLES);
        CalculaNormal(object[j], &vetorNormal); // Passa face triangular e endereço do vertice
        glNormal3d(vetorNormal.x, vetorNormal.y,vetorNormal.z);
        for(int v = 0; v < 3; v++) // Vertices de cada face
            glVertex3d(object[j][v].x,object[j][v].y,object[j][v].z);
        glEnd();
    }
}

void display(void)
{
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Posicao da luz
    float pos[4] = {10.0, 10.0, 10.0, 1.0};

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt (0.0, 0.0, 4.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
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

    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(45.0, (GLfloat) w/(GLfloat) h, 0.01, 200.0);
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
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (600, 600);
    glutInitWindowPosition (100, 100);
    glutCreateWindow (argv[0]);
    init ();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc( mouse );
    glutMotionFunc( motion );
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
