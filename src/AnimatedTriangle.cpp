/*
  Name:         triangle_anim.cpp
  Copyright:    Version 0.1
  Author:       Rodrigo Luis de Souza da Silva
  Last Update:  29/11/2018 (Animation based on FPS)
  Release:      28/10/2004
  Description:  Animação de triângulo usando função idle.
                Update: visualização e escolhe do FPS desejado
*/

#include <iostream>
#include <GL/glut.h>
#include <glcFPSViewer.h>

using namespace std;

float desiredFPS = 60;
float var = 0.0f;
float dir = 1.0f;
glcFPSViewer *fpsViewer = new glcFPSViewer((char*) "Triangle Animation. ", (char*) " - Press ESC to Exit");

void display(void);
void init (void);
void idle();
void mouse(int button, int state, int x, int y);
void keyboard(unsigned char key, int x, int y);

void idle()
{
    float t, desiredFrameTime, frameTime;
    static float tLast = 0.0;

    // Get elapsed time
    t = glutGet(GLUT_ELAPSED_TIME);
    // convert milliseconds to seconds
    t /= 1000.0;

    // Calculate frame time
    frameTime = t - tLast;
    // Calculate desired frame time
    desiredFrameTime = 1.0 / (float) (desiredFPS);

    // Check if the desired frame time was achieved. If not, skip animation.
    if( frameTime <= desiredFrameTime)
        return;

    // **  UPDATE ANIMATION VARIABLES ** //
    float step = 1; // Speed of the animation
    if(var <= -0.001 || var >= 8.0)
        dir *= -1;
    var+=dir* (step / desiredFPS); // Variation computed considering the FPS

    // Update tLast for next time, using static local variable
    tLast = t;

    glutPostRedisplay();

}

void display(void)
{
    // Limpar todos os pixels
    glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glColor3f (1.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex2f (0.0, 0.0);
    glVertex2f (8.0, 0.0);
    glColor3f (1.0, 1.0, 0.0);
    glVertex2f (0.0, var);
    glEnd();
    glutSwapBuffers ();

    // Print FPS
    fpsViewer->drawFPS();
}

void init (void)
{
    // selecionar cor de fundo (preto)
    glClearColor (0.0, 0.0, 0.0, 0.0);

    // inicializar sistema de viz.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 8.0, 0.0, 8.0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case '1':
            desiredFPS = 60;
            break;
        case '2':
            desiredFPS = 30;
            break;
        case '3':
            desiredFPS = 10;
            break;
        case 27:
            exit(0);
            break;
    }
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    cout << "Triangle Animation Example" << endl;
    cout << "--------------------------" << endl;
    cout << "Press 1 to change FPS to ~60" << endl;
    cout << "Press 2 to change FPS to ~30" << endl;
    cout << "Press 3 to change FPS to ~10" << endl;
    cout << "Press ESC to exit" << endl;
    glutInitDisplayMode (GLUT_DOUBLE|GLUT_DEPTH|GLUT_RGB);
    glutInitWindowSize (400, 400);
    glutCreateWindow ("");
    glutKeyboardFunc(keyboard);
    glutIdleFunc( idle);
    init ();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
