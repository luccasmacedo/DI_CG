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
        string v;

        ~glcShader();
        glcShader();
        glcShader(string vs, string fs);
        void ApplyShader();
        void DisableShader();
        int  GetProgram();

    private:
        GLuint programHandler;
        char*  ReadShaderFile(string);
        //char*  ReadShaderFile2(string);
        //string ReadShaderFile(string);
        int  WriteShaderFile(string, string);
        void InstallShaders(string vs, string fs);
        void PrintError(char *fn);
};

#endif


