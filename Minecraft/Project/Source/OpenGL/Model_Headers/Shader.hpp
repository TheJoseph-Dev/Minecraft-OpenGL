//
//  Shader.hpp
//  OpenGL Basics
//
//  Created by Jose Martos on 10/05/21.
//

#pragma once

#include <stdio.h>
#include <string>
#include "glm/glm.hpp"

struct ShaderSource
{
    std::string VertexShader;
    std::string FragmentShader;
};

class Shader
{
    
private:
    unsigned int rendererID;
    
public:
    Shader(const std::string& filepath);
    ~Shader();
    
    void Bind() const;
    void Unbind() const;
    
    
    
    // Set uniforms
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void SetUniform1f(const std::string& name, float value);
    void SetUniform1i(const std::string& name, int value);
    
    void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
    
private:
    
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    ShaderSource ParseShader(const std::string& filePath);
    unsigned int CompileShader(unsigned int type, const std::string& source);
       
    
    // Get uniforms
    int GetUniformLocation(const std::string& name);
};
