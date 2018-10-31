/// \file Material.h
/// \class Material
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef MATERIAL_H
#define MATERIAL_H

//Qt includes
#include <QString>
#include <QImage>

//Dependency includes
#include "glm/vec3.hpp"

//Project includes
#include "Texture.h"

/// Class to wrap the material of a mesh
class Material
{
public:
    /// Constructor
    /// \param pName Name of the material
    explicit Material(const QString &pName);
    /// Destructor
    ~Material();

    /// Get the ambient color
    glm::vec3 GetKa() const;
    /// Set the ambient color
    void SetKa(const glm::vec3 &pKa);
    /// Get the ambient texture
    Texture *GetKaTexture() const;
    /// Set the ambient texture
    void SetKaTexture(QImage* pKaTexture);
    /// Return if the material has an ambient texture
    bool HasKaTexture() const;

    /// Get the diffuse color
    glm::vec3 GetKd() const;
    /// Set the diffuse color
    void SetKd(const glm::vec3 &pKd);
    /// Get the diffuse texture
    Texture* GetKdTexture() const;
    /// Set the diffuse texture
    void SetKdTexture(QImage* pKdTexture);
    /// Return if the material has a diffuse texture
    bool HasKdTexture() const;

    /// Get the specular color
    glm::vec3 GetKs() const;
    /// Set the specular color
    void SetKs(const glm::vec3 &pKs);
    /// Get the specular texture
    Texture* GetKsTexture() const;
    /// Set the specular texture
    void SetKsTexture(QImage* pKsTexture);
    /// Return if the material has a specular texture
    bool HasKsTexture() const;
    /// Get the specular exponent
    float GetShininess() const;
    /// Set the specular exponent
    void SetShininess(float pShininess);

private:
    /// Name of the material
    QString mName;
    /// Ambient color
    glm::vec3 mKa;
    /// Ambient texture
    Texture* mKaTexture;
    /// Diffuse color
    glm::vec3 mKd;
    /// Diffuse texture
    Texture* mKdTexture;
    /// Specular color
    glm::vec3 mKs;
    /// Specular texture
    Texture* mKsTexture;
    /// Specular exponent
    float mShininess;
};

#endif
