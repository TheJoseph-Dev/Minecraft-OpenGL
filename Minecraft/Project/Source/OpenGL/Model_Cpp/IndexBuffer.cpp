//
//  IndexBuffer.cpp
//  OpenGL Basics
//
//  Created by Jose Martos on 09/05/21.
//

#include "../Model_Headers/IndexBuffer.hpp"

#include "../../Headers/Debug.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int indexCount)
    : indexCount(indexCount)
{
    
    GLCall(glGenBuffers(1, &rendererID));
    
    //Setup Buffer
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
    GLCall(glDeleteBuffers(1, &rendererID));
}

void IndexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
}

void IndexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
