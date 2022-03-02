//
//  VertexBuffer.cpp
//  OpenGL Basics
//
//  Created by Jose Martos on 09/05/21.
//

#include "../Model_Headers/VertexBuffer.hpp"

#include "../../Headers/Debug.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    GLCall(glGenBuffers(1, &rendererID)); // The first parameter indicates the number of buffer objects that will be generated, the second one return the array of buffers.
    
    //Setup Buffer
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID)); // The first parameter indicates the purpose of the buffer, the second one specifies the buffer object.
    
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW)); // Stores the data in the buffer.
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &rendererID));
}

void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
