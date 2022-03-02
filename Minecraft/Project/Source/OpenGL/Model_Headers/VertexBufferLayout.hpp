//
//  VertexBufferLayout.hpp
//  OpenGL Basics
//
//  Created by Jose Martos on 09/05/21.
//

#pragma once

#include <vector>
#include <GL/glew.h>

struct VertexBufferElement
{
    unsigned int type;
    unsigned int count;
    bool normalized;
    
};

class VertexBufferLayout
{
private:
    std::vector<VertexBufferElement> elements;
    unsigned int stride;
public:
    VertexBufferLayout()
    : stride(0) {}
    
    template<typename T>
    void Push(unsigned int count) {
        //static_assert(false);
        static_assert(sizeof(T) == 0, "Type not implemented.");
    }
    
    template<>
    void Push<float>(unsigned int count)
    {
        const VertexBufferElement vertexBufferElement = {GL_FLOAT, count, GL_FALSE};
        elements.push_back(vertexBufferElement);
        stride += sizeof(GL_FLOAT) * count;
    }
    
    template<>
    void Push<unsigned int>(unsigned int count)
    {
        elements.push_back({GL_UNSIGNED_INT, count, false});
        stride += sizeof(GL_UNSIGNED_INT) * count;
    }
    
    template<>
    void Push<unsigned char>(unsigned int count)
    {
        elements.push_back({GL_UNSIGNED_BYTE, count, false});
        stride += sizeof(GL_UNSIGNED_BYTE) * count;
    }
    
    inline const std::vector<VertexBufferElement> GetElements() const& { return elements; }
    inline unsigned int GetStride() const { return stride; }
    
};
