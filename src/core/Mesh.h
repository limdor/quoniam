#ifndef MESH_H
#define MESH_H

#include "Geometry.h"
#include "Material.h"

#include <QtCore/QString>

#include <memory>

/// Class to wrap a 3d mesh
class Mesh
{
public:
    Mesh(std::shared_ptr<Geometry> pGeometry, std::shared_ptr<Material> pMaterial);
    Mesh(const Mesh& pMesh) = delete;
    ~Mesh() = default;

    /// Set the name of the mesh
    void SetName(const QString &pName);

    /// Set the material that will be used for the renderer
    void SetMaterial(std::shared_ptr<Material> pMaterial);
    /// Get the material used for the renderer
    std::shared_ptr<Material> GetMaterial() const;

    /// Get geometry
    std::shared_ptr<Geometry> GetGeometry() const;

    /// Have to be rendered?
    void SetVisible(bool pVisible);

    /// Draw the mesh
    void Draw();

private:
    /// Name of the mesh
    QString mName;

    /// Geometry associated to the mesh
    std::shared_ptr<Geometry> mGeometry;
    /// Material used to render the mesh
    std::shared_ptr<Material> mMaterial;
    /// Have to be rendered?
    bool mVisible;
};

#endif
