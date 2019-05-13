#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <list>
#include <string.h>
#include "extras.h"
#include <vector>
#include <fstream>
#define VECTOR_SIZE 6

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

class polygon
{
public:
    vector<int> vertices;
};

class figure
{
public:
    vector<vertice> vertex;
    vector<polygon> faces;
    float matriz [3][4];
    vertice medio;
};

/// Globals
float zdist = 30.0;
float rotationX = 25.0, rotationY = 0.0;
int  last_x, last_y;
int  width = 800, height = 600;
bool fullSreen = false;
int indexGrupoAtual = 0;
bool wireframe = false;
bool superficie = true;

float matrizIluminacao[6][3][4] =
{
    {
        {.1745, .01175, .01175, 1.0},
        {.61424, .04136, .04136, 1.0},
        {.727811, .626959, .626959, 1.0}
    },
    {
        {.0518, .09175, .01175, 1.0 },
        {.5424, .0836, .04136, 1.0 },
        {.17811, .99959, .626959, 1.0}
    },
    {
        {0.2f, 0.2f, 0.2f, 1.0f},
        {0.7f, 0.7f, 0.7f, 1.0f},
        {0.1f, 0.1f, 0.1f, 1.0f}
    },
    {
        {1.0f, 0.05f, 1.0f, 1.0f},
        {1.0f, 0.0f, 0.8787f, 1.0f},
        {0.0044f, 0.970f, 0.789f, 1.0f}
    },
    {
        {0.1515f, 0.21848f, 0.3887f, 1.0f},
        {0.387f, 0.898f, 0.7488f, 1.0f},
        {0.0018f, 0.0489f, 0.7850f, 1.0f}
    },
    {
        {0.17f, 0.01f, 0.01f, 0.55f},
        {0.61f, 0.04f, 0.04f, 0.55f},
        {0.73f, 0.63f, 0.63f, 0.55f}
    }
};

string fileName[VECTOR_SIZE] = {"budda.ply",
                                "bunny.ply",
                                "cow.ply",
                                "dragon.ply",
                                "dragon_full.ply",
                                "snowman.ply"
                               };

///ESTRUTURA QUE ARMAZENA AS INFORMAÇOES DOS ARQUIVOS
vector<figure> arquivos;

/// Functions

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

//FUNÇÃO QUE DESENHA OS TRIANGULOS, UM POR VEZ
void drawTriangle(vertice v1, vertice v2, vertice v3)
{
    //glTranslatef(-arquivos[indexGrupoAtual].medio.x, -arquivos[indexGrupoAtual].medio.y, -arquivos[indexGrupoAtual].medio.z);

    vertice vetorNormal;
    triangle t = {v1,v2,v3};

    glBegin(GL_TRIANGLES);

    CalculaNormal(t, &vetorNormal); // Passa face triangular e endereço do vetor normal de saída
    glNormal3f(vetorNormal.x, vetorNormal.y,vetorNormal.z);
    for(int j = 0; j < 3; j++) // vertices do triangulo
        glVertex3d(t.v[j].x, t.v[j].y, t.v[j].z);
    glEnd();
}

void drawTriangleWireframe(vertice v1, vertice v2, vertice v3)
{
    //glTranslatef(-arquivos[indexGrupoAtual].medio.x, -arquivos[indexGrupoAtual].medio.y, -arquivos[indexGrupoAtual].medio.z);
    glBegin(GL_LINE_LOOP);
    glVertex3f(v1.x, v1.y, v1.z);
    glVertex3f(v2.x, v2.y, v2.z);
    glVertex3f(v3.x, v3.y, v3.z);
    glEnd();
}
void showInfoOnTitle()
{
    static char title[256] = {0};
    char aux1[32];
    sprintf(aux1,"Figura atual: %.1d, ",indexGrupoAtual);
    char aux2[32];
    sprintf(aux2,"Quantida de poligonos: %.1d",arquivos[indexGrupoAtual].faces.size());

    strcpy(title, "Visualizador de Arquivos Ply, " );
    strcat(title,aux1);
    strcat(title,aux2);
    strcat(title," - Pressione ESC para sair");

    glutSetWindowTitle(title);
}

void SetMaterial(float arrayAmbiente[], float arrayDifusa[], float arrayEspecular[]);

void modelaObjeto()
{
    SetMaterial(arquivos[indexGrupoAtual].matriz[0], arquivos[indexGrupoAtual].matriz[1], arquivos[indexGrupoAtual].matriz[2] );
    for(int i = 0; i < arquivos[indexGrupoAtual].faces.size(); i++)
    {

        if(arquivos[indexGrupoAtual].faces[i].vertices.size() == 3)
        {
            int index[3];
            index[0] = arquivos[indexGrupoAtual].faces[i].vertices[0];
            index[1] = arquivos[indexGrupoAtual].faces[i].vertices[1];
            index[2] = arquivos[indexGrupoAtual].faces[i].vertices[2];

            if(superficie)
            {
                drawTriangle(arquivos[indexGrupoAtual].vertex[index[0]],
                             arquivos[indexGrupoAtual].vertex[index[1]],
                             arquivos[indexGrupoAtual].vertex[index[2]]);
            }
            else
            {
                drawTriangleWireframe(arquivos[indexGrupoAtual].vertex[index[0]],
                                      arquivos[indexGrupoAtual].vertex[index[1]],
                                      arquivos[indexGrupoAtual].vertex[index[2]]);
            }
        }
        else
        {
            int index[4];
            index[0] = arquivos[indexGrupoAtual].faces[i].vertices[0];
            index[1] = arquivos[indexGrupoAtual].faces[i].vertices[1];
            index[2] = arquivos[indexGrupoAtual].faces[i].vertices[2];
            index[3] = arquivos[indexGrupoAtual].faces[i].vertices[3];

            if(superficie)
            {
                drawTriangle(arquivos[indexGrupoAtual].vertex[index[0]],
                             arquivos[indexGrupoAtual].vertex[index[1]],
                             arquivos[indexGrupoAtual].vertex[index[2]]);

                drawTriangle(arquivos[indexGrupoAtual].vertex[index[2]],
                             arquivos[indexGrupoAtual].vertex[index[3]],
                             arquivos[indexGrupoAtual].vertex[index[0]]);
            }
            else
            {
                drawTriangleWireframe(arquivos[indexGrupoAtual].vertex[index[2]],
                                      arquivos[indexGrupoAtual].vertex[index[3]],
                                      arquivos[indexGrupoAtual].vertex[index[0]]);
            }
        }
    }
}

void display(void)
{
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    showInfoOnTitle();

    glViewport(0,0,(GLsizei)width, (GLsizei)height);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(60.0, (GLfloat) (width)/(GLfloat) height, 1.0, 200.0);

    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();
    gluLookAt (0.0, 0.0, zdist, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glPushMatrix();
    glRotatef( rotationY, 0.0, 1.0, 0.0 );
    glRotatef( rotationX, 1.0, 0.0, 0.0 );
    glTranslatef(-arquivos[indexGrupoAtual].medio.x, -arquivos[indexGrupoAtual].medio.y, -arquivos[indexGrupoAtual].medio.z);
    modelaObjeto();
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
    case 'w':
        wireframe = true;
        superficie = false;
        break;
    case 's':
        superficie = true;
        wireframe = false;
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
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
    {
        last_x = x;
        last_y = y;
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
    case GLUT_KEY_RIGHT:
        if(indexGrupoAtual < VECTOR_SIZE - 1)
            indexGrupoAtual++;
        break;

    case GLUT_KEY_LEFT:
        if(indexGrupoAtual > 0)
            indexGrupoAtual--;
        break;

    case GLUT_KEY_F12:
        //Modo Fullscreen
        fullSreen = !fullSreen;
        if(fullSreen)
        {
            glutFullScreen();
        }
        else
        {
            glutReshapeWindow(800,600);
            glutPositionWindow(100,100);
        }
        break;
    }
    glutPostRedisplay();
}

void exibeMenu()
{
    printf("--------------MENU----------------\n");
    printf("<- e -> para alternar entre os objetos.\n");
    printf("w para representação no modo wireframe\n");
    printf("s para representação no modo superficie(PADRÂO)\n");
    printf("Utlize o mouse para rotacionar o objeto\n");
    printf("------------------------------------\n");
}

void readPlyFiles(string arquivo);

void readPlyFiles()
{
    arquivos.resize(VECTOR_SIZE);
    for(int i = 0; i < VECTOR_SIZE; i++)
    {
        readPlyFiles(fileName[i]);

        for(int i = 0; i < 3; i++)
        {
            for(int j = 0; j < 4; j++)
            {
                arquivos[indexGrupoAtual].matriz[i][j] = matrizIluminacao[indexGrupoAtual][i][j];
            }
        }
        indexGrupoAtual++;
    }
    indexGrupoAtual = 0;
}

void init(void)
{
    readPlyFiles();
    initLight(width, height); // Função extra para tratar iluminação.
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

/// Main
int main(int argc, char** argv)
{
    //Definição do primeiro grupo(padrao)
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


void readPlyFiles(string arquivo)
{
    // numero total de vertices do objeto
    int num_vertices;

    // numero total de faces do objeto
    int num_faces;

    char * cstr = new char [arquivo.length()+1];
    strcpy (cstr, arquivo.c_str());
    //abrindo o arquivo
    FILE *file = fopen(cstr, "r");
    char buff[255];

    if(!file)
    {
        printf("ERRO NA LEITURA DO ARQUIVO");
        return;
    }

    //o numero total de vertices e faces
    int cont_vertices = 0;
    int cont_faces = 0;

    //Indica em qual palavra estou na linha
    int indice_linha = 0;

    //numero de vertices por linha
    int num_propriedades = 0;

    //loop por cada linha do arquivo
    while(fgets(buff, sizeof(buff), file))
    {

        //dividi a linha em cada palavra (token)
        char *palavra = strtok(buff, " ");

        //se é a palavra 'element',paga o numero de vertices e faces
        if(strcmp (palavra, "element") == 0)
        {

            while(palavra != NULL)
            {

                if(strcmp (palavra, "vertex") == 0)
                {
                    palavra = strtok(NULL, " ");
                    num_vertices = atoi(palavra);
                }

                if(strcmp (palavra, "face") == 0)
                {
                    palavra = strtok(NULL, " ");
                    num_faces = atoi(palavra);
                }

                palavra = strtok(NULL, " ");
            }
        }

        //numero de propriedades que vai estar no vetor de vertices
        if(palavra != NULL && strcmp (palavra, "property") == 0)
        {
            char* proxima_palavra = strtok(NULL, " ");
            if(strcmp (proxima_palavra, "list") != 0)
            {
                num_propriedades++;
            }

        }
        //termino do header
        vertice v_min, v_max;
        if(buff[0] == 'e' && buff[1] == 'n' && buff[2] == 'd')
        {
            arquivos[indexGrupoAtual].vertex.resize(num_vertices);
            arquivos[indexGrupoAtual].faces.resize(num_faces);
            int cont = 0;
            //Leitura dos vertices
            while(fgets(buff, sizeof(buff), file))
            {
                char* vertice = strtok(buff, " ");
                arquivos[indexGrupoAtual].vertex[cont].x = atof(vertice);

                // proxima palavra da linha
                vertice = strtok(NULL, " ");
                arquivos[indexGrupoAtual].vertex[cont].y = atof(vertice);

                // proxima palavra da linha
                vertice = strtok(NULL, " ");
                arquivos[indexGrupoAtual].vertex[cont].z = atof(vertice);

                if(cont == 0)
                {
                    v_min.x = arquivos[indexGrupoAtual].vertex[cont].x;
                    v_min.y = arquivos[indexGrupoAtual].vertex[cont].y;
                    v_min.z = arquivos[indexGrupoAtual].vertex[cont].z;

                    v_max.x = arquivos[indexGrupoAtual].vertex[cont].x;
                    v_max.y = arquivos[indexGrupoAtual].vertex[cont].y;
                    v_max.z = arquivos[indexGrupoAtual].vertex[cont].z;
                }
                else
                {
                    if(v_min.x > arquivos[indexGrupoAtual].vertex[cont].x)
                        v_min.x = arquivos[indexGrupoAtual].vertex[cont].x;
                    if(v_min.y > arquivos[indexGrupoAtual].vertex[cont].y)
                        v_min.y = arquivos[indexGrupoAtual].vertex[cont].y;
                    if(v_min.z > arquivos[indexGrupoAtual].vertex[cont].z)
                        v_min.z = arquivos[indexGrupoAtual].vertex[cont].z;

                    if(v_max.x < arquivos[indexGrupoAtual].vertex[cont].x)
                        v_max.x = arquivos[indexGrupoAtual].vertex[cont].x;
                    if(v_max.y < arquivos[indexGrupoAtual].vertex[cont].y)
                        v_max.y = arquivos[indexGrupoAtual].vertex[cont].y;
                    if(v_max.z < arquivos[indexGrupoAtual].vertex[cont].z)
                        v_max.z = arquivos[indexGrupoAtual].vertex[cont].z;
                }

                cont++;

                if(cont == num_vertices)
                    break;
            }

            arquivos[indexGrupoAtual].medio.x = (v_min.x + v_max.x)/2.0;
            arquivos[indexGrupoAtual].medio.y = (v_min.y + v_max.y)/2.0;
            arquivos[indexGrupoAtual].medio.z = (v_min.z + v_max.z)/2.0;

            int k = 0;
            ///Leitura dos poligonos
            while(fgets(buff, sizeof(buff), file))
            {
                int tam  = atoi(strtok(buff, " "));
                arquivos[indexGrupoAtual].faces[k].vertices.resize(tam);

                char* verticeFace = strtok(NULL, " ");
                for (int j = 0; j < tam; j++)
                {
                    arquivos[indexGrupoAtual].faces[k].vertices[j] = stoi(verticeFace);
                    verticeFace = strtok(NULL, " ");
                }
                k++;
            }
            fclose(file);
        }
    }
}

void SetMaterial(float arrayAmbiente[], float arrayDifusa[], float arrayEspecular[])
{

    // Material do objeto (neste caso, ruby). Parametros em RGBA
    GLfloat objeto_ambient[]   = {arrayAmbiente[0], arrayAmbiente[1],arrayAmbiente[2],arrayAmbiente[3]};
    GLfloat objeto_difusa[]    = {arrayDifusa[0], arrayDifusa[1], arrayDifusa[2], arrayDifusa[3]};
    GLfloat objeto_especular[] = {arrayEspecular[0], arrayEspecular[1], arrayEspecular[2], arrayEspecular[3]};
    GLfloat objeto_brilho[]    = { 90.0f };

    // Define os parametros da superficie a ser iluminada
    glMaterialfv(GL_FRONT, GL_AMBIENT, objeto_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, objeto_difusa);
    glMaterialfv(GL_FRONT, GL_SPECULAR, objeto_especular);
    glMaterialfv(GL_FRONT, GL_SHININESS, objeto_brilho);
}
