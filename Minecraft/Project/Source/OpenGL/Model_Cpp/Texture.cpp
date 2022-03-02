//
//  Texture.cpp
//  OpenGL Basics
//
//  Created by Jose Martos on 15/05/21.
//

#include "../Model_Headers/Texture.hpp"
#include <string>

#define STBI_FAILURE_USERMSG
#define STB_IMAGE_IMPLEMANTATION
//#include "stb_image.h"

#include "../../Headers/Debug.h"

#include "SOIL2/SOIL2.h"


Texture::Texture(const std::string &path)
: rendererID(0), localBuffer(nullptr), width(0), height(0), BPP(0)
{
    //stbi_set_flip_vertically_on_load(0); // We have to flip the image because OpenGL draw start point is in the bottom left, instead of top left.
    
    //localBuffer = stbi_load(path.c_str(), &width, &height, &BPP, 4);
    //print(localBuffer);
    GLCall(glGenTextures(1, &rendererID));
    Bind();
    
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    
    //GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer));
    
    unsigned char *image = SOIL_load_image( path.c_str(), &width, &height, 0, SOIL_LOAD_RGBA );
    GLCall(glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image ));
    //glRotatef(3.14159265, 1, 0, 0);
    glGenerateMipmap( GL_TEXTURE_2D );
    SOIL_free_image_data( image );
    
    Unbind();
    

    //if (localBuffer) /* != NULL */ { stbi_image_free(localBuffer); }
}


Texture::~Texture()
{
    GLCall(glDeleteTextures(1, &rendererID));
}

void Texture::Bind(unsigned int slot /* = 0*/) const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot)); // Defines the Texture slot
    GLCall(glBindTexture(GL_TEXTURE_2D, rendererID));
}

void Texture::Unbind() const
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}


/*
Texture::Texture(const std::string& path)
: rendererID(0), localBuffer(nullptr), width(0), height(0), BPP(0)
{
    stbi_set_flip_vertically_on_load(1);
    localBuffer = stbi_load(path.c_str(), &width, &height, &BPP, 4);
    GLCall( glGenTextures(1, &rendererID) );
    GLCall( glBindTexture(GL_TEXTURE_2D, rendererID) ); // Bind without slot selection

    GLCall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR) );
    GLCall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR) );
    GLCall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE) );
    GLCall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE) );

    GLCall( glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer) );
    Unbind();

    if (localBuffer)
        stbi_image_free(localBuffer);
};

Texture::~Texture()
{
    GLCall( glDeleteTextures(1, &rendererID) );
}

void Texture::Bind(unsigned int slot) const
{
    GLCall( glActiveTexture(GL_TEXTURE0 + slot) );
    GLCall( glBindTexture(GL_TEXTURE_2D, rendererID) );
}

void Texture::Unbind() const
{
    GLCall( glBindTexture(GL_TEXTURE_2D, 0) );
}
*/
