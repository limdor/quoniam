#include "Material.h"

#include <QtOpenGL/QGLWidget>

Material::Material(const QString &pName):
    mName(pName), mKa(0.2f), mKaTexture(nullptr), mKd(0.8f), mKdTexture(nullptr), mKs(1.0f), mKsTexture(nullptr), mShininess(0.0f)
{

}

Material::~Material()
{
    delete mKaTexture;
    delete mKdTexture;
    delete mKsTexture;
}

glm::vec3 Material::GetKa() const
{
    return mKa;
}

void Material::SetKa(const glm::vec3 &pKa)
{
    mKa = pKa;
}

Texture* Material::GetKaTexture() const
{
    return mKaTexture;
}

void Material::SetKaTexture(QImage* pKaTexture)
{
    delete mKaTexture;
    mKaTexture = new Texture( new QImage( QGLWidget::convertToGLFormat(*pKaTexture) ) );
}

bool Material::HasKaTexture() const
{
    return ( mKaTexture != nullptr );
}

glm::vec3 Material::GetKd() const
{
    return mKd;
}

void Material::SetKd(const glm::vec3 &pKd)
{
    mKd = pKd;
}

Texture *Material::GetKdTexture() const
{
    return mKdTexture;
}

void Material::SetKdTexture(QImage* pKdTexture)
{
    delete mKdTexture;
    mKdTexture = new Texture( new QImage( QGLWidget::convertToGLFormat(*pKdTexture) ) );
}

bool Material::HasKdTexture() const
{
    return ( mKdTexture != nullptr );
}

glm::vec3 Material::GetKs() const
{
    return mKs;
}

void Material::SetKs(const glm::vec3 &pKs)
{
    mKs = pKs;
}

Texture *Material::GetKsTexture() const
{
    return mKsTexture;
}

void Material::SetKsTexture(QImage* pKsTexture)
{
    delete mKsTexture;
    mKsTexture = new Texture( new QImage( QGLWidget::convertToGLFormat(*pKsTexture) ) );
}

bool Material::HasKsTexture() const
{
    return ( mKsTexture != nullptr );
}

float Material::GetShininess() const
{
    return mShininess;
}

void Material::SetShininess(float pShininess)
{
    mShininess = pShininess;
}
