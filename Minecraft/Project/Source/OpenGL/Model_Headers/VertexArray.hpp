//
//  VertexArray.hpp
//  OpenGL Basics
//
//  Created by Jose Martos on 09/05/21.
//

#pragma once

#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"

class VertexArray
{
private:
    unsigned int rendererID;
public:
    VertexArray();
    ~VertexArray();
    
    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
    
    void Bind() const;
    void Unbind() const;
};

/*class VertexArray
{
    private:
        unsigned int m_RendererID;

    public:
        VertexArray();
        ~VertexArray();

        void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
        void Bind() const;
        void Unbind() const;
};*/
