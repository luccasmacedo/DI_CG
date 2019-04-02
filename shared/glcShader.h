/*
  Name:        glcShader.h
  Author:      Rodrigo Luis de Souza da Silva
  Date:        30/11/2017
  Description: GL class to help dealing with shaders
*/

#ifndef _GLC_SHADER_H
#define _GLC_SHADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <GL/glew.h>

using namespace std;

class glcShader
{
    public:
        ~glcShader();
        glcShader();
        glcShader(string vs, string fs);
        int  GetProgram();
        void Use(int use = 1);

    public: // deprecated
        void ApplyShader();
        void DisableShader();

    private:
        GLuint programHandler;
        char*  ReadShaderFile(string);
        void InstallShaders(string vs, string fs);
        void PrintError(char *fn);
};

#endif


