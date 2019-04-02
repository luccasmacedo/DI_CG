#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include "glcText.h"

using namespace std;

//######################################################################
//######################################################################
void init(void)
{
    glClearColor (0.0, 0.0, 0.0, 0.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 600, 200, 0.0);
}
//######################################################################
//######################################################################
void display(void)
{
    glClear (GL_COLOR_BUFFER_BIT);

    glcText t1;

    t1.setAll((char *) "OpenGLinfo", 8, 10, 20,1.0,0.6,0.0  );

    t1.setAll((char *) "Fornecedor   :", 5, 10, 40, 1.0,0.6,0.0 );
    t1.setAll((char *) "Versao       :", 5, 10, 60, 1.0,0.6,0.0  );
    t1.setAll((char *) "Adaptador    :", 5, 10, 80, 1.0,0.6,0.0  );
    t1.setAll((char *) "GLSL Version :", 5, 10, 100,1.0,0.6,0.0  );

    t1.setAll((char * )glGetString(GL_VENDOR), 5, 150, 40,1.0,1.0,1.0  );
    t1.setAll((char * )glGetString(GL_VERSION), 5, 150, 60,1.0,1.0,1.0  );
    t1.setAll((char * )glGetString(GL_RENDERER), 5, 150, 80,1.0,1.0,1.0  );
    t1.setAll((char * )glGetString(GL_SHADING_LANGUAGE_VERSION), 5, 150, 100,1.0,1.0,1.0  );

    glFlush ();
}
//######################################################################
//######################################################################
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize (600, 200);
    glutInitWindowPosition (100, 100);
    glutCreateWindow (argv[0]);
    init ();
    glutDisplayFunc(display);
    glutMainLoop();

    //system("pause");
    return 0;
}
//######################################################################
//######################################################################
