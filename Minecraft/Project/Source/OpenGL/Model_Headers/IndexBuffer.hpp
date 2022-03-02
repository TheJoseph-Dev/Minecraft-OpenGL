//
//  IndexBuffer.hpp
//  OpenGL Basics
//
//  Created by Jose Martos on 09/05/21.
//

#pragma once

class IndexBuffer
{
    
private:
    unsigned int rendererID;
    unsigned int indexCount;
    
public:
    IndexBuffer(const unsigned int* data, unsigned int indexCount);
    ~IndexBuffer();
    
    void Bind() const;
    void Unbind() const;
    
    inline unsigned int GetCount() const { return indexCount; };
};
