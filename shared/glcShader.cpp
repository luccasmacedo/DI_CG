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

void glcShader::Use(int use)
{
    if(use)
        glUseProgram(this->programHandler);
    else
        glUseProgram(0);
}

// deprecated
void glcShader::ApplyShader()
{
    this->Use();
}

// deprecated
void glcShader::DisableShader()
{
    this->Use(0);
}

void glcShader::PrintError(char *f)
{
    cout << "glcShader Error: File \"" << f << "\" not found.\n";
    exit(1);
}

void glcShader::InstallShaders(string vs_in, string fs_in)
{
    const char *VertexCodeString   = ReadShaderFile(vs_in);
    const char *FragmentCodeString = ReadShaderFile(fs_in);

    GLuint VS, FS;    // handles to objects

    // Create a vertex shader object and a fragment shader object
    VS = glCreateShader(GL_VERTEX_SHADER);
    FS = glCreateShader(GL_FRAGMENT_SHADER);

    // Load source code strings into shaders
    glShaderSource(VS, 1, &VertexCodeString, NULL);
    glShaderSource(FS, 1, &FragmentCodeString, NULL);

    // Compile shaders
    glCompileShader(VS);
    glCompileShader(FS);

    this->programHandler = glCreateProgram();
    glAttachShader(this->programHandler, VS);
    glAttachShader(this->programHandler, FS);
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
