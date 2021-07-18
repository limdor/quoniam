#include "Material.h"

#include <QtOpenGL/QGLWidget>

Material::Material(const std::string &pName):
    mName(pName), mKa(0.2f), mKaTexture(nullptr), mKd(0.8f), mKdTexture(nullptr), mKs(1.0f), mKsTexture(nullptr), mShininess(0.0f)
{

}

glm::vec3 Material::GetKa() const
{
    return mKa;
}

void Material::SetKa(const glm::vec3 &pKa)
{
    mKa = pKa;
}

std::shared_ptr<Texture> Material::GetKaTexture() const
{
    return mKaTexture;
}

void Material::SetKaTexture(const QImage& pKaTexture)
{
    mKaTexture = std::make_shared<Texture>( std::make_unique<QImage>( QGLWidget::convertToGLFormat(pKaTexture) ) );
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

std::shared_ptr<Texture> Material::GetKdTexture() const
{
    return mKdTexture;
}

void Material::SetKdTexture(const QImage& pKdTexture)
{
    mKdTexture = std::make_shared<Texture>( std::make_unique<QImage>( QGLWidget::convertToGLFormat(pKdTexture) ) );
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

std::shared_ptr<Texture> Material::GetKsTexture() const
{
    return mKsTexture;
}

void Material::SetKsTexture(const QImage& pKsTexture)
{
    mKsTexture = std::make_shared<Texture>( std::make_unique<QImage>( QGLWidget::convertToGLFormat(pKsTexture) ) );
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
