//Definition include
#include "Material.h"

//Qt includes
#include <QGLWidget>

Material::Material(const QString &pName):
    mName(pName), mKa(0.2f), mKaTexture(NULL), mKd(0.8f), mKdTexture(NULL), mKs(1.0f), mKsTexture(NULL), mShininess(0.0f)
{

}

Material::~Material()
{
    if( mKaTexture != NULL )
    {
        delete mKaTexture;
    }
    if( mKdTexture != NULL )
    {
        delete mKdTexture;
    }
    if( mKsTexture != NULL )
    {
        delete mKsTexture;
    }
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
    if(mKaTexture != NULL)
    {
        delete mKaTexture;
    }
    mKaTexture = new Texture( new QImage( QGLWidget::convertToGLFormat(*pKaTexture) ) );
}

bool Material::HasKaTexture() const
{
    return ( mKaTexture != NULL );
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
    if(mKdTexture != NULL)
    {
        delete mKdTexture;
    }
    mKdTexture = new Texture( new QImage( QGLWidget::convertToGLFormat(*pKdTexture) ) );
}

bool Material::HasKdTexture() const
{
    return ( mKdTexture != NULL );
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
    if( mKsTexture != NULL )
    {
        delete mKsTexture;
    }
    mKsTexture = new Texture( new QImage( QGLWidget::convertToGLFormat(*pKsTexture) ) );
}

bool Material::HasKsTexture() const
{
    return ( mKsTexture != NULL );
}

float Material::GetShininess() const
{
    return mShininess;
}

void Material::SetShininess(float pShininess)
{
    mShininess = pShininess;
}
