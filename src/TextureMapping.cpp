#include <GL/glut.h>
#include <GL/glext.h>
#include <iostream>
#include "glcTexture.h"

#define COLOR_RUBY    1
#define COLOR_DEFAULT 2


using namespace std;

// Aqui é criada a referência ao objeto que gerenciará as texturas
glcTexture *textureManager;
int selected = 0;
int object = 1;
float width, height;
float zoom = 1.8f;
float pan[2]  = {0.0f, 0.0f};
bool fullScreen = false;
float rotationX = 0.0, rotationY = 0.0;
int   last_x, last_y;
int window;

bool wrappingMode = 1;
bool magMode = 1;
bool minMode = 1;
int colorMode = 0;
float tiles = 1.0;

// Usado no colorMode = GL_MODULATE
void setMaterial(int op)
{
    if(op == COLOR_RUBY)
    {
        // Material do objeto (neste caso, ruby)
        GLfloat objeto_ambient[]   = { 0.2, 0.0, 0.0, 1.0 };
        GLfloat objeto_difusa[]    = { 0.6, 0.0, 0.0, 1.0 };
        GLfloat objeto_especular[] = { 0.7, 0.6, 0.6, 1.0 };
        GLfloat objeto_brilho[]    = { 90.0f };

        glMaterialfv(GL_FRONT, GL_AMBIENT, objeto_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, objeto_difusa);
        glMaterialfv(GL_FRONT, GL_SPECULAR, objeto_especular);
        glMaterialfv(GL_FRONT, GL_SHININESS, objeto_brilho);
    }
    if(op == COLOR_DEFAULT)
    {
        // Set colors to the default (according to https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glMaterial.xml)
        GLfloat objeto_ambient[]   = { 0.2, 0.2, 0.2, 1.0 };
        GLfloat objeto_difusa[]    = { 0.8, 0.8, 0.8, 1.0 };
        GLfloat objeto_especular[] = { 0.0, 0.0, 0.0, 1.0 };

        glMaterialfv(GL_FRONT, GL_AMBIENT, objeto_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, objeto_difusa);
        glMaterialfv(GL_FRONT, GL_SPECULAR, objeto_especular);
    }
}

// Como as imagens são lidas a partir do canto superior direito, temos que inverter as coordenadas 'u' e 'v'
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, width/height, 0.1, 100);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(pan[0], pan[1], zoom, pan[0], pan[1], 0.0, 0.0, 1.0, 0.0);
    // Seleciona a textura corrente
    textureManager->Bind(selected);
    float aspectRatio = textureManager->GetAspectRatio(selected);

    // Calculo abaixo funciona apenas se textura estiver centralizada na origem
    float h = 1.0f;
    float w = 1.0f;

    if( aspectRatio > 1.0f) h  = h/aspectRatio;
    else                    w  = w*aspectRatio;

    glPushMatrix();
        glRotatef( rotationY, 0.0, 1.0, 0.0 );
        glRotatef( rotationX, 1.0, 0.0, 0.0 );

        if(object == 1)
        {
            glBegin(GL_QUADS);
                glNormal3f(0.0, 0.0, 1.0);
                glTexCoord2f(0.0, 0.0);
                glVertex3f(-w, -h, 0.0);

                glTexCoord2f(tiles, 0.0);
                glVertex3f(w, -h, 0.0);

                glTexCoord2f(tiles, tiles);
                glVertex3f(w, h, 0.0);

                glTexCoord2f(0.0, tiles);
                glVertex3f(-w, h, 0.0);
            glEnd();
        }
        else // object == 2
        {
            glPushMatrix();
                glRotatef( 30, 1.0, 0.0, 0.0 ); // Put the teapot in a better position
                glutSolidTeapot(0.7);
            glPopMatrix();
        }
    glPopMatrix();

    glutSwapBuffers();

    // Desabilita o uso de texturas
    textureManager->Disable();
}

void resizeWindow(int w, int h)
{
    width = w;
    height = h;
    glViewport(0, 0, w, h);
}

void changeWindowTitle(char* name)
{
    glutSetWindow(window); // Set main window as current
    glutSetWindowTitle(name); // change title
}

void keyboard (unsigned char key, int x, int y)
{
    if(isdigit(key))
    {
        int val = atoi((const char *) &key);
        if(val > 0 && val <= textureManager->GetNumberOfTextures() )
            selected = val-1;
    }
    switch (key)
    {
        case 27:
            exit(0);
            break;
        case '+':
            if(zoom>=0.3)
                zoom-=0.1f;
            break;
        case '-':
            if(zoom<=20.0)
                zoom+=0.1f;
            break;
        case 'w': // wrapping modes
            wrappingMode = !wrappingMode;
            if(wrappingMode)
            {
                textureManager->SetWrappingMode(GL_REPEAT);
                changeWindowTitle( (char *) "Wrapping Mode = GL_REPEAT");
            }
            else
            {
                textureManager->SetWrappingMode(GL_CLAMP);
                changeWindowTitle( (char *) "Wrapping Mode = GL_CLAMP");
            }
            textureManager->Update();
            break;
        case 'm': // minificação
            minMode = !minMode;
            if(minMode)
            {
                textureManager->SetMinFilterMode(GL_LINEAR);
                changeWindowTitle( (char *) "Minification Mode = GL_LINEAR");
            }
            else
            {
                textureManager->SetMinFilterMode(GL_NEAREST);
                changeWindowTitle( (char *) "Minification Mode = GL_NEAREST");
            }
            textureManager->Update();
            break;
        case 'g': // magnificação
            magMode = !magMode;
            if(magMode)
            {
                textureManager->SetMagFilterMode(GL_NEAREST);
                changeWindowTitle( (char *) "Magnification Mode = GL_NEAREST");
            }
            else
            {
                textureManager->SetMagFilterMode(GL_LINEAR);
                changeWindowTitle( (char *) "Magnification Mode = GL_LINEAR");
            }
            textureManager->Update();
            break;
        case 'c': // Color Mode
            colorMode++;
            if(colorMode > 3)
                colorMode = 0;
            switch(colorMode)
            {
                case 0:
                    setMaterial(COLOR_DEFAULT);
                    textureManager->SetColorMode(GL_MODULATE);
                    changeWindowTitle( (char *) "Color Mode = GL_MODULATE (Default color)");
                    break;
                case 1:
                    setMaterial(COLOR_RUBY);
                    textureManager->SetColorMode(GL_MODULATE);
                    changeWindowTitle( (char *) "Color Mode = GL_MODULATE (Ruby color)");
                    break;
                case 2:
                    textureManager->SetColorMode(GL_REPLACE);
                    changeWindowTitle( (char *) "Color Mode = GL_REPLACE");
                    break;
                case 3:
                    float env_color[] = {0.2, 0.8, 0.0, 1.0};
                    setMaterial(COLOR_DEFAULT);
                    textureManager->SetEnvColor(env_color);
                    textureManager->SetColorMode(GL_BLEND);
                    changeWindowTitle( (char *) "Color Mode = GL_BLEND");
                    break;
            }
            textureManager->Update();
            break;
        case 't': // Increase Texture Tiles
            tiles++;
            break;
        case 'T': // Decrease Texture Tiles
            if(tiles>1.0f)
                tiles--;
            break;
    }
    glutPostRedisplay();
}

void specialKeys(int key, int x, int y)
{
    float f = 0.05;
    switch(key)
    {
        case GLUT_KEY_UP:
            pan[1]+=f;
            break;
        case GLUT_KEY_DOWN:
            pan[1]-=f;
            break;
        case GLUT_KEY_LEFT:
            pan[0]-=f;
            break;
        case GLUT_KEY_RIGHT:
            pan[0]+=f;
            break;
        case GLUT_KEY_F11:
            (!fullScreen) ? glutFullScreen() : glutReshapeWindow(800, 600);
            fullScreen = !fullScreen;
            break;
        case GLUT_KEY_F1:
            object = 1;
            break;
        case GLUT_KEY_F2:
            object = 2;
            break;
    }
    glutPostRedisplay();
}


void init(void)
{
    glClearColor (0.5, 0.5, 0.5, 0.0);
    glShadeModel (GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);               // Habilita Z-buffer
    glEnable(GL_LIGHTING);                 // Habilita luz
    glEnable(GL_LIGHT0);                   // habilita luz 0

    glEnable(GL_ALPHA_TEST);      // O alpha test descarta fragmentos dependendo de uma comparação (abaixo)
    glAlphaFunc(GL_GREATER, 0.1); // Info: https://www.opengl.org/sdk/docs/man2/xhtml/glAlphaFunc.xml

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // https://www.opengl.org/sdk/docs/man/html/glBlendFunc.xhtml

    // Cor da fonte de luz (RGBA)
    GLfloat cor_luz[]     = { 1.0, 1.0, 1.0, 1.0};
    GLfloat posicao_luz[] = { 0.0, 0.0, 10.0, 1.0};

    // Define parametros da luz
    glLightfv(GL_LIGHT0, GL_AMBIENT, cor_luz);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, cor_luz);
    glLightfv(GL_LIGHT0, GL_SPECULAR, cor_luz);
    glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz);


    // Essa é a principal parte deste exemplo
    textureManager = new glcTexture();            // Criação do arquivo que irá gerenciar as texturas
    textureManager->SetNumberOfTextures(4);       // Estabelece o número de texturas que será utilizado
    textureManager->CreateTexture("../data/marble.png", 0); // Para testar magnificação, usar a imagem marble128
    textureManager->CreateTexture("../data/carbon.png", 1); // Textura transparente, não sendo múltipla de zero
    textureManager->CreateTexture("../data/fishermen.png", 2); // Textura transparente, não sendo múltipla de zero
    textureManager->CreateTexture("../data/paper.png", 3);
}

// Motion callback//glcTexture *texture;
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
    if(button == 3) // Scroll up
    {
        if(zoom>=0.3)  zoom-=0.04f;
    }
    if(button==4)   // Scroll down
    {
        if(zoom<=20.0) zoom+=0.04f;
    }
}

void idle ()
{
    glutPostRedisplay();
}

void printOpenGLVersion()
{
    cout << "\nDados do OpenGL instalado neste sistema operacional:" << endl;
    char *vendor  = (char * ) glGetString(GL_VENDOR);
    char *version = (char * ) glGetString(GL_VERSION);
    char *render  = (char * ) glGetString(GL_RENDERER);
    cout << "   Vendor:   " << vendor   << endl;
    cout << "   Version:  " << version  << endl;
    cout << "   Renderer: " << render   << endl;
    if(version[0] == '1')
    {
        cout << "\nAviso: A biblioteca pode nao funcionar corretamente (versao antiga do OpenGL).";
        cout << "\n       Maiores detalhes nas notas do arquivo \"README_FIRST.txt\".\n";
    }
    cout << endl;
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1024, 768);
    glutInitWindowPosition(100, 100);
    window = glutCreateWindow(argv[0]);

    init();
    printOpenGLVersion();
    cout << "\n------------------------------------------------------------\n" << endl;
    cout << "Instrucoes: " << endl;
    cout << " * Modifique as texturas pressionando as teclas de um 1 a " << textureManager->GetNumberOfTextures() << ";" << endl;
    cout << " * Pressione F1 e F2 para mudar os objetos;" << endl;
    cout << " * Teclas '+' e '-' fazem zoom;" << endl;
    cout << " * Teclas 'w' para alternar entre os \"Wrapping modes\";" << endl;
    cout << " * Teclas 'm' para alternar entre os modos do filtro de minificação;" << endl;
    cout << " * Teclas 'g' para alternar entre os modos do filtro de magnificação;" << endl;
    cout << " * Teclas 'c' para alternar entre os modos de cor de textura;" << endl;
    cout << " * Teclas 't' ou 'T' para aumentar/diminuir o tamanho da coordenada de textura (em 2D);" << endl;
    cout << " * Direcionais do teclado movem a camera (pan);" << endl;
    cout << " * Mouse rotaciona o objeto;" << endl;
    cout << " * Pressione F11 para ligar e desligar modo FullScreen;" << endl;

    glutDisplayFunc(display);
    glutReshapeFunc(resizeWindow);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc( specialKeys );
    glutMouseFunc( mouse );
    glutMotionFunc( motion );
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
