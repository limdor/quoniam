/// \file Texture.h
/// \class Texture
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

//GLEW has to be included before any OpenGL include
#include "glew.h"

//Qt includes
#include <QImage>

/// Class to wrap an OpenGL texture
class Texture
{
public:
    /// Constructor
    Texture(QImage* pTexture, bool pRectangle = false);
    /// Destructor
    ~Texture();
    /// Get the texture id in the GPU
    GLuint GetGLId() const;

private:
    /// Image of the texture
    QImage* mTexture;
    /// Id of the texture in the GPU
    GLuint mGLId;
};

#endif
