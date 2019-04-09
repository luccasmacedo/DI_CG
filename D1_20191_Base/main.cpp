#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <list>
#include <string.h>
#include "extras.h"
#include <../shared/glcFPSViewer.h>
#include <vector>

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
int  last_x, last_y;
int  width, height;
int click = 0;

int indexGrupoAtual = 0;

vector<list<vertice>> grupos;
list<vertice> grupoAtual;

/// Functions
void init(void)
{
    initLight(width, height); // Função extra para tratar iluminação.
    setMaterials();
    glClearColor(0.0, 0.0, 0.0, 0.0);
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

void drawTriangle(vertice v1, vertice v2)
{
    vertice vetorNormal;
    vertice v[4] = {{v1.x, v1.y,  0.0f},
                    {v2.x, v2.y,  0.0f},
                    {v1.x, v1.y,  v1.z},
                    {v2.x, v2.y,  v2.z}};

    triangle t[2] = {{v[0], v[1], v[2]},
                     {v[1], v[3], v[2]}};

    glBegin(GL_TRIANGLES);
        for(int i = 0; i < 2; i++) // triangulos
        {
            CalculaNormal(t[i], &vetorNormal); // Passa face triangular e endereço do vetor normal de saída
            glNormal3f(vetorNormal.x, vetorNormal.y,vetorNormal.z);

            printf("normalX = %f \n",vetorNormal.x);
            printf("normalY = %f \n",vetorNormal.y);
            printf("normalZ = %f \n",vetorNormal.z);
            printf("\n \n");

            for(int j = 0; j < 3; j++) // vertices do triangulo
                glVertex3d(t[i].v[j].x, t[i].v[j].y, t[i].v[j].z);
        }
    glEnd();
}

void showInfoOnTitle(int group,float height){
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

void desenhaEixos2D(){
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f(-10.0, 0.0, 0.0);
        glVertex3f( 10.0, 0.0, 0.0);

        glColor3f(0.0, 1.0, 0.0);
        glVertex3f(0.0, -10.0, 0.0);
        glVertex3f(0.0,  10.0, 0.0);
    glEnd();
    glEnable(GL_LIGHTING);
}

void modela3D(){
    for(int i = 0; i < grupos.size(); i++){
        for(list<vertice>::iterator it = grupos[i].begin(); it != grupos[i].end(); it++){
            vertice v1;
            v1.x = it->x;
            v1.y = it->y;
            v1.z = it->z;

            it++;
            if(it != grupos[i].end()){
                vertice v2;
                v2.x = it->x;
                v2.y = it->y;
                v2.z = it->z;

                drawTriangle(v1,v2);
            }else{
                break;
            }
            it--;
        }
    }
}

void calculaCoordenadasPonto(float x, float y){
    float pointX = (-10 + x /(width/2) *20);
    float pointY = (10 - y/height * 20);

    printf("x = %f \n",x);
    printf("y = %f \n",y);

    printf("pointX = %f \n",pointX);
    printf("pointY = %f \n",pointY);
    printf("\n \n");

    //adiciona novo vertice ao grupo atual
    vertice *novo = new vertice();
    novo->x = pointX;
    novo->y = pointY;
    novo->z = alturaZPonto;
    grupos[indexGrupoAtual].push_back(*novo);

    printf("indexGrupoAtual: %d \n",indexGrupoAtual);
    printf("sizeGrupos: %d \n",grupos.size());
    printf("sizeList: %d \n",grupos[indexGrupoAtual].size());
    printf("\n \n");
}

void desenhaPonto(float x, float y){
    glPointSize(10.0);
    glEnable(GL_POINT_SMOOTH);
    glBegin(GL_POINTS);
        glColor3f(0.0, 0.0, 1.0);
        glVertex3f(x, y, 0.0);
    glEnd();
}

void display(void)
{
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /// Index grupo, alturaZPonto
    showInfoOnTitle(indexGrupoAtual,alturaZPonto);

    ///Desenha primeira ViewPort (2D)
    glViewport ((int) 0, (int) 0, (int) width/2, (int) height);

    glDisable(GL_DEPTH_TEST);
    //Define cor de fundo da primeira viewport
    glScissor((int) 0, (int) 0, (int) width/2, (int) height);
    glEnable(GL_SCISSOR_TEST);
    glClearColor(1.0, 1.0, 1.0, 1.0 );
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glOrtho(-10.0, 10.0, -10.0, 10.0, -1, 1);

    /*desenha eixos 2D*/
    desenhaEixos2D();


    /*desenha ponto 2D*/
    if(click == 1){
        for(int i = 0; i < grupos.size(); i++){
            for(list<vertice>::iterator it = grupos[i].begin(); it != grupos[i].end(); it++){
                desenhaPonto(it->x, it->y);
            }
        }
    }

    ///Desenha segunda ViewPort (3D)
    glViewport ((int)  width/2, (int) 0, (int)  width/2, (int) height);

    //Define cor de fundo da segunda viewport
    glScissor((int) width/2, (int) 0, (int) width/2, (int) height);
    glEnable(GL_SCISSOR_TEST);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(60.0, (GLfloat) (width/2)/(GLfloat) height, 1.0, 200.0);
    gluLookAt (0.0, 0.0, zdist, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);


    /*Objetos 3D*/
    glPushMatrix();
        glRotatef( rotationY, 0.0, 1.0, 0.0 );
        glRotatef( rotationX, 1.0, 0.0, 0.0 );
        //Desenha objeto 3D
        modela3D();
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
        click = 1;
        if(x < width/2)
            calculaCoordenadasPonto(x,y);
    }
    if ( button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN )
    {
        if(grupos[indexGrupoAtual].size() > 0){
            grupos[indexGrupoAtual].pop_back();
        }
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
            alturaZPonto++;
            break;
        case GLUT_KEY_DOWN:
            //Diminui altura do ponto criado
            if(alturaZPonto > 1)
                 alturaZPonto--;
            break;
        case GLUT_KEY_RIGHT:
            //Troca grupo e cria se necessário
            indexGrupoAtual++;
            if(indexGrupoAtual >= grupos.size()){
                list<vertice> grupo1;
                grupos.push_back(grupo1);
            }
            break;
        case GLUT_KEY_LEFT:
            //troca grupo
            if(indexGrupoAtual > 0){
                indexGrupoAtual--;
            }
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
