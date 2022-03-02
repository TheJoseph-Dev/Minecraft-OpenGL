//
//  VertexBuffer.hpp
//  OpenGL Basics
//
//  Created by Jose Martos on 09/05/21.
//

#pragma once

class VertexBuffer
{
    
private:
    unsigned int rendererID;
    
public:
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();
    
    void Bind() const;
    void Unbind() const;
};
