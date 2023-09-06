#pragma once
#ifndef SHADER_UTILS_H
#define SHADER_UTILS_H

#include <string>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

std::string ReadShaderFile(const char* filePath);
void AddShader(GLuint program, std::string shadercode, GLenum shaderType);
void CompileShaders(GLuint* shader, GLuint* uniformModel, GLuint* uniformProjection, std::string vShader, std::string fShader);

#endif