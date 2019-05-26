#include "glcShader.h"
#include <iostream>
#include <string.h>

glcShader::~glcShader()
{
}

glcShader::glcShader()
{
    this->programHandler = 0;
}

glcShader::glcShader(string vs, string fs)
{
    this->InstallShaders(vs, fs);
}

void glcShader::ApplyShader()
{
    glUseProgram(this->programHandler);
}

void glcShader::DisableShader()
{
    glUseProgram(0);
}

void glcShader::PrintError(char *f)
{
    cout << "glcShader Error: File \"" << f << "\" not found.\n";
    exit(1);
}

//void glcShader::InstallShaders(string vs_in, string fs_in)
//{
//    char *vs = NULL;
//    char *fs = NULL;
//
//    string vsi;
//    vs = ReadShaderFile((char*) vs_in.c_str());
//    fs = ReadShaderFile((char*) fs_in.c_str());
//
//    const char * phongFragment = fs;
//    const char * phongVertex = vs;
//
//    GLuint phongVS, phongFS;    // handles to objects
//
//    // Create a vertex shader object and a fragment shader object
//    phongVS = glCreateShader(GL_VERTEX_SHADER);
//    phongFS = glCreateShader(GL_FRAGMENT_SHADER);
//
//    // Load source code strings into shaders
//    glShaderSource(phongVS, 1, &phongVertex, NULL);
//    glShaderSource(phongFS, 1, &phongFragment, NULL);
//
//    // Compile the phong vertex shader and print out the compiler log file.
//    glCompileShader(phongVS);
//    glCompileShader(phongFS);
//
//    this->programHandler = glCreateProgram();
//    glAttachShader(this->programHandler, phongVS);
//    glAttachShader(this->programHandler, phongFS);
//    glLinkProgram(this->programHandler);
//
//    // Install program object as part of current state
//    glUseProgram(this->programHandler);
//}

void glcShader::InstallShaders(string vs_in, string fs_in)
{
    char *vs = NULL;
    char *fs = NULL;

    string vsi;
    vs = ReadShaderFile((char*) vs_in.c_str());
    fs = ReadShaderFile((char*) fs_in.c_str());

    const char * phongFragment = fs;
    const char * phongVertex = vs;

    GLuint phongVS, phongFS;    // handles to objects

    // Create a vertex shader object and a fragment shader object
    phongVS = glCreateShader(GL_VERTEX_SHADER);
    phongFS = glCreateShader(GL_FRAGMENT_SHADER);

    // Load source code strings into shaders
    glShaderSource(phongVS, 1, &phongVertex, NULL);
    glShaderSource(phongFS, 1, &phongFragment, NULL);

    // Compile the phong vertex shader and print out the compiler log file.
    glCompileShader(phongVS);
    glCompileShader(phongFS);

    this->programHandler = glCreateProgram();
    glAttachShader(this->programHandler, phongVS);
    glAttachShader(this->programHandler, phongFS);
    glLinkProgram(this->programHandler);

    // Install program object as part of current state
    glUseProgram(this->programHandler);
}

int glcShader::GetProgram()
{
    return this->programHandler;
}

char* glcShader::ReadShaderFile(string file)
{
    char *fn = (char *) file.c_str();

    FILE *fp;
    char *content = NULL;

    int count=0;

    if (fn != NULL)
    {
        fp = fopen(fn,"rt");

        if (fp != NULL)
        {

            fseek(fp, 0, SEEK_END);
            count = ftell(fp);
            rewind(fp);

            if (count > 0)
            {
                content = (char *)malloc(sizeof(char) * (count+1));
                count = fread(content,sizeof(char),count,fp);
                content[count] = '\0';
            }
            fclose(fp);
        }
        else
        {
            this->PrintError(fn);
        }
    }
    return content;
}


int glcShader::WriteShaderFile(string file, string fileOut)
{
    char *fn = (char *) file.c_str();
    char *s = NULL;

    FILE *fp;
    int status = 0;

    if (fn != NULL)
    {
        fp = fopen(fn,"w");

        if (fp != NULL)
        {

            if (fwrite(s,sizeof(char),strlen(s),fp) == strlen(s))
                status = 1;
            fclose(fp);
        }
    }

    fileOut.copy(s, strlen(s));
    return(status);
}

