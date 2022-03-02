//
//  Texture.hpp
//  OpenGL Basics
//
//  Created by Jose Martos on 15/05/21.
//

#pragma once

#include <stdio.h>
#include <iostream>

class Texture {
  
private:
    unsigned int rendererID;
    //std::string filePath;
    unsigned char* localBuffer;
    int width, height, BPP;
    
public:
    Texture(const std::string& path);
    ~Texture();
    
    void Bind(unsigned int slot = 0) const;
    void Unbind() const;
    
    inline int GetWidth() const { return width; }
    inline int GetHeight() const { return height; }
};
