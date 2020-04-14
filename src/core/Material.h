#ifndef MATERIAL_H
#define MATERIAL_H

#include "Texture.h"

#include <QtCore/QString>
#include <QtGui/QImage>

#include "glm/vec3.hpp"

#include <memory>

/// Class to wrap the material of a mesh
class Material
{
public:
    /// Constructor
    /// \param pName Name of the material
    explicit Material(const QString &pName);

    /// Get the ambient color
    glm::vec3 GetKa() const;
    /// Set the ambient color
    void SetKa(const glm::vec3 &pKa);
    /// Get the ambient texture
    std::shared_ptr<Texture> GetKaTexture() const;
    /// Set the ambient texture
    void SetKaTexture(const QImage& pKaTexture);
    /// Return if the material has an ambient texture
    bool HasKaTexture() const;

    /// Get the diffuse color
    glm::vec3 GetKd() const;
    /// Set the diffuse color
    void SetKd(const glm::vec3 &pKd);
    /// Get the diffuse texture
    std::shared_ptr<Texture> GetKdTexture() const;
    /// Set the diffuse texture
    void SetKdTexture(const QImage& pKdTexture);
    /// Return if the material has a diffuse texture
    bool HasKdTexture() const;

    /// Get the specular color
    glm::vec3 GetKs() const;
    /// Set the specular color
    void SetKs(const glm::vec3 &pKs);
    /// Get the specular texture
    std::shared_ptr<Texture> GetKsTexture() const;
    /// Set the specular texture
    void SetKsTexture(const QImage& pKsTexture);
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
    std::shared_ptr<Texture> mKaTexture;
    /// Diffuse color
    glm::vec3 mKd;
    /// Diffuse texture
    std::shared_ptr<Texture> mKdTexture;
    /// Specular color
    glm::vec3 mKs;
    /// Specular texture
    std::shared_ptr<Texture> mKsTexture;
    /// Specular exponent
    float mShininess;
};

#endif
