#ifndef TEXTURE_H
#define TEXTURE_H

//GLEW has to be included before any OpenGL include
#include "GL/glew.h"

#include <QtGui/QImage>

#include <memory>

/// Class to wrap an OpenGL texture
class Texture
{
public:
    Texture(std::unique_ptr<QImage> pTexture, bool pRectangle = false);
    ~Texture();
    /// Get the texture id in the GPU
    GLuint GetGLId() const;

private:
    /// Image of the texture
    std::unique_ptr<QImage> mTexture;
    /// Id of the texture in the GPU
    GLuint mGLId;
};

#endif
