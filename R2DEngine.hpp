/**
 * @file R2DEngine.hpp
 * @author Daniel Hongyu Ding
 * @brief Rainstorm 2D Engine
 * @version 0.1
 * @date 2021-01-05
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once
#ifndef R2DENGINE_HPP
#define R2DENGINE_HPP

// standard libraries
#include <cstdint>
#include <cmath>
#include <string>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <chrono>
#include <vector>
#include <array>
#include <list>
#include <map>
#include <algorithm>

// opengl related
#define GLFW_INCLUDE_NONE
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/*
STATIC_ASSERT(expr) assert expr at compile-time

ASSERT(expr) assert expr at run-time

DEBUG_MSG(expr) put expr to standard error

DEBUG_MSG_INDENT(expr, depth) put expr with indentation based on depth
    to standard error

DEBUG_LINE() put a horizontal line to standard error

DEBUG_LINE_INDENT(depth) put a horizontal line with indentation
    based on depth to standard error
*/

#if DEBUG_ENABLED

    #ifdef __cplusplus
        #if __cplusplus >= 201103L
            #define STATIC_ASSERT(expr) \
                static_assert(expr, "static assert failed:"#expr)
        #endif
    #endif

    #if defined(__i386__) || defined(__x86_64__)
        #define debugbreak() __asm__ __volatile__("int3")
    #elif defined(__aarch64__)
        #define debugbreak() __asm__ __volatile__(".inst 0xd4200000")
    #elif defined(__arm__)
        #define debugbreak() __asm__ __volatile__(".inst 0xe7f001f0")
    #endif

    #define ASSERT(expr) \
    if (expr) {}  \
    else {  \
        Debug::msg(#expr, __FILE__, __LINE__);  \
        debugbreak(); \
    }

    #define DEBUG_MSG(expr) Debug::msg(expr)
    #define DEBUG_MSG_INDENT(expr, depth) Debug::msg(expr, depth)
    #define DEBUG_LINE() Debug::line()
    #define DEBUG_LINE_INDENT(depth) Debug::line(depth)

#else
    #define STATIC_ASSERT(expr)
    #define ASSERT(expr) 
    #define DEBUG_MSG(expr) 
    #define DEBUG_MSG_INDENT(expr, depth) 
    #define DEBUG_LINE()
    #define DEBUG_LINE_INDENT(depth)
#endif

#define DEBUG_ERROR(expr) Debug::error(expr)
#define DEBUG_ERROR_INDENT(expr, depth) Debug::error(expr, depth);

namespace Debug {
    void msg(const char* expr, const char* file, int line) {
        std::cerr << "[" << file << "] at line " << line << ": " << expr << std::endl; 
    }

    void msg(const char* expr, int depth = 0) {
        for (int i = 0; i < depth; i ++) {
            std::cerr << "    ";
        }
        std::cerr << expr << std::endl;
    }

    void error(const char* expr, int depth = 0) {
        for (int i = 0; i < depth; i ++) {
            std::cerr << "    ";
        }
        std::cerr << "ERROR: " << expr << std::endl;
    }

    void line(int depth = 0) {
        for (int i = 0; i < depth; i ++) {
            std::cerr << "    ";
        }
        for (int i = 0; i < 80 - depth * 4; i ++) {
            std::cerr << "-";
        }
        std::cerr << std::endl;
    }
};

// glfw callbacks
void glfwErrorCallback(int error, const char* description);
void glfwFramebufferSizeCallback(GLFWwindow* window, int screenWidth, int screenHeight);

// default shaders
const char *vShader = "                     \n\
#version 410                                \n\
                                            \n\
layout (location = 0) in vec2 position;     \n\
layout (location = 1) in vec2 tex;          \n\
                                            \n\
out vec2 texCoord;                          \n\                                            
                                            \n\
void main() {                               \n\
    gl_Position = vec4(position, 0.0, 1.0); \n\
    texCoord = tex;                         \n\
}";

const char *fShader = "                             \n\
#version 410                                        \n\
                                                    \n\
in vec2 texCoord;                                   \n\
out vec4 gl_FragColor;                              \n\
uniform sampler2D theTexture;                       \n\
                                                    \n\
void main() {                                       \n\
    gl_FragColor = texture(theTexture, texCoord);   \n\
}";

class R2DEngine {
private:
    // game
    bool loop;

    // graphics
    GLFWwindow* window;
    GLuint shader;
    GLuint bufferTexture;
    GLubyte* bufferData;
    GLuint vao, ibo, vbo;

protected:
    // game
    struct Coord {
        uint32_t x = 0;
        uint32_t y = 0;
    };

    struct Color {
        uint8_t r = 0;
        uint8_t g = 0;
        uint8_t b = 0;
        uint8_t a = 255;
    };
    
    // graphics
    int32_t screenWidth;
    int32_t screenHeight;
    int32_t innerWidth;
    int32_t innerHeight;
    std::string windowTitle;

    // events
    int32_t mousePosX;
    int32_t mousePosY;
    
private:
    void gameLoop();

    void clearBuffer();
    void swapBuffers();
    void addShader(GLuint program, const char* shaderCode, GLenum shaderType);
    void compileShaders();

public:
    R2DEngine();
    virtual ~R2DEngine();

public:
    // user interfaces
    virtual bool onCreate() = 0;
    virtual bool onUpdate(double deltaTime) = 0;
    virtual bool onDestroy() {
        return true;
    }

public:
    // game
    bool construct(int32_t screenWidth = 800, int32_t screenHeight = 600, int32_t innerWidth = 800, int32_t innerHeight = 600);
    void init();

public:
    // graphics
    void drawPoint(Coord coord, Color color);
    void drawLine(Coord coord1, Coord coord2, Color color);
};

void glfwErrorCallback(int error, const char* description) {
    DEBUG_MSG(description);
}

void glfwFramebufferSizeCallback(GLFWwindow* window, int screenWidth, int screenHeight) {
    glViewport(0, 0, screenWidth, screenHeight);
}


R2DEngine::R2DEngine() {
    window = nullptr;
    shader = 0;
    vao = 0;
    vbo = 0;
    ibo = 0;
    bufferData = nullptr;
    bufferTexture = 0;
    loop = false;

    screenWidth = 0;
    screenHeight = 0;
    innerWidth = 0;
    innerHeight = 0;
    windowTitle = "R2DEngine";
}

R2DEngine::~R2DEngine() {}

bool R2DEngine::construct(int32_t screenWidth, int32_t screenHeight, int32_t innerWidth, int innerHeight) {
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    this->innerWidth = innerWidth;
    this->innerHeight = innerHeight;

    if (!glfwInit()) {
        DEBUG_ERROR("Failed to initialize GLFW");
        return false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwSetErrorCallback(glfwErrorCallback);

    window = glfwCreateWindow(this->screenWidth, this->screenHeight, windowTitle.c_str(), nullptr, nullptr);
    if (!window) {
        DEBUG_ERROR("Failed to create GLFW window");
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        DEBUG_ERROR("Failed to initialize GLEW");
        glfwDestroyWindow(window);
        glfwTerminate();
        return false;
    }

    glViewport(0, 0, this->screenWidth, this->screenHeight);
    glfwSetFramebufferSizeCallback(window, glfwFramebufferSizeCallback);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, this->screenWidth, this->screenHeight, 0.0, 1.0, -1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glfwSwapInterval(1);

    DEBUG_MSG("window constructed");

    compileShaders();
    DEBUG_MSG("shaders compiled");

    bufferData = new GLubyte[screenWidth * screenHeight * 4];
    memset(bufferData, 0, sizeof(bufferData));
    glGenTextures(1, &bufferTexture);
    glBindTexture(GL_TEXTURE_2D, bufferTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenWidth, screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)bufferData);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLfloat vertices[] = {
        -1.0f, 1.0f,    0.0f, 0.0f,
        1.0f, 1.0f,     1.0f, 0.0f,
        1.0f, -1.0f,    1.0f, 1.0f,
        -1.0f, -1.0f,   0.0f, 1.0f
    };
    GLuint indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 4, 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 4, (GLvoid*)(sizeof(vertices[0]) * 2));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    DEBUG_MSG("buffer generated");

    return true;
}

void R2DEngine::init() {
    DEBUG_MSG("init");
    loop = true;
    gameLoop();
}

void R2DEngine::clearBuffer() {
    glClear(GL_COLOR_BUFFER_BIT);
}

void R2DEngine::swapBuffers() {
    glUseProgram(shader);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, bufferTexture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, screenWidth, screenHeight, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)bufferData);

    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glUseProgram(0);
    glfwSwapBuffers(window);
}

void R2DEngine::addShader(GLuint program, const char* shaderCode, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);

    const GLchar *theCode[1];
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    glShaderSource(shader, 1, theCode, codeLength);
    glCompileShader(shader);

    GLint test;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &test);
    if (!test) {
        DEBUG_ERROR("shader compilation failed:");
        GLchar log[1024] = {0};
        glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
        DEBUG_ERROR(log);
        return;
    }
    glAttachShader(program, shader);
}

void R2DEngine::compileShaders() {
    shader = glCreateProgram();
    if (!shader) {
        DEBUG_ERROR("failed to create shader program");
        return;
    }

    addShader(shader, vShader, GL_VERTEX_SHADER);
    addShader(shader, fShader, GL_FRAGMENT_SHADER);

    glLinkProgram(shader);

    GLint test;
    glGetProgramiv(shader, GL_LINK_STATUS, &test);
    if (!test) {
        DEBUG_ERROR("failed to link program:");
        GLchar log[1024] = {0};
        glGetProgramInfoLog(shader, sizeof(log), nullptr, log);
        DEBUG_ERROR(log);
        return;
    }

    glValidateProgram(shader);

    glGetProgramiv(shader, GL_VALIDATE_STATUS, &test);
    if (!test) {
        DEBUG_ERROR("failed to validate program:");
        GLchar log[1024] = {0};
        glGetProgramInfoLog(shader, sizeof(log), nullptr, log);
        DEBUG_ERROR(log);
        return;
    }
}

void R2DEngine::gameLoop() {
    if (!onCreate()) {
        loop = false;
    }

    double time_a = glfwGetTime();
    double lastSec = glfwGetTime();
    double time_b = glfwGetTime();
    uint32_t nFrames = 0;

    DEBUG_MSG("game loop start");
    while (loop) {
        while (loop) {
            time_b = glfwGetTime();
            double deltaTime = time_b - time_a;
            double secPast = time_b - lastSec;
            time_a = time_b;
            nFrames ++;
            if (secPast >= 1.0f) {
                double fps = static_cast<double>(nFrames) / deltaTime;
                std::stringstream ss;
                ss << windowTitle << " - FPS:" << std::setprecision(3) << fps;
                glfwSetWindowTitle(window, ss.str().c_str());
                nFrames = 0;
                lastSec = time_b;
            }

            if (glfwWindowShouldClose(window)) {
                loop = false;
            }
            glfwPollEvents();

            clearBuffer();
            if (!onUpdate(deltaTime)) {
                loop = false;
            }
            swapBuffers();
        }

        if (!onDestroy()) {
            loop = true;
        }
    }

    DEBUG_MSG("game loop end");

    if (ibo != 0) {
        glDeleteBuffers(1, &ibo);
        ibo = 0;
    }
    if (vbo != 0) {
        glDeleteBuffers(1, &vbo);
        vbo = 0;
    }
    if (vao != 0) {
        glDeleteBuffers(1, &vao);
        vao = 0;
    }
    glDeleteTextures(1, &bufferTexture);
    delete bufferData;
            
    glfwDestroyWindow(window);
    glfwTerminate();

    DEBUG_MSG("glfw destroyed");
}

void R2DEngine::drawPoint(Coord coord, Color color) {
    bufferData[coord.y * screenWidth * 4 + coord.x * 4 + 0] = color.r;
    bufferData[coord.y * screenWidth * 4 + coord.x * 4 + 1] = color.g;
    bufferData[coord.y * screenWidth * 4 + coord.x * 4 + 2] = color.b;
    bufferData[coord.y * screenWidth * 4 + coord.x * 4 + 3] = color.a;
}

#endif