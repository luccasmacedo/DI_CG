/*
  OBJ Viewer with Texture

  Author:       Rodrigo Luis de Souza da Silva
  Date:         23/11/2017
  Last Update:  24/05/2018
  Description:  View OBJ files with an external texture (PNG) reader.

  Some repositories:
    https://www.cs.cmu.edu/~kmcrane/Projects/ModelRepository/
*/


#include <GL/glut.h>
#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "glcWavefrontObject.h"
#include "glcTexture.h"
#define NUM_OBJECTS 7

using namespace std;

char objectFiles[NUM_OBJECTS][50] =
{
    "../data/texturedObj/littleCow.obj",
    "../data/texturedObj/duck.obj",
    "../data/texturedObj/fish.obj",
    "../data/texturedObj/L200.obj",
    "../data/texturedObj/plane.obj",
    "../data/texturedObj/Florest_Guardian.obj",
    "../data/texturedObj/BlueDragon.obj"
};

char textureFiles[NUM_OBJECTS][50] =
{
    "../data/texturedObj/littleCow.png",
    "../data/texturedObj/duck.png",
    "../data/texturedObj/fish.png",
    "../data/texturedObj/L200.png",
    "../data/texturedObj/plane.png",
    "../data/texturedObj/Florest_Guardian.png",
    "../data/texturedObj/BlueDragon.png"
};

// Managers for texture and objects
glcTexture *textureManager = NULL;
glcWavefrontObject *objectManager = NULL;
bool fullScreen = false;

// Globals
int selected = 0;
float dist = 2.0;
int width  = 1024;
int height = 768;

float rotationX = 135.0, rotationY = 15;
int   last_x, last_y;
bool drawboundingbox = false;
bool drawaxes = false;

// Menu
void showMenu()
{
    cout << "OBJECT VIEWER + TEXTURES" << endl;
    cout << "------------------------\n" << endl;
    cout << "Mouse" << endl;
    cout << "* Movimenta o objeto e scroll para zoom." << endl;
    cout << "Teclado" << endl;
    cout << "* Setas para direita ou esquerda para mudar objeto." << endl;
    cout << "* 'b' para desenhar bounding box" << endl;
    cout << "* 'a' para deixar eixos" << endl;
    cout << "* F11 - Habilita/Desabilita modo fullscreen." << endl;
    cout << "* ESC - Para sair" << endl;
}

//Reshape Func
void reshape( int w, int h)
{
    width = w;
    height = h;
    glViewport(0,0,w,h);
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
            exit(0);
            break;
        case 'b':
            drawboundingbox=!drawboundingbox;
            break;
        case 'a':
            drawaxes=!drawaxes;
            break;
    }
    glutPostRedisplay();
}


void specialKeys(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_F11:
            (!fullScreen) ? glutFullScreen() : glutReshapeWindow(1024, 768);
            //glutPositionWindow(100, 100);
            fullScreen = !fullScreen;
            break;
        case GLUT_KEY_RIGHT:
            selected++;
            if(selected >= NUM_OBJECTS)
                selected = 0;
            break;
        case GLUT_KEY_LEFT:
            selected--;
            if(selected < 0)
                selected = NUM_OBJECTS-1;
            break;
    }
    glutPostRedisplay();
}

// Motion callback
void motion(int x, int y )
{
    rotationY += (float) (y - last_y);
    rotationX += (float) (x - last_x);

    last_x = x;
    last_y = y;

    glutPostRedisplay();
}

// Mouse callback
void mouse(int button, int state, int x, int y)
{
    if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
    {
        last_x = x;
        last_y = y;
    }
    if(button == 3) // Scroll up
    {
        if(dist>=1.0)  dist-=0.25;
    }
    if(button==4)   // Scroll down
    {
        if(dist<=20.0) dist+=0.25;
    }
}

void drawAxes()
{
    int size = 20.0f;

    glDisable(GL_LIGHTING);   // Disabling Lighting
    glDisable(GL_TEXTURE_2D); // Disabling texture

    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f (-size, 0.0, 0.0);
        glVertex3f ( size, 0.0, 0.0);
        glColor3f(0.0, 1.0, 0.0);
        glVertex3f (0.0, -size, 0.0);
        glVertex3f (0.0,  size, 0.0);
        glColor3f(0.0, 0.0, 1.0);
        glVertex3f (0.0, 0.0, -size);
        glVertex3f (0.0, 0.0,  size);
    glEnd();

    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D); // Disabling texture
}

void display(void)
{
    // Limpar todos os pixels
    glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    // Sistema de Projeção
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0f,(GLfloat)width/(GLfloat)height,0.1f,200.0f);
    gluLookAt (0.0, 0.0, dist, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // Sistema de Visualização
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Movimenta o objeto e camera fica parada
    glRotatef( rotationY, 1.0, 0.0, 0.0 );
    glRotatef( rotationX, 0.0, 1.0, 0.0 );

    // Use selected texture
    textureManager->Bind(selected);

    // Use selected Object
    //objectManager->SetColor(1.0, 0.0, 0.0);
    objectManager->SelectObject(selected);
    objectManager->SetShadingMode(SMOOTH_SHADING); // Alternative: FLAT_SHADING
    objectManager->SetRenderMode(USE_TEXTURE_AND_MATERIAL);
    objectManager->Unitize();
    objectManager->Draw();
    if(drawboundingbox) objectManager->DrawBoundingBox();
    if(drawaxes)        drawAxes();

    char title[50];
    sprintf(title, "Vertices: %d / Triangles: %d", objectManager->GetNumberOfVertices(), objectManager->GetNumberOfTriangles());
    glutSetWindowTitle(title);

    glutSwapBuffers ();
    glutPostRedisplay();
}


void init (void)
{
    glClearColor (0.6, 0.6, 0.6, 0.0);
    glShadeModel (GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.2);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Cor da fonte de luz (RGBA)
    GLfloat cor_luz[]     = { 1.0, 1.0, 1.0, 1.0};
    GLfloat posicao_luz[] = { 0.0, 0.0, dist, 1.0};

    // Define parametros da luz
    glLightfv(GL_LIGHT0, GL_AMBIENT, cor_luz);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, cor_luz);
    glLightfv(GL_LIGHT0, GL_SPECULAR, cor_luz);
    glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz);

    // LOAD TEXTURES
    textureManager = new glcTexture();
    textureManager->SetNumberOfTextures(NUM_OBJECTS);
    textureManager->SetWrappingMode(GL_REPEAT);
    for(int i = 0; i < NUM_OBJECTS; i++)
        textureManager->CreateTexture( textureFiles[i], i);

    // LOAD OBJECTS
    objectManager = new glcWavefrontObject();
    objectManager->SetNumberOfObjects(NUM_OBJECTS);
    for(int i = 0; i < NUM_OBJECTS; i++)
    {
        objectManager->SelectObject(i);
        objectManager->ReadObject(objectFiles[i]);
        objectManager->Unitize();
        objectManager->FacetNormal();
        objectManager->VertexNormals(90.0);
        objectManager->Scale(5);
    }
}


int main(int argc, char** argv)
{
    showMenu();
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
    glutInitWindowSize (width, height);
    glutCreateWindow ("Object Viewer + Texture");
    init ();
    glutMouseFunc( mouse );
    glutKeyboardFunc(keyboard);
    glutMotionFunc( motion );
    glutReshapeFunc( reshape );
    glutDisplayFunc(display);
    glutSpecialFunc( specialKeys );
    glutMainLoop();

    return 0;
}

