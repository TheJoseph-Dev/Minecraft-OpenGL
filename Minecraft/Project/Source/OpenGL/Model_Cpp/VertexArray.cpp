//
//  VertexArray.cpp
//  OpenGL Basics
//
//  Created by Jose Martos on 09/05/21.
//


#include "../Model_Headers/VertexArray.hpp"

#include "../../Headers/Debug.h"

VertexArray::VertexArray()
{
    GLCall(glGenVertexArrays(1, &rendererID));
}

VertexArray::~VertexArray()
{
    GLCall(glDeleteVertexArrays(1, &rendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)

{
    Bind();
    vb.Bind();
    const auto& elements = layout.GetElements();
    unsigned int offset = 0;
    
    for (unsigned int i = 0; i < elements.size(); i++)
    {
        const auto& element = elements[i];
        GLCall(glEnableVertexAttribArray(i)); //Enables the vertex, specified by the index in the array to allow the draw function.
        GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), INT2VOIDP(offset) /* Cast offset to void */ )); // Tells OpenGL how to layout the data recived. (the first parameter links a vertex array to the buffer), (the second parameter indicates the object dimension), (the stride parameter is the offset in bytes between each vertex).

        offset += (sizeof(element.type) * element.count);
    }
    
}


void VertexArray::Bind() const {
    GLCall(glBindVertexArray(rendererID));
}

void VertexArray::Unbind() const {
    GLCall(glBindVertexArray(0));
}
