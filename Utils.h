#pragma once

#include <string>
//#include <GL/glew.h>

// Function declarations
GLuint createShaderProgram(const char* vertexShader, const char* fragShader);
std::string readShaderSource(const char* filePath);
void printShaderLog(GLuint shader);
void printProgramLog(int prog);
bool checkOpenGLError();
