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
#define VECTOR_SIZE 5
#include "camera.h"
#include "glcTexture.h"]
#include "glcWavefrontObject.h"

using namespace std;

/// Estruturas iniciais para armazenar vertices
class vertice
{
public:
    float x,y,z;
    float espessura;
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
    vertice medio;
    float scale_factor;
    float translateZ;
    float rotation[4];
};

class Ply
{
public:
    int objetoEscolhido;
    vertice posicao;
    float orientacao;
    int material;
};
/// Globals
float zdist = 30.0;

//Coordenada z do ponto criado
float alturaZPonto = 3.0;
float rotationX = 0.0, rotationY = 0.0;
int  last_x, last_y;
int  width, height;
int click = 0;
float espessuraGlobal = 1.0;
bool fullSreen = false;
vertice oldVector[4];
bool entraIf = false;
int indexGrupoAtual = 0;
int contPlyObjects = 0;
int materialSelected = 0;
int plySelected = 0;

Camera  g_camera;
bool 	g_key[256];
bool 	g_shift_down = false;
int 	g_viewport_width = 0;
int 	g_viewport_height = 0;
bool	fullscreen = false;	// Fullscreen Flag Set To Fullscreen Mode By Default
bool 	inverseMouse = false;
bool	boostSpeed = false; // Change keyboard speed
bool    flyMode = false;
bool	releaseMouse = false;
bool    waitingPosition = false;

// Movement settings
float g_translation_speed = 0.05;
float g_rotation_speed = M_PI/180*0.2;

/** determina qual view port esta visivel**/
bool viewPort = true;

float materiais[5][3][4] =
{
    {
        {0.2, 0.2, 0.2, 1.0} ,
        {0.8, 0.8, 0.8, 1.0},
        {0.0, 0.0, 0.0, 1.0}
    },
    {
        {.33, .22, .07, 1.0},
        {.78, .57, .11, 1.0},
        {.99, .94, .81, 1.0}
    },
    {
        {.10, .19, .17, .80},
        {.40, .74, .69, .80},
        {.30, .31, .31, .80}
    },
    {
        {.25, .25, .25, 1.0},
        {.40, .40, .40, 1.0},
        {.77, .77, .77, 1.0}
    },
    {
        {.25, .22, .06, 1.0},
        {.35, .31, .09, 1.0},
        {.80, .72, .21, 1.0}
    }
};
///Arquivos ply
string fileName[5] = {"egret.txt",
                      "big_act.txt",
                      "weathervane.txt",
                      "balance.txt",
                      "fracttree.txt"
                     };

vector<list<vertice>> grupos;
list<vertice> grupoAtual;
vector<Ply> objetosPly(10);
///Associa um tipo de material a cada grupo, Padrao 0(material 1)
vector<int> materialAssociado(15,0);
///Armazena os arquivos que podem ser inseridos no ambiente
vector<figure> arquivos(5);

glcTexture *textureManager;
///Associa uma textura a cada grupo, Padrao 0(sem textura)
vector<int> texturaAssociada(15,0);
int texturaAtual = 0;

glcWavefrontObject *objectManager = NULL;

void KeyboardUp(unsigned char key, int x, int y);
void MouseMotion(int x, int y);
void modela3D();
void modelaObjeto();
void initLuzScene();
void desenhaPonto(float x, float y);
void carregaModelo();
void carregaMuseu();

void KeyboardUp(unsigned char key, int x, int y)
{
    g_key[key] = false;
}

void Timer(int value)
{
    float speed = g_translation_speed;

    if(g_key['w'] || g_key['W'])
    {
        g_camera.Move(speed, flyMode);
    }
    else if(g_key['s'] || g_key['S'])
    {
        g_camera.Move(-speed, flyMode);
    }
    else if(g_key['a'] || g_key['A'])
    {
        g_camera.Strafe(speed);
    }
    else if(g_key['d'] || g_key['D'])
    {
        g_camera.Strafe(-speed);
    }
    glutTimerFunc(1, Timer, 0);
}

void setMaterial(void)
{
    // Material do objeto (neste caso, ruby). Parametros em RGBA
    GLfloat objeto_ambient[]   = { .1745, .01175, .01175, 1.0 };
    GLfloat objeto_difusa[]    = { .91424, .04136, .04136, 1.0 };
    GLfloat objeto_especular[] = { .727811, .626959, .626959, 1.0 };
    GLfloat objeto_brilho[]    = { 90.0f };

    // Define os parametros da superficie a ser iluminada
    glMaterialfv(GL_FRONT, GL_AMBIENT, objeto_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, objeto_difusa);
    glMaterialfv(GL_FRONT, GL_SPECULAR, objeto_especular);
    glMaterialfv(GL_FRONT, GL_SHININESS, objeto_brilho);
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

void setMaterialChao(void)
{
    // Material do objeto (neste caso, ruby). Parametros em RGBA
    GLfloat objeto_ambient[]   = { .0745, .01175, .11175, 1.0 };
    GLfloat objeto_difusa[]    = { .01424, .04136, .14136, 1.0 };
    GLfloat objeto_especular[] = { .627811, .626959, .726959, 1.0 };
    GLfloat objeto_brilho[]    = { 0.0f };

    // Define os parametros da superficie a ser iluminada
    glMaterialfv(GL_FRONT, GL_AMBIENT, objeto_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, objeto_difusa);
    glMaterialfv(GL_FRONT, GL_SPECULAR, objeto_especular);
    glMaterialfv(GL_FRONT, GL_SHININESS, objeto_brilho);
}


void scene()
{
    float size = 110.0f;
    glPushMatrix();
    setMaterialChao();

    textureManager->Bind(0);
    float aspectRatio = textureManager->GetAspectRatio(0);

    // Calculo abaixo funciona apenas se textura estiver centralizada na origem
    float h = 110.0f;
    float w = 110.0f;

    if( aspectRatio > 1.0f) h  = h/aspectRatio;
    else                    w  = w*aspectRatio;

    float repeat = 200.0;
    glPushMatrix();
        glRotatef( -90.0, 1.0, 0.0, 0.0 );
            glBegin(GL_QUADS);
                glNormal3f(0.0, 0.0, 1.0);
                glTexCoord2f(0.0, 0.0);
                glVertex3f(-w, -h, 0.0);

                glTexCoord2f(repeat, 0.0);
                glVertex3f(w, -h, 0.0);

                glTexCoord2f(repeat, repeat);
                glVertex3f(w, h, 0.0);

                glTexCoord2f(0.0, repeat);
                glVertex3f(-w, h, 0.0);
            glEnd();
    glPopMatrix();

    glPushMatrix();
    glRotated(-90.0,1.0,0.0,0.0);
    modela3D();
        glPushMatrix();
            glTranslatef(0.0,5.0,0.0);
            modelaObjeto();
        glPopMatrix();
    glPopMatrix();
}

/// Functions
void init(void)
{
    initLight(width, height); // Função extra para tratar iluminação.
    carregaMuseu();

    textureManager = new glcTexture();
    textureManager->SetNumberOfTextures(9);
    textureManager->CreateTexture("../data/marble.png", 0);
    textureManager->CreateTexture("../data/teste.png", 1);
    textureManager->CreateTexture("../data/carbon.png", 2);
    textureManager->CreateTexture("../data/paper.png", 3);
    textureManager->CreateTexture("../data/abapuru.png", 4);
    textureManager->CreateTexture("../data/trabalho.png", 5);
    textureManager->CreateTexture("../data/religiao.png", 6);
    textureManager->CreateTexture("../data/a-negra.png", 7);
    textureManager->CreateTexture("../data/horizontal.png", 8);
}

void initLuzScene()
{
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glEnable(GL_LIGHT1);                   // habilita luz 0

    // Cor da fonte de luz (RGBA)
    GLfloat cor_luz[]     = { 1.0, 1.0, 1.0, 1.0};

    // Posicao da fonte de luz. Ultimo parametro define se a luz sera direcional (0.0) ou tera uma posicional (1.0)
    GLfloat posicao_luz[] = { 50.0, 50.0, 50.0, 1.0};

    // Define parametros da luz
    glLightfv(GL_LIGHT1, GL_AMBIENT, cor_luz);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, cor_luz);
    glLightfv(GL_LIGHT1, GL_SPECULAR, cor_luz);
    glLightfv(GL_LIGHT1, GL_POSITION, posicao_luz);
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

void CalculoOrtogonal(vertice v1, vertice v2, vertice* vn)
{
    vertice vetor;
    vetor.x = v1.x - v2.x;
    vetor.y = v1.y - v2.y;
    vetor.z = 0.0;

    vn->x = vetor.y;
    vn->y = -vetor.x;

    float v_length = sqrt(pow(vetor.x/2 - vn->x,2) + pow(vetor.y/2 - vn->y,2));

    vn->x /= v_length;
    vn->y /= v_length;
}

void drawTriangle(vertice v1, vertice v2, vertice v3, vertice v4)
{
    vertice vetorNormal;
    vertice v[4] = {{v1.x, v1.y,  v1.z},
        {v2.x, v2.y,  v2.z},
        {v3.x,  v3.y,  v3.z},
        {v4.x,  v4.y, v4.z}
    };

    triangle t[2] = {{v[0], v[2], v[1]},
        {v[2], v[0], v[3]}
    };
    float repeat = 1.0;

    glPushMatrix();
        glRotatef( -90.0, 1.0, 0.0, 0.0 );
            glBegin(GL_QUADS);
                //glNormal3f(-1.0, 0.0, 0.0);
                CalculaNormal(t[0], &vetorNormal); // Passa face triangular e endereço do vetor normal de saída
                glNormal3f(vetorNormal.x, vetorNormal.y,vetorNormal.z);

                glTexCoord2f(0.0, 0.0);
                glVertex3f(v1.x, v1.y, v1.z);

                glTexCoord2f(repeat, 0.0);
                glVertex3f(v4.x, v4.y, v4.z);

                 CalculaNormal(t[1], &vetorNormal); // Passa face triangular e endereço do vetor normal de saída
                glNormal3f(vetorNormal.x, vetorNormal.y,vetorNormal.z);

                glTexCoord2f(repeat, repeat);
                glVertex3f(v3.x, v3.y, v3.z);

                glTexCoord2f(0.0, repeat);
                glVertex3f(v2.x, v2.y, v2.z);

            glEnd();
    glPopMatrix();
}

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

void drawSolido(vertice v1, vertice v2, int indexGrupo)
{
    vertice ortogonal;
    CalculoOrtogonal(v1, v2, &ortogonal);

    vertice v3, v4, v5, v6, v7, v8, v9, v10;

    v3.x =  v1.x + ortogonal.x*(v1.espessura);
    v3.y =  v1.y + ortogonal.y*(v1.espessura);
    v3.z = 0;

    v4.x =  v1.x - ortogonal.x*(v1.espessura);
    v4.y =  v1.y - ortogonal.y*(v1.espessura);
    v4.z = 0;

    v5.x =  v2.x + ortogonal.x*(v2.espessura);
    v5.y =  v2.y + ortogonal.y*(v2.espessura);
    v5.z = 0;

    v6.x =  v2.x - ortogonal.x*(v2.espessura);
    v6.y =  v2.y - ortogonal.y*(v2.espessura);
    v6.z = 0;

    if(indexGrupo >= 1 && indexGrupo <= 6){
        v3.z = 2.0;
        v4.z = 2.0;
        v5.z = 2.0;
        v6.z = 2.0;
    }

    v7.x = v3.x;
    v7.y = v3.y;
    v7.z = v1.z;

    v8.x = v4.x;
    v8.y = v4.y;
    v8.z = v1.z;

    v9.x = v5.x;
    v9.y = v5.y;
    v9.z = v2.z;

    v10.x = v6.x;
    v10.y = v6.y;
    v10.z = v2.z;

    if(grupos[indexGrupo].size() > 2 && entraIf)
    {
        v3 = oldVector[0];
        v7 = oldVector[1];
        v8 = oldVector[2];
        v4 = oldVector[3];
    }

    oldVector[0] = v5;
    oldVector[1] = v9;
    oldVector[2] = v10;
    oldVector[3] = v6;


    //1 face do solido
    drawTriangle(v3,v4,v6,v5);

    //2 face do solido
    drawTriangle(v7,v3,v5,v9);

    //3 face do solido
    drawTriangle(v7,v8,v4,v3);

    //4 face do solido
    drawTriangle(v4,v8,v10,v6);

    //5 face do solido
    drawTriangle(v10,v9,v5,v6);

    //6 face do solido
    drawTriangle(v8,v7,v9,v10);

    entraIf = true;
}

void showInfoOnTitle(int group,float height)
{
    static char title[256] = {0};
    char aux1[32];
    sprintf(aux1,"Grupo atual: %.1d, ",group);
    char aux2[32];
    sprintf(aux2,"Material atual : %d, ",materialSelected + 1);
    char aux3[32];
    sprintf(aux2,"Textura atual : %d, ",texturaAtual + 1);
    char aux4[32];
    sprintf(aux3,"Arquivo Ply escolhido: ");
    char aux5[32];
    std::size_t length = fileName[plySelected].copy(aux4,fileName[plySelected].size(),0);
    aux4[length] = '\0';

    strcpy(title, "Ambiente Virtual, " );
    strcat(title,aux1);
    strcat(title,aux2);
    strcat(title,aux3);
    strcat(title,aux4);
    strcat(title,aux5);
    strcat(title," - Pressione ESC para sair");

    glutSetWindowTitle(title);
}

void desenhaEixos2D()
{
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

void desenhaPontoPly(float x, float y)
{
    glDisable(GL_LIGHTING);
    glPointSize(10.0);
    glEnable(GL_POINT_SMOOTH);
    glBegin(GL_POINTS);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(x, y, 0.0);
    glEnd();
    glEnable(GL_LIGHTING);
}

void modela3D()
{
    for(int i = 0; i < grupos.size(); i++)
    {
        SetMaterial(materiais[materialAssociado[i]][0], materiais[materialAssociado[i]][1],
                    materiais[materialAssociado[i]][2]);

        textureManager->Bind(texturaAssociada[i]);
        float aspectRatio = textureManager->GetAspectRatio(texturaAssociada[i]);


        for(list<vertice>::iterator it = grupos[i].begin(); it != grupos[i].end(); it++)
        {
            vertice v1;
            v1.x = it->x;
            v1.y = it->y;
            v1.z = it->z;
            v1.espessura = it->espessura;

            it++;
            if(it != grupos[i].end())
            {
                vertice v2;
                v2.x = it->x;
                v2.y = it->y;
                v2.z = it->z;
                v2.espessura = it->espessura;

                drawSolido(v1,v2,i);
            }
            else
            {
                break;
            }
            it--;
        }
        entraIf = false;

    }
    setMaterial();

}

void readPlyFiles(string arquivo, int contador)
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
            arquivos[contador].vertex.resize(num_vertices);
            arquivos[contador].faces.resize(num_faces);
            int cont = 0;
            //Leitura dos vertices
            while(fgets(buff, sizeof(buff), file))
            {
                char* vertice = strtok(buff, " ");
                arquivos[contador].vertex[cont].x = atof(vertice);

                // proxima palavra da linha
                vertice = strtok(NULL, " ");
                arquivos[contador].vertex[cont].y = atof(vertice);

                // proxima palavra da linha
                vertice = strtok(NULL, " ");
                arquivos[contador].vertex[cont].z = atof(vertice);

                if(cont == 0)
                {
                    v_min.x = arquivos[contador].vertex[cont].x;
                    v_min.y = arquivos[contador].vertex[cont].y;
                    v_min.z = arquivos[contador].vertex[cont].z;

                    v_max.x = arquivos[contador].vertex[cont].x;
                    v_max.y = arquivos[contador].vertex[cont].y;
                    v_max.z = arquivos[contador].vertex[cont].z;
                }
                else
                {
                    if(v_min.x > arquivos[contador].vertex[cont].x)
                        v_min.x = arquivos[contador].vertex[cont].x;
                    if(v_min.y > arquivos[contador].vertex[cont].y)
                        v_min.y = arquivos[contador].vertex[cont].y;
                    if(v_min.z > arquivos[contador].vertex[cont].z)
                        v_min.z = arquivos[contador].vertex[cont].z;

                    if(v_max.x < arquivos[contador].vertex[cont].x)
                        v_max.x = arquivos[contador].vertex[cont].x;
                    if(v_max.y < arquivos[contador].vertex[cont].y)
                        v_max.y = arquivos[contador].vertex[cont].y;
                    if(v_max.z < arquivos[contador].vertex[cont].z)
                        v_max.z = arquivos[contador].vertex[cont].z;
                }

                cont++;

                if(cont == num_vertices)
                    break;
            }

            arquivos[contador].medio.x = (v_min.x + v_max.x)/2.0;
            arquivos[contador].medio.y = (v_min.y + v_max.y)/2.0;
            arquivos[contador].medio.z = (v_min.z + v_max.z)/2.0;

            int k = 0;
            ///Leitura dos poligonos
            while(fgets(buff, sizeof(buff), file))
            {
                int tam  = atoi(strtok(buff, " "));
                arquivos[contador].faces[k].vertices.resize(tam);

                char* verticeFace = strtok(NULL, " ");
                for (int j = 0; j < tam; j++)
                {
                    arquivos[contador].faces[k].vertices[j] = atoi(verticeFace);
                    verticeFace = strtok(NULL, " ");
                }
                k++;
            }
            fclose(file);
        }
    }
}

void readPlyFiles()
{
    arquivos.resize(VECTOR_SIZE);
    for(int i = 0; i < VECTOR_SIZE; i++)
    {
        readPlyFiles(fileName[i],i);
    }

    arquivos[0].scale_factor = 0.001;
    arquivos[0].rotation[0] = 0.0;
    arquivos[0].rotation[1] = 0.0;
    arquivos[0].rotation[2] = 0.0;
    arquivos[0].rotation[3] = 0.0;
    arquivos[0].translateZ = 2.18;

    arquivos[1].scale_factor = 0.7;
    arquivos[1].rotation[0] = 90.0;
    arquivos[1].rotation[1] = 1.0;
    arquivos[1].rotation[2] = 0.0;
    arquivos[1].rotation[3] = 0.0;
    arquivos[1].translateZ = 0.2;

    arquivos[2].scale_factor = 0.003;
    arquivos[2].rotation[0] = 0.0;
    arquivos[2].rotation[1] = 0.0;
    arquivos[2].rotation[2] = 0.0;
    arquivos[2].rotation[3] = 0.0;
    arquivos[2].translateZ = 1.5;

    arquivos[3].scale_factor = 0.025;
    arquivos[3].rotation[0] = 0.0;
    arquivos[3].rotation[1] = 0.0;
    arquivos[3].rotation[2] = 0.0;
    arquivos[3].rotation[3] = 0.0;
    arquivos[3].translateZ = 0.0;

    arquivos[4].scale_factor = 1.6;
    arquivos[4].rotation[0] = 90.0;
    arquivos[4].rotation[1] = 1.0;
    arquivos[4].rotation[2] = 0.0;
    arquivos[4].rotation[3] = 0.0;
    arquivos[4].translateZ = 7.0;
}

void modelaObjeto()
{
    for(int j = 0; j < contPlyObjects; j++)
    {
        glPushMatrix();
        glTranslatef(objetosPly[j].posicao.x ,objetosPly[j].posicao.y ,arquivos[objetosPly[j].objetoEscolhido].translateZ);
        float rotation[4];
        rotation[0] = arquivos[objetosPly[j].objetoEscolhido].rotation[0];
        rotation[1] = arquivos[objetosPly[j].objetoEscolhido].rotation[1];
        rotation[2] = arquivos[objetosPly[j].objetoEscolhido].rotation[2];
        rotation[3] = arquivos[objetosPly[j].objetoEscolhido].rotation[3];

        glRotatef( rotation[0], rotation[1], rotation[2], rotation[3]);

        float scale = arquivos[objetosPly[j].objetoEscolhido].scale_factor;
        glScalef(scale,scale,scale);
        glEnable (GL_NORMALIZE);
        SetMaterial(materiais[objetosPly[j].material][0],materiais[objetosPly[j].material][1],
                    materiais[objetosPly[j].material][2]);
        for(int i = 0; i < arquivos[objetosPly[j].objetoEscolhido].faces.size(); i++)
        {
            if(arquivos[objetosPly[j].objetoEscolhido].faces[i].vertices.size() == 3)
            {
                int index[3];
                index[0] = arquivos[objetosPly[j].objetoEscolhido].faces[i].vertices[0];
                index[1] = arquivos[objetosPly[j].objetoEscolhido].faces[i].vertices[1];
                index[2] = arquivos[objetosPly[j].objetoEscolhido].faces[i].vertices[2];


                drawTriangle(arquivos[objetosPly[j].objetoEscolhido].vertex[index[0]],
                             arquivos[objetosPly[j].objetoEscolhido].vertex[index[1]],
                             arquivos[objetosPly[j].objetoEscolhido].vertex[index[2]]
                             );
            }
            else
            {
                int index[4];
                index[0] = arquivos[objetosPly[j].objetoEscolhido].faces[i].vertices[0];
                index[1] = arquivos[objetosPly[j].objetoEscolhido].faces[i].vertices[1];
                index[2] = arquivos[objetosPly[j].objetoEscolhido].faces[i].vertices[2];
                index[3] = arquivos[objetosPly[j].objetoEscolhido].faces[i].vertices[3];


                drawTriangle(arquivos[objetosPly[j].objetoEscolhido].vertex[index[0]],
                             arquivos[objetosPly[j].objetoEscolhido].vertex[index[1]],
                             arquivos[objetosPly[j].objetoEscolhido].vertex[index[2]]);

                drawTriangle(arquivos[objetosPly[j].objetoEscolhido].vertex[index[2]],
                             arquivos[objetosPly[j].objetoEscolhido].vertex[index[3]],
                             arquivos[objetosPly[j].objetoEscolhido].vertex[index[0]]);

            }
        }
        glPopMatrix();

    }
}

void inseriObjeto()
{
    objetosPly[contPlyObjects].material = materialSelected;
    objetosPly[contPlyObjects].objetoEscolhido = plySelected;
    objetosPly[contPlyObjects].orientacao = 0.0;
}

void carregaModelo()
{
    char nomeArquivoAux [100];
    char nomeArquivo [100];
    printf("Digite nome do modelo a ser carregado: \n");
    cin >> nomeArquivo;

    strcat(nomeArquivo,".txt");

    //abrindo o arquivo
    FILE *file = fopen(nomeArquivo, "r");
    char buff[255];

    if(!file)
    {
        printf("ERRO NA LEITURA DO ARQUIVO");
        return;
    }

    //Descarta grupos anteriores
    grupos.erase(grupos.begin(), grupos.end());
    grupos.resize(15);
    indexGrupoAtual = -1;

    contPlyObjects = 0;

    bool leituraGrupos = true;
    //loop por cada linha do arquivo
    while(fgets(buff, sizeof(buff), file))
    {
        //dividi a linha em cada palavra (token)
        char *palavra = strtok(buff, " ");

        if(strcmp(palavra, "GRUPOS") == 0)
        {
            leituraGrupos = true;
            palavra = strtok(NULL, " ");
        }

        if(strcmp(palavra, "PLY") == 0)
        {
            leituraGrupos = false;
            palavra = strtok(NULL, " ");
        }

        if(strcmp(palavra, "material") == 0 && leituraGrupos == true)
        {
            indexGrupoAtual++;
            palavra = strtok(NULL, " ");
            materialAssociado[indexGrupoAtual] =  atoi(palavra);

            palavra = strtok(NULL, " ");
            texturaAssociada[indexGrupoAtual] =  atoi(palavra);

            palavra = strtok(NULL, " ");
        }

        if(leituraGrupos == true &&
                strcmp(palavra, "material") != 0 &&
                strcmp(palavra, "GRUPOS") != 0 &&
                strcmp(palavra, "\n") != 0)
        {

            vertice *novo = new vertice();
            novo->x = atof(palavra);
            palavra = strtok(NULL, " ");

            novo->y =  atof(palavra);
            palavra = strtok(NULL, " ");

            novo->z =  atof(palavra);
            palavra = strtok(NULL, " ");

            novo->espessura =  atof(palavra);
            grupos[indexGrupoAtual].push_back(*novo);
        }

         if(leituraGrupos == false &&
                strcmp(palavra, "material") != 0 &&
                strcmp(palavra, "PLY") != 0 &&
                strcmp(palavra, "\n") != 0)
        {
            objetosPly[contPlyObjects].objetoEscolhido = atoi(palavra);
            palavra = strtok(NULL, " ");

            objetosPly[contPlyObjects].posicao.x = atof(palavra);
            palavra = strtok(NULL, " ");

            objetosPly[contPlyObjects].posicao.y = atof(palavra);
            palavra = strtok(NULL, " ");

            objetosPly[contPlyObjects].posicao.z = 0;
            objetosPly[contPlyObjects].posicao.espessura = -1;

            objetosPly[contPlyObjects].orientacao = atof(palavra);
            palavra = strtok(NULL, " ");

            objetosPly[contPlyObjects].material = atoi(palavra);
            contPlyObjects++;
        }
    }

    fclose(file);
    cout << "FIM" << endl;
}


void carregaMuseu()
{
    //abrindo o arquivo
    FILE *file = fopen("museu.txt", "r");
    char buff[255];

    //Descarta grupos anteriores
    grupos.erase(grupos.begin(), grupos.end());
    grupos.resize(15);
    indexGrupoAtual = -1;

    contPlyObjects = 0;

    bool leituraGrupos = true;
    //loop por cada linha do arquivo
    while(fgets(buff, sizeof(buff), file))
    {
        //dividi a linha em cada palavra (token)
        char *palavra = strtok(buff, " ");

        if(strcmp(palavra, "GRUPOS") == 0)
        {
            leituraGrupos = true;
            palavra = strtok(NULL, " ");
        }

        if(strcmp(palavra, "PLY") == 0)
        {
            leituraGrupos = false;
            palavra = strtok(NULL, " ");
        }

        if(strcmp(palavra, "material") == 0 && leituraGrupos == true)
        {
            indexGrupoAtual++;
            palavra = strtok(NULL, " ");
            materialAssociado[indexGrupoAtual] =  atoi(palavra);

            palavra = strtok(NULL, " ");
            texturaAssociada[indexGrupoAtual] =  atoi(palavra);

            palavra = strtok(NULL, " ");
        }

        if(leituraGrupos == true &&
                strcmp(palavra, "material") != 0 &&
                strcmp(palavra, "GRUPOS") != 0 &&
                strcmp(palavra, "\n") != 0)
        {

            vertice *novo = new vertice();
            novo->x = atof(palavra);
            palavra = strtok(NULL, " ");

            novo->y =  atof(palavra);
            palavra = strtok(NULL, " ");

            novo->z =  atof(palavra);
            palavra = strtok(NULL, " ");

            novo->espessura =  atof(palavra);
            grupos[indexGrupoAtual].push_back(*novo);
        }

         if(leituraGrupos == false &&
                strcmp(palavra, "material") != 0 &&
                strcmp(palavra, "PLY") != 0 &&
                strcmp(palavra, "\n") != 0)
        {
            objetosPly[contPlyObjects].objetoEscolhido = atoi(palavra);
            palavra = strtok(NULL, " ");

            objetosPly[contPlyObjects].posicao.x = atof(palavra);
            palavra = strtok(NULL, " ");

            objetosPly[contPlyObjects].posicao.y = atof(palavra);
            palavra = strtok(NULL, " ");

            objetosPly[contPlyObjects].posicao.z = 0;
            objetosPly[contPlyObjects].posicao.espessura = -1;

            objetosPly[contPlyObjects].orientacao = atof(palavra);
            palavra = strtok(NULL, " ");

            objetosPly[contPlyObjects].material = atoi(palavra);
            contPlyObjects++;
        }
    }

    fclose(file);
}


void salvaModelo()
{
    FILE *arq;
    int result;

    char nomeArquivo[100];

    printf("Digite o nome do arquivo para salvar o modelo: \n");
    cin >> nomeArquivo;

    strcat(nomeArquivo,".txt");
    arq = fopen(nomeArquivo, "wt");  /// Cria um arquivo texto para gravação

    result = fprintf(arq,"GRUPOS \n");
    for (int j = 0; j < grupos.size(); j++ )
    {
        result = fprintf(arq,"material %d \n",materialAssociado[j]);
        for(list<vertice>::iterator it = grupos[j].begin(); it != grupos[j].end(); it++)
        {
            /// formato: x y z espessura
            result = fprintf(arq,"%f %f %f %f\n",it->x, it->y, it->z, it->espessura);
        }
    }

    result = fprintf(arq,"PLY \n");
    for (int i = 0; i < contPlyObjects; i++ )
    {
        /// formato: objetoEscolhido x y orientacao material
        result = fprintf(arq,"%d %f %f %f %d\n",objetosPly[i].objetoEscolhido,
        objetosPly[i].posicao.x,
        objetosPly[i].posicao.y,
        objetosPly[i].orientacao,
        objetosPly[i].material);
    }

    fclose(arq);

    printf("MODELO SALVO \n");
}

void calculaCoordenadasPonto(float x, float y)
{
    float pointX = (-10 + x /(width/2) *20);
    float pointY = (10 - y/height * 20);

    //adiciona novo vertice ao grupo atual
    vertice *novo = new vertice();
    novo->x = pointX;
    novo->y = pointY;
    novo->z = alturaZPonto;
    novo->espessura = espessuraGlobal;
    grupos[indexGrupoAtual].push_back(*novo);
}

void desenhaPonto(float x, float y)
{
    glDisable(GL_LIGHTING);
    glPointSize(10.0);
    glEnable(GL_POINT_SMOOTH);
    glBegin(GL_POINTS);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(x, y, 0.0);
    glEnd();
    glEnable(GL_LIGHTING);
}

void display(void)
{
    if(viewPort == true)
    {
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /// Index grupo, alturaZPonto
        showInfoOnTitle(indexGrupoAtual,alturaZPonto);

        ///Desenha primeira ViewPort (2D)
        glLoadIdentity ();
        glViewport ((int) 0, (int) 0, (int) width/2, (int) height);

        glDisable(GL_DEPTH_TEST);
        ///Define cor de fundo da primeira viewport
        glScissor((int) 0, (int) 0, (int) width/2, (int) height);
        glEnable(GL_SCISSOR_TEST);
        glClearColor(1.0, 1.0, 1.0, 1.0 );
        glClear(GL_COLOR_BUFFER_BIT);

        glutSetCursor(GLUT_CURSOR_CROSSHAIR);
        glMatrixMode (GL_PROJECTION);
        glLoadIdentity ();
        glOrtho(-10.0, 10.0, -10.0, 10.0, -1, 1);

        /*desenha eixos 2D*/
        desenhaEixos2D();
        for(int j = 0; j < contPlyObjects; j++)
        {
            desenhaPontoPly(objetosPly[j].posicao.x, objetosPly[j].posicao.y);
        }

        /*desenha ponto 2D*/
        if(click == 1)
        {
            for(int i = 0; i < grupos.size(); i++)
            {
                for(list<vertice>::iterator it = grupos[i].begin(); it != grupos[i].end(); it++)
                {
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

        //seleciona a textura
        /*Objetos 3D*/
        glMatrixMode (GL_MODELVIEW);
        glPushMatrix();
        glRotatef( rotationY, 0.0, 1.0, 0.0 );
        glRotatef( rotationX, 1.0, 0.0, 0.0 );

        //Desenha objeto 3D
        setMaterials();
        modela3D();
        glPopMatrix();

        glutSwapBuffers();
        textureManager->Disable();
    }
    else
    {
        ///Desenha segunda ViewPort, Navegação (3D)
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0, 0.0, 0.0, 0.0 );
        glDisable(GL_SCISSOR_TEST);

        glMatrixMode (GL_PROJECTION);
        glLoadIdentity ();
        gluPerspective(60.0, (GLfloat) (width)/(GLfloat) height, 1.0, 200.0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity ();
        gluLookAt (0.0, 0.0, zdist, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
        glViewport ((int) 0, (int) 0, (int)  width, (int) height);

        /*Objetos 3D*/
        textureManager->Bind(0);
        glutSetCursor(GLUT_CURSOR_NONE);
        glMatrixMode (GL_MODELVIEW);
        glPushMatrix();
        g_camera.Refresh();
        //Desenha objeto 3D
        scene();
        glPopMatrix();
        glutSwapBuffers();
        textureManager->Disable();
    }
}

void idle ()
{
    glutPostRedisplay();
}

void reshape (int w, int h)
{
    width = w;
    height = h;

    g_viewport_width = w;
    g_viewport_height = h;
}

void keyboard (unsigned char key, int x, int y)
{
    switch (tolower(key))
    {
    case 27:
        exit(0);
        break;
    case '.':
        //Aumenta espessura do modelo 3D
        espessuraGlobal +=0.2;
        break;
    case ',':
        if(espessuraGlobal > 0.4)
        {
            espessuraGlobal -= 0.2;
        }
        break;

    case 's':
        if(viewPort)
            salvaModelo();
        break;
    case 'l':
        carregaModelo();
        break;
    case 'm':
        viewPort = !viewPort;
        g_camera.SetPos(0.0,1.0,6.0);
        break;
    case 'p':
        if(viewPort)
        {
            waitingPosition = true;
            inseriObjeto();
        }
        break;
    case 'b':
        boostSpeed = !boostSpeed;
        (boostSpeed) ? g_translation_speed = 0.5 : g_translation_speed = 0.5;
        boostSpeed ? printf("BoostMode ON\n") : printf("BoostMode OFF\n");
        break;
    case '1':
        materialAssociado[indexGrupoAtual] =  0;
        materialSelected = 0;
        break;
    case '2':
        materialAssociado[indexGrupoAtual] =  1;
        materialSelected = 1;
        break;
    case '3':
        materialAssociado[indexGrupoAtual] =  2;
        materialSelected = 2;
        break;
    case '4':
        materialAssociado[indexGrupoAtual] =  3;
        materialSelected = 3;
        break;
    case '5':
        materialAssociado[indexGrupoAtual] =  4;
        materialSelected = 4;
        break;
    case '6':
        plySelected = 0;
        break;
    case '7':
        plySelected = 1;
        break;
    case '8':
        plySelected = 2;
        break;
    case '9':
        plySelected = 3;
        break;
    case '0':
        plySelected = 4;
        break;
    case '+':
        if(texturaAtual < 3){
            texturaAtual++;
        }
        texturaAssociada[indexGrupoAtual] =  texturaAtual;
        break;
    case '-':
        if(texturaAtual > 0){
            texturaAtual--;
        }
        texturaAssociada[indexGrupoAtual] =  texturaAtual;
        break;
    }
    g_key[key] = true;
}

// Motion callback
void motion(int x, int y )
{
    if( viewPort == true)
    {
        rotationX += (float) (y - last_y);
        rotationY += (float) (x - last_x);

        last_x = x;
        last_y = y;
    }
    else
    {
        MouseMotion(x,y);
    }
}

void MouseMotion(int x, int y)
{
    if(viewPort == false)
    {
        // This variable is hack to stop glutWarpPointer from triggering an event callback to Mouse(...)
        // This avoids it being called recursively and hanging up the event loop
        static bool just_warped = false;

        if(just_warped)
        {
            just_warped = false;
            return;
        }

        int dx = x - g_viewport_width/2;
        int dy = y - g_viewport_height/2;

        if(dx)
            g_camera.RotateYaw(g_rotation_speed*dx);
        if(dy)
            g_camera.RotatePitch(g_rotation_speed*dy);

        if(!releaseMouse)
            glutWarpPointer(g_viewport_width/2, g_viewport_height/2);

        just_warped = true;
    }
}

void savePlyPosition(float x, float y)
{
    float pointX = (-10 + x /(width/2) *20);
    float pointY = (10 - y/height * 20);

    objetosPly[contPlyObjects].posicao.espessura = -1;
    objetosPly[contPlyObjects].posicao.z = 0;
    objetosPly[contPlyObjects].posicao.x = pointX;
    objetosPly[contPlyObjects].posicao.y = pointY;

    waitingPosition = false;
    contPlyObjects++;
}

// Mouse callback
void mouse(int button, int state, int x, int y)
{
    if(viewPort == true)
    {
        if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
        {
            last_x = x;
            last_y = y;
            click = 1;
            if(waitingPosition)
            {
                savePlyPosition(last_x,last_y);
            }
            else
            {
                if(x < width/2)
                    calculaCoordenadasPonto(x,y);
            }
        }
        if ( button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN )
        {
            if(grupos[indexGrupoAtual].size() > 0)
            {
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
        if(indexGrupoAtual >= grupos.size())
        {
            list<vertice> grupo1;
            grupos.push_back(grupo1);
        }
        break;
    case GLUT_KEY_LEFT:
        //troca grupo
        if(indexGrupoAtual > 0)
        {
            indexGrupoAtual--;
        }
        break;
    default:
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
    }
    glutPostRedisplay();
}

void exibeMenu()
{
    printf("--------------Menu-----------------\n");
    printf("LEFT(DIRECIONAL) Retorna ao grupo anterior. \n");
    printf("RIGHT(DIRECIONAL) Avança para o proximo grupo. \n");
    printf("UP(DIRECIONAL) Aumenta altura do ponto a ser criado. \n");
    printf("DOWN(DIRECIONAL) Diminui altura do ponto a ser criado. \n");
    printf("F12 Modo Fullscreen. \n");
    printf("s Salvar o modelo(modo edição) \n");
    printf("l Carregar modelo.\n");
    printf(". Aumenta espessura do modelo gerado.\n");
    printf(", Diminui espessura do modelo gerado.\n");
    printf("1,2,3,4 e 5 Escolhe material a ser aplicado nos próximos objetos.\n");
    printf("6,7,8,9 e 0 Escolhe objeto ply a ser inserido(olhar nome da janela)\n");
    printf("p Inserção de um objeto ply e sua orientação\n");
    printf("ESC para sair.\n");
    printf("-------------------------------------\n\n");
}

/// Main
int main(int argc, char** argv)
{
    //Definição do primeiro grupo(padrao)
    list<vertice> grupo1;
    grupoAtual = grupo1;
    grupos.push_back(grupo1);

    readPlyFiles();
    exibeMenu();
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (800, 600);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("Edição e Navegação");

    glutIgnoreKeyRepeat(1);

    init ();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc( mouse );
    glutMotionFunc( motion );
    glutPassiveMotionFunc ( MouseMotion );
    glutSpecialFunc(specialKeysPress);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(KeyboardUp);
    glutIdleFunc(idle);
    glutTimerFunc(1, Timer, 0);
    glutMainLoop();
    return 0;
}

