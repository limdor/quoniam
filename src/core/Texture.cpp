#include "Texture.h"

Texture::Texture(QImage *pTexture, bool pRectangle):
    mTexture(pTexture)
{
    int imageWidth = pTexture->width();
    int imageHeight = pTexture->height();

    glGenTextures(1, &mGLId);
    if(!pRectangle)
    {
        glBindTexture(GL_TEXTURE_2D, mGLId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pTexture->bits());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        glBindTexture(GL_TEXTURE_RECTANGLE, mGLId);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA32F, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pTexture->bits());
    }
}

Texture::~Texture()
{
    glDeleteTextures(1, &mGLId);
    delete mTexture;
}

GLuint Texture::GetGLId() const
{
    return mGLId;
}
