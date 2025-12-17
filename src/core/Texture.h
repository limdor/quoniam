#ifndef TEXTURE_H
#define TEXTURE_H

// GLEW has to be included before any OpenGL include
#include "GL/glew.h"

#include <QtGUI/QOpenGLTexture>

class QImage;

/// Class to wrap an OpenGL texture
class Texture
{
public:
    Texture(const QImage& pTexture);
    /// Get the texture id in the GPU
    GLuint GetGLId() const;

private:
    QOpenGLTexture mTexture;
    GLuint mGLId;
};

#endif
