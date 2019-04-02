/*
  Name:        SpotlightExample
  Author:      Rodrigo Luis de Souza da Silva
  Last Update: 30/06/2016
  Date:        30/06/2016
  Description: Exemplo de iluminação através de spotlights.
*/

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

float quality 		= 10;
int width         = 800;
int height        = 800;

float rotation = 0.0;
int   last_x, last_y;
float keyStep = 0.25;
float moveX = 0;
float moveY = 0;

GLfloat ambient[]   = { 0.3, 0.3, 0.3, 1.0 };
GLfloat emissao[] =   { 0.0, 0.0, 0.0, 0.0 };
GLfloat brilho[]    = { 128.0 };

// Material da base (amarelo)
GLfloat base_difusa[]    = { 1.0, 1.0, 0.0, 1.0 };
GLfloat base_especular[] = { 1.0, 1.0, 0.0, 1.0 };

// Material dos objetos a serem iluminados
GLfloat object_difusa[]    = { 1.0, 0.65, 0.2, 1.0 };
GLfloat object_especular[] = { 1.0, 0.65, 0.2, 1.0 };

// Material da lanterna
GLfloat lanterna1[] = { 1.0, 0.0, 0.0, 1.0 }; // corpo
GLfloat lanterna2[] = { 1.0, 1.0, 1.0, 1.0 }; // ponta

// Defini cor da luz ambiente
GLfloat cor_luz_amb[]      = { 0.1, 0.1, 0.1, 1.0};

// Especificação da luz do spotlight
GLfloat cor_luz_ativa[]    = { 1.0, 0.0, 0.0, 1.0};
GLfloat posicao_luz0[]     = { 0.0, 0.0, 0.0, 1.0};
GLfloat cor_luz0[]         = { 1.0, 1.0, 1.0, 1.0};
GLfloat spotDir[3] 			= { 0.0, 1.0, 0.0 };
GLfloat cutoffAngle = 45.0f;

// Especificação da luz que iluminará a lanterna
GLfloat cor_luz1[]         = { 0.5, 0.5, 0.5, 1.0};

void init(void)
{
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_NORMALIZE);

    // Define parametros do spotlight
    glLightfv(GL_LIGHT0, GL_AMBIENT, cor_luz_amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, cor_luz0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, cor_luz0);
    glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz0);
    glLightf(GL_LIGHT0,GL_SPOT_CUTOFF, cutoffAngle);
    glLightf(GL_LIGHT0,GL_SPOT_EXPONENT, 1.0);

    // Define parametros da luz que iluminará a lanterna
    glLightfv(GL_LIGHT1, GL_DIFFUSE, cor_luz0);
    glLightfv(GL_LIGHT1, GL_SPECULAR, cor_luz0);

    printf("\nMenu");
    printf("\n====\n\n");
    printf("  Press 'w', 'a', 's', 'd' to move the flashlight.\n");
    printf("  Press '+' and '-' to increase/decrease scene quality.\n");
    printf("  Use the mouse to rotate the flashlight.\n");
    printf("  Use mouse scroll to control spolight cutoff.\n");
    printf("  Press ESC to exit sair.\n");
}

void setMaterial(int id)
{
    glMaterialfv(GL_FRONT, GL_EMISSION, emissao);
    glMaterialfv(GL_FRONT, GL_SHININESS, brilho);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);

    switch(id)
    {
        case 0:
            glMaterialfv(GL_FRONT, GL_DIFFUSE, base_difusa);
            glMaterialfv(GL_FRONT, GL_SPECULAR, base_especular);
            break;
        case 1:
            glMaterialfv(GL_FRONT, GL_DIFFUSE, object_difusa);
            glMaterialfv(GL_FRONT, GL_SPECULAR, object_especular);
            break;
        case 2:
            glMaterialfv(GL_FRONT, GL_DIFFUSE, lanterna1);
            glMaterialfv(GL_FRONT, GL_SPECULAR, lanterna1);
            break;
        case 3:
            glMaterialfv(GL_FRONT, GL_DIFFUSE, lanterna2);
            glMaterialfv(GL_FRONT, GL_SPECULAR, lanterna2);
            break;
    }
}

void geraCenario()
{
    float i, j, sx = 20, sy = 20;

    // Yellow Spheres
    setMaterial(0);
    float resolution = 20 / quality;
    for(i = -sx; i < sx; i+=resolution)
    {
        for(j = -sy; j < sy; j+=resolution)
        {
            glPushMatrix();
            glTranslatef(i, j, 0.0);
            glScalef(1.0, 1.0, 2.0 / resolution);
            glutSolidSphere(resolution, 10, 10);
            glPopMatrix();
        }
    }

    // Objects around
    int objectSize = 4;
    int objectResolution = quality * 20;
    if(quality < 10) objectResolution = quality * 3;

    setMaterial(1);
    glPushMatrix();
        glTranslatef(sx-objectSize, sy-objectSize, objectSize);
        glutSolidSphere(objectSize, objectResolution, objectResolution);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-sx+objectSize+2, sy-objectSize-2, objectSize/2.0);
        glutSolidCone(objectSize, objectSize*2, objectResolution, objectResolution);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-sx+objectSize, -sy/3.0, objectSize);
        glutSolidTorus(objectSize/4.0, objectSize/4.0+1, objectResolution, objectResolution);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(sx-objectSize, -sy/3.0, objectSize);
        glRotatef(90, 1.0, 0.0, 0.0);
        glutSolidTeapot(objectSize);
    glPopMatrix();
}

void desenhaLanterna()
{
    glPushMatrix();
        setMaterial(2);
        glTranslatef(0.0, -1.0, 0.0);
        glutSolidCube(2.0);
        glTranslatef(0.0, 2.0, 0.0);
        glRotatef(90.0, 1.0, 0.0, 0.0);
        setMaterial(3);
        glutSolidCone(1.0, 3.0, 10, 10);
    glPopMatrix();
}

void display(void)
{
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(60.0, (GLfloat) width/(GLfloat) height, 1.0, 200.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity ();
    gluLookAt (0.0, -30.0, 25.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);

    posicao_luz0[0]+=moveX;
    posicao_luz0[1]+=moveY;

    // Posiciona lanterna
    glPushMatrix();
        glEnable(GL_LIGHT1);
        glTranslatef(posicao_luz0[0],posicao_luz0[1],posicao_luz0[2]+3);
        glRotatef( -rotation, 0.0, 0.0, 1.0 );
        glMaterialfv(GL_FRONT, GL_EMISSION, cor_luz_ativa);
        desenhaLanterna();
        glDisable(GL_LIGHT1);
    glPopMatrix();

    // Posiciona spotlight
    glPushMatrix();
        glTranslatef(posicao_luz0[0],posicao_luz0[1],posicao_luz0[2]+3);
        glRotatef( -rotation, 0.0, 0.0, 1.0 );
        glTranslatef(-posicao_luz0[0], -posicao_luz0[1], -posicao_luz0[2]);
        glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz0);
        glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,spotDir);
        glLightf(GL_LIGHT0,GL_SPOT_CUTOFF, cutoffAngle);
    glPopMatrix();

    geraCenario();

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

void keyboardPress (unsigned char key, int x, int y)
{
    float speed = 0.25;
    switch (tolower(key))
    {
        case 'w':
            moveY = speed;
            break;
        case 's':
            moveY = -speed;
            break;
        case 'd':
            moveX = speed;
            break;
        case 'a':
            moveX = -speed;
            break;
        case '+':
            if(quality<10)
                quality++;
            break;
        case '-':
            if(quality>5)
                quality--;
            break;
        case 27:
            exit(0);
            break;
    }
}


// Keyboard callback
void keyboardRelease(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'w' :
        case 's' :
            moveY = 0;
            break;
        case 'd' :
        case 'a' :
            moveX = 0;
            break;
    }
}
// Motion callback
void motion(int x, int y )
{
    rotation += (float) (x - last_x);

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
    if(button == 3) // Scroll up
    {
        if(cutoffAngle<=90.0) cutoffAngle+=0.5;
    }
    if(button == 4) // Scroll Down
    {
        if(cutoffAngle>=20.0) cutoffAngle-=0.5;
        printf("cutoff: %.2f\n", cutoffAngle);
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (width, height);
    glutInitWindowPosition (50, 50);
    glutCreateWindow ("Spotlight Example");
    init ();
    glutIgnoreKeyRepeat(1);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc( keyboardPress );
    glutKeyboardUpFunc( keyboardRelease );
    glutMouseFunc( mouse );
    glutMotionFunc( motion );
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
