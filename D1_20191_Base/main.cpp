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
#include <fstream>

#include "camera.h"

using namespace std;

/// Estruturas iniciais para armazenar vertices
//  Você poderá utilizá-las adicionando novos métodos (de acesso por exemplo) ou usar suas próprias estruturas.
class vertice
{
public:
    float x,y,z;
    float espessura;
    int indexMaterial;
};

class triangle
{
public:
    vertice v[3];
};

class figura
{
public:
    vector<list<vertice>> objetoGrupos;
};

/// Globals
float zdist = 30.0;

//Coordenada z do ponto criado
float alturaZPonto = 3.0;
float rotationX = 0.0, rotationY = 0.0;
int  last_x, last_y;
int  width, height;
int click = 0;
float espessuraGlobal = 1;
bool fullSreen = false;
vertice oldVector[4];
bool entraIf = false;
int indexGrupoAtual = 0;

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

// Movement settings
float g_translation_speed = 0.05;
float g_rotation_speed = M_PI/180*0.2;

/** determina qual view port esta visivil**/
bool viewPort = true;

vector<list<vertice>> grupos;
list<vertice> grupoAtual;

void readPlyFiles(char *arquivo);
void KeyboardUp(unsigned char key, int x, int y);
void MouseMotion(int x, int y);
void modela3D();
void initLuzScene();

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
    glScalef(size, .1, size);
    setMaterialChao();
    glutSolidCube(0.5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0,1.0,0.0);
    glRotated(-90.0,1.0,0.0,0.0);
	modela3D();
	glPopMatrix();
}

/// Functions
void init(void)
{
    initLight(width, height); // Função extra para tratar iluminação.
}

void initLuzScene(){
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

    triangle t[2] = {{v[0], v[1], v[2]},
        {v[2], v[3], v[0]}
    };

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
    sprintf(aux2,"Altura do ponto: %.1f",height);

    strcpy(title, "Modelagem 2D-3D, " );
    strcat(title,aux1);
    strcat(title,aux2);
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

void modela3D()
{
    for(int i = 0; i < grupos.size(); i++)
    {
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
}

void carregaModelo()
{
    //Espaço como separador de string
    string line;
    string delimiter = " ";

    char nomeArquivo [100];
    printf("Digite nome do modelo a ser carregado: \n");
    cin >> nomeArquivo;
    strcat(nomeArquivo,".txt");

    ifstream myfile (nomeArquivo);

    //Descarta grupos anteriores
    grupos.erase(grupos.begin(), grupos.end());
    grupos.resize(10);

    indexGrupoAtual = 0;

    if (myfile.is_open())
    {
        size_t pos = 0;
        std::string token;

        while ( getline (myfile,line) )
        {
            int j = 0;
            float auxArray[4];

            //Separa campos para serem armazenados
            while ((pos = line.find(delimiter)) != std::string::npos)
            {
                token = line.substr(0, pos);
                line.erase(0, pos + delimiter.length());
                auxArray[j] = stof(token);
                j++;

            }
            vertice *novo = new vertice();
            novo->x = auxArray[0];
            novo->y = auxArray[1];
            novo->z = auxArray[2];
            novo->espessura = auxArray[3];
            grupos[indexGrupoAtual].push_back(*novo);
        }
        myfile.close();
        //indexGrupoAtual++;
    }
    else cout << "Unable to open file";
}

void salvaModelo()
{
    FILE *arq;
    int result;

    char nomeArquivo[100];

    printf("Digite o nome do arquivo para salvar o grupo atual: \n");
    cin >> nomeArquivo;

    strcat(nomeArquivo,".txt");
    arq = fopen(nomeArquivo, "wt");  /// Cria um arquivo texto para gravação

    for(list<vertice>::iterator it = grupos[indexGrupoAtual].begin(); it != grupos[indexGrupoAtual].end(); it++)
    {
        /// formato: x y z espessura
        result = fprintf(arq,"%f %f %f %f \n",it->x, it->y, it->z, it->espessura);
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
    glPointSize(10.0);
    glEnable(GL_POINT_SMOOTH);
    glBegin(GL_POINTS);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(x, y, 0.0);
    glEnd();
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
    }
    else{
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
        glutSetCursor(GLUT_CURSOR_NONE);
        glMatrixMode (GL_MODELVIEW);
        glPushMatrix();
        g_camera.Refresh();
        //Desenha objeto 3D
        scene();
        glPopMatrix();

        glutSwapBuffers();
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

    case 'z':
        salvaModelo();
        break;
    case 'l':
        carregaModelo();
        break;
    case 'm':
        viewPort = !viewPort;
        g_camera.SetPos(-10.0,3.0,0.0);
        break;
    case 'b':
        boostSpeed = !boostSpeed;
        (boostSpeed) ? g_translation_speed = 0.5 : g_translation_speed = 0.5;
        boostSpeed ? printf("BoostMode ON\n") : printf("BoostMode OFF\n");
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

        if(dx) g_camera.RotateYaw(g_rotation_speed*dx);
        if(dy) g_camera.RotatePitch(g_rotation_speed*dy);

        if(!releaseMouse)	glutWarpPointer(g_viewport_width/2, g_viewport_height/2);

        just_warped = true;
    }
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
            if(x < width/2)
                calculaCoordenadasPonto(x,y);
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
    printf("Opcoes de interação:\n");
    printf("LEFT(DIRECIONAL) Retorna ao grupo anterior. \n");
    printf("RIGHT(DIRECIONAL) Avança para o proximo grupo. \n");
    printf("UP(DIRECIONAL) Aumenta altura do ponto a ser criado. \n");
    printf("DOWN(DIRECIONAL) Diminui altura do ponto a ser criado. \n");
    printf("F12 Modo Fullscreen. \n");
    printf("s Salvar o modelo. \n");
    printf("l Carregar modelo.\n");
    printf(". Aumenta espessura do modelo gerado.\n");
    printf(", Diminui espessura do modelo gerado.\n");
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

