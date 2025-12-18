#include "Texture.h"

Texture::Texture(const QImage& pTexture) : mTexture(pTexture.mirrored()) {}

GLuint Texture::GetGLId() const { return mTexture.textureId(); }
