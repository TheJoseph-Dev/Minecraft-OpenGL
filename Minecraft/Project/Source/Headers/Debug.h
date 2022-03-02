#pragma once

#include <iostream>
#include <map> // Don't know why, but it only compiles here :/

#define print(any) std::cout << any << std::endl

#define arrCount(arr, type) sizeof(arr)/sizeof(type)

#define printArrWType(arr, type) for (int i = 0; i < count(arr, type); i++) { if(i == 0){ std::cout << "[ "; }  std::cout << arr[i] << ", "; if(i == count(arr, type) - 1){ std::cout << " ]\n"; } }
#define printArr(arr, c) for (int i = 0; i < c; i++) { if(i == 0){ std::cout << "[ "; }  std::cout << arr[i] << ", "; if(i == c - 1){ std::cout << " ]\n"; } }

//OpenGL Debug
#include <GL/glew.h>
//#include <GLFW/glfw3.h>

#define ASSERT(x) if (!(x)) assert(false)
#define INT2VOIDP(i) (void*)(uintptr_t)(i)

void inline GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool inline GLCheckError()
{
    while (GLenum error = glGetError())
    {

        std::cout << "[OpenGL Error] ";
        switch (error) {
        case GL_INVALID_ENUM:
            std::cout << "GL_INVALID_ENUM : An unacceptable value is specified for an enumerated argument.";
            break;
        case GL_INVALID_VALUE:
            std::cout << "GL_INVALID_OPERATION : A numeric argument is out of range.";
            break;
        case GL_INVALID_OPERATION:
            std::cout << "GL_INVALID_OPERATION : The specified operation is not allowed in the current state.";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            std::cout << "GL_INVALID_FRAMEBUFFER_OPERATION : The framebuffer object is not complete.";
            break;
        case GL_OUT_OF_MEMORY:
            std::cout << "GL_OUT_OF_MEMORY : There is not enough memory left to execute the command.";
            break;
        case GL_STACK_UNDERFLOW:
            std::cout << "GL_STACK_UNDERFLOW : An attempt has been made to perform an operation that would cause an internal stack to underflow.";
            break;
        case GL_STACK_OVERFLOW:
            std::cout << "GL_STACK_OVERFLOW : An attempt has been made to perform an operation that would cause an internal stack to overflow.";
            break;
        default:
            std::cout << "Unrecognized error" << error;
        }
        std::cout << std::endl;
        return false;
    }
    return true;
}


#ifdef DEBUG
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLCheckError())
#else
#define GLCall(x) x
#endif