/*
  Name:         extras.h
  Copyright:    Version 0.1
  Author:       Rodrigo Luis de Souza da Silva
  Last Update:	23/10/2018
  Date:         23/08/2016
  Description:  Funções com recursos a serem vistos posteriormente no curso
*/

void initLight(int width, int height)
{
    glEnable(GL_LIGHTING);                 // Habilita luz
    glEnable(GL_LIGHT0);                   // habilita luz 0

    // Cor da fonte de luz (RGBA)
    GLfloat cor_luz[]     = { 1.0, 1.0, 1.0, 1.0};
    // Posicao da fonte de luz. Ultimo parametro define se a luz sera direcional (0.0) ou tera uma posicional (1.0)
    GLfloat posicao_luz[] = { (float) width, (float)height, 1000.0, 1.0};

    // Define parametros da luz
    glLightfv(GL_LIGHT0, GL_AMBIENT, cor_luz);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, cor_luz);
    glLightfv(GL_LIGHT0, GL_SPECULAR, cor_luz);
    glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz);
}

void initLight(float pos[3])
{
    glEnable(GL_LIGHTING);                 // Habilita luz
    glEnable(GL_LIGHT0);                   // habilita luz 0

    // Cor da fonte de luz (RGBA)
    GLfloat cor_luz[]     = { 1.0, 1.0, 1.0, 1.0};
    // Posicao da fonte de luz. Ultimo parametro define se a luz sera direcional (0.0) ou tera uma posicional (1.0)
    GLfloat posicao_luz[] = { pos[0], pos[1], pos[2], 1.0};

    // Define parametros da luz
    glLightfv(GL_LIGHT0, GL_AMBIENT, cor_luz);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, cor_luz);
    glLightfv(GL_LIGHT0, GL_SPECULAR, cor_luz);
    glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz);
}

void setMaterialVermelho(void)
{
    // Material do objeto (neste caso, ruby). Parametros em RGBA
    GLfloat objeto_ambient[]   = { .1745, .01175, .01175, 1.0 };
    GLfloat objeto_difusa[]    = { .61424, .04136, .04136, 1.0 };
    GLfloat objeto_especular[] = { .727811, .626959, .626959, 1.0 };
    GLfloat objeto_brilho[]    = { 90.0f };

    // Define os parametros da superficie a ser iluminada
    glMaterialfv(GL_FRONT, GL_AMBIENT, objeto_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, objeto_difusa);
    glMaterialfv(GL_FRONT, GL_SPECULAR, objeto_especular);
    glMaterialfv(GL_FRONT, GL_SHININESS, objeto_brilho);
}

void setMaterialVerde(void)
{
    GLfloat objeto_ambient[]   = { 0.01, 0.17, 0.01, 1.0 };
    GLfloat objeto_difusa[]    = { 0.04, 0.61, 0.04, 1.0 };
    GLfloat objeto_especular[] = { 0.62, 0.82, 0.62, 1.0 };
    GLfloat objeto_brilho[]    = { 70.0f };

    // Define os parametros da superficie a ser iluminada
    glMaterialfv(GL_FRONT, GL_AMBIENT, objeto_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, objeto_difusa);
    glMaterialfv(GL_FRONT, GL_SPECULAR, objeto_especular);
    glMaterialfv(GL_FRONT, GL_SHININESS, objeto_brilho);
}

void setMaterialAmarelo(void)
{
    GLfloat objeto_ambient[]   = { 0.2, 0.2, 0.0, 1.0 };
    GLfloat objeto_difusa[]    = { 0.6, 0.6, 0.0, 1.0 };
    GLfloat objeto_especular[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat objeto_brilho[]    = { 110.0f };

    // Define os parametros da superficie a ser iluminada
    glMaterialfv(GL_FRONT, GL_AMBIENT, objeto_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, objeto_difusa);
    glMaterialfv(GL_FRONT, GL_SPECULAR, objeto_especular);
    glMaterialfv(GL_FRONT, GL_SHININESS, objeto_brilho);
}

// Default é vermelho
void setMaterial(void)
{
    setMaterialVermelho();
}
