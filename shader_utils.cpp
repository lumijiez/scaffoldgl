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

std::string ReadShaderFile(const char* filePath) {
    std::ifstream shaderFile(filePath);
    std::string shaderCode(
        (std::istreambuf_iterator<char>(shaderFile)),
        (std::istreambuf_iterator<char>())
    );
    shaderFile.close();
    return shaderCode;
}

void AddShader(GLuint program, std::string shadercode, GLenum shaderType) {
    // Creates a shader, links it
    GLuint theShader = glCreateShader(shaderType);

    // Stores shader code and length
    const GLchar* theCode[1] = { shadercode.c_str() };
    GLint codeLength[1] = { strlen(shadercode.c_str()) };

    // Links the source, length to the shader, then compiles it
    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    // Used in error logging
    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    // Gets compile status, outputs result and status
    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);

    // Error?
    if (!result) {
        glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
        printf("ERROR COMPILING %d SHADER! %s", shaderType, eLog);
        return;
    }

    // Attaches shader to program
    glAttachShader(program, theShader);
}

void CompileShaders(GLuint* shader, GLuint* uniformModel, GLuint* uniformProjection, std::string vShader, std::string fShader) {
    // Creates the program template for our shaders
    *shader = glCreateProgram();

    // Error?
    if (!shader) {
        printf("Error creating shader program!");
        return;
    }

    // Adds shaders to the program
    AddShader(*shader, vShader, GL_VERTEX_SHADER);
    AddShader(*shader, fShader, GL_FRAGMENT_SHADER);

    // Initialization for the result code and operation log
    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    // Links shader to program, gets link status into the "eLog" array
    glLinkProgram(*shader);
    glGetProgramiv(*shader, GL_LINK_STATUS, &result);

    // Error?
    if (!result) {
        glGetProgramInfoLog(*shader, sizeof(eLog), NULL, eLog);
        printf("ERROR LINKING PROGRAM %s", eLog);
        return;
    }

    // Validates shader, gets the status into the "eLog" array
    glValidateProgram(*shader);
    glGetProgramiv(*shader, GL_VALIDATE_STATUS, &result);

    // Error?
    if (!result) {
        glGetProgramInfoLog(*shader, sizeof(eLog), NULL, eLog);
        printf("ERROR VALIDATING PROGRAM %s", eLog);
        return;
    }

    // Gets the shader location IDs
    *uniformModel = glGetUniformLocation(*shader, "model");
    *uniformProjection = glGetUniformLocation(*shader, "projection");
}