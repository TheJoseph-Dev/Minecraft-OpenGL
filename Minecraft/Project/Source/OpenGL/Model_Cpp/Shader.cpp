//
//  Shader.cpp
//  OpenGL Basics
//
//  Created by Jose Martos on 10/05/21.
//

#include "../Model_Headers/Shader.hpp"

#include "../../Headers/Debug.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ostream>

Shader::Shader(const std::string& filepath)
{
    ShaderSource source = ParseShader(filepath);
    unsigned int shader = CreateShader(source.VertexShader, source.FragmentShader);
    rendererID = shader;
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(0));
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram(); // A program object is an object to which shader objects can be attached
    
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    
    __glewAttachShader(program, vs); // Attach Vertex Shader to program
    __glewAttachShader(program, fs); // Attach Fragment Shader to program
    
    glLinkProgram(program); // Link the shaders at the program.
    glValidateProgram(program); // Checks to see whether the executables contained in program can execute given the current OpenGL state.
    
    //After link and validate the program, the shaders are not useful anymore, and can be removed.
    glDeleteShader(vs);
    glDeleteShader(fs);
    
    return program;
}

ShaderSource Shader::ParseShader(const std::string& filePath)
{
    std::ifstream stream(filePath);

    print(filePath);
    if (!stream) {
        print("Null directory");
    }
    
    enum class ShaderType {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };
    
    std::string line;
    std::stringstream ss[2];
    
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line)) {
        
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
                //print("v");
            }
            else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
                //print("f");
            }
        }
        else {
            ss[(int)type] << line << '\n';
        }
    }
    
    //return {ss[0].str(), ss[1].str()};
    return {ss[0].str(), ss[1].str()};
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type); //A shader object is used to maintain the source code strings that define a shader.
    const char* src = source.c_str();
    
    glShaderSource(id, 1, &src, nullptr); // Sets the source code in shader to the source code in the array of strings, any source code previously stored in the shader object is completely replaced.
    glCompileShader(id); // Compiles the source code strings that have been stored in the shader object specified by shader.
    
    
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result); //Returns the value in the third parameter for a specific shader object.
    
    if (result == GL_FALSE) { //Handle Errors
        int lenght;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght);
        char* message = (char*)alloca(lenght * sizeof(char));
        glGetShaderInfoLog(id, lenght, &lenght, message); //Get the error message
        print("Failed to compile shader");
        print(message);
        glDeleteShader(id);
        return 0;
    }
    
    
    return id;
}



void Shader::Bind() const
{
    GLCall(glUseProgram(rendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}



void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform1f(const std::string &name, float value)
{
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform1i(const std::string &name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

int Shader::GetUniformLocation(const std::string& name)
{
    GLCall(int location = glGetUniformLocation(rendererID, name.c_str()));
    
    if (location == -1) { print("Uniform location does not exisit"); return location; }
    
    return location;
}
