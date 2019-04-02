/*
  Name:        CameraRotation.cpp
  Copyright:   Version 0.1
  Author:      Rodrigo Luis de Souza da Silva
  Date:        10/11/2014
  Description: Código base para simular a rotação da câmera em volta de um objeto posicionado na origem.
*/

#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>

int window;
float rotationX = 0.0, rotationY = 0.0;
int   last_x, last_y;
int width = 600, height = 600;
float m_yaw, m_pitch;
float dist = 30.0f;
bool yawLock = false;
bool pitchLock = false;
char title[128] = "Camera Rotation";

// Initial Position
float EyeInitial[3] = {dist, dist, dist};
float upInitial[3]  = {0.0f, 1.0f, 0.0f};
float foc[3]        = {0.0f, 0.0f, 0.0f};

// Rotated Position
float eye[3];
float up[3];

void changeWindowTitle(char *name)
{
    glutSetWindow(window); // Set main window as current
    glutSetWindowTitle(name); // change title
}

void init(void)
{
    glClearColor (0.2, 0.2, 0.2, 0.0);
    glShadeModel (GL_SMOOTH);

    glEnable(GL_LIGHT0);                   // habilita luz 0
    glEnable(GL_COLOR_MATERIAL);           // Utiliza cor do objeto como material
    glColorMaterial(GL_FRONT, GL_DIFFUSE);

    glEnable(GL_LIGHTING);                 // Habilita luz
    glEnable(GL_DEPTH_TEST);               // Habilita Z-buffer
    glEnable(GL_CULL_FACE);                // Habilita Backface-Culling
    glEnable(GL_NORMALIZE);
}

void drawScene()
{
    glPushMatrix();
        glColor3f(0.7f, 0.7f, 0.7f);
        glScalef(30.0f, 1.0f, 30.0f);
        glutSolidCube(1.0);
    glPopMatrix();

    glColor3f (1.0f, 0.0, 0.0);
    int inner = 4;
    glTranslatef(0.0f, inner, 0.0f);
    glRotatef(90.0f, 1, 0, 0);
    glutSolidTorus(inner, inner*2, 40, 40);
}

void RotatePoint(float *in, float *out, float pitch, float yaw)
{
    float xt = in[0], yt = in[1], zt = in[2];
    float x, y, z;

    // Rotation in 'y' axis
    x = zt * sin(yaw) + xt * cos(yaw);
    y = yt;
    z = zt * cos(yaw) - xt * sin(yaw);

    // Rotation in 'x' axis
    out[0] = x;
    out[1] = y * cos(pitch) - z * sin(pitch);
    out[2] = y * sin(pitch) + z * cos(pitch);
}

void RotateCamera()
{
    float r_pitch = M_PI * m_pitch / 180;
    float r_yaw   = M_PI * m_yaw / 180;

    // Rotate eye vector
    RotatePoint(EyeInitial, eye, r_pitch, r_yaw);

    // Rotate up vector
    RotatePoint(upInitial, up, r_pitch, r_yaw);
}

void display(void)
{
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(60.0, (GLfloat) width/(GLfloat) height, 1.0, 200.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity ();

    RotateCamera();
    gluLookAt (eye[0], eye[1], eye[2],
               foc[0], foc[1], foc[2],
                up[0],  up[1],  up[2]);

    drawScene();

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
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
}

void keyboard (unsigned char key, int x, int y)
{
    switch (tolower(key))
    {
        case '+' :
            dist++;
            if(dist>100) dist=100;
            EyeInitial[0]=EyeInitial[1]=EyeInitial[2]=dist;

            sprintf(title, "Camera Rotation - Camera Distance: %.2f", dist);
            changeWindowTitle(title);
            break;
        case '-' :
            dist--;
            if(dist<20) dist=20;

            EyeInitial[0]=EyeInitial[1]=EyeInitial[2]=dist;

            sprintf(title, "Camera Rotation - Camera Distance: %.2f", dist);
            changeWindowTitle(title);
            break;

        case 'y' :
            if(yawLock)
            {
                yawLock = false;
                changeWindowTitle((char *) "Camera Rotation - Unlocking Yaw");
            }
            else
            {
                yawLock = true;
                changeWindowTitle((char *) "Camera Rotation - Locking Yaw");
            }
            break;

        case 'p' :
            if(pitchLock)
            {
                pitchLock = false;
                changeWindowTitle((char *) "Camera Rotation - Unlocking Pitch");
            }
            else
            {
                pitchLock = true;
                changeWindowTitle((char *) "Camera Rotation - Locking Pitch");
            }
            break;

        case 27:
            exit(0);
            break;
    }
}

void fixAngle(float *angle)
{
    if(*angle > 360) *angle = 1.0f;
    if(*angle < 0)   *angle = 359.0f;
}

// Motion callback
void motion(int x, int y )
{
    if(!yawLock) 	rotationX += (float) (x - last_x);
    if(!pitchLock)	rotationY += (float) (y - last_y);

    last_x = x;
    last_y = y;

    fixAngle(&rotationX);
    fixAngle(&rotationY);

    m_yaw   = rotationX;
    m_pitch = rotationY;

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

void showMenu()
{
    printf("Camera Rotation\n");
    printf("---------------\n\n");

    printf("Mouse: \n");
    printf("* Left click and move horizontally to yaw (rotate over the y axis)\n");
    printf("* Left click and move vertically to pitch (rotate over the x axis)\n\n");

    printf("Keyboard: \n");
    printf("* Press '+' or '-' to increase/decrease camera distance\n");
    printf("* Press 'y' to lock/unlock yaw movement\n");
    printf("* Press 'p' to lock/unlock pitch movement\n");
    printf("* Press 'ESC' to quit\n\n");
}

int main(int argc, char** argv)
{
    showMenu();
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (width, height);
    glutInitWindowPosition (100, 100);
    window = glutCreateWindow (title);
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
