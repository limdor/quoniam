/// \file Mesh.h
/// \class Mesh
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef _MESH_H_
#define _MESH_H_

//Qt includes
#include <QString>

//Project includes
#include "Geometry.h"
#include "Material.h"

/// Class to wrap a 3d mesh
class Mesh
{
public:
    /// Constructor
    Mesh(Geometry * pGeometry, Material * pMaterial);
    /// Copy constructor
    Mesh(const Mesh& pMesh);
    /// Destructor
    ~Mesh();

    /// Set the name of the mesh
    void SetName(const QString &pName);

    /// Set the material that will be used for the renderer
    void SetMaterial(Material* pMaterial);
    /// Get the material used for the renderer
    Material* GetMaterial() const;

    /// Get geometry
    Geometry* GetGeometry() const;

    /// Have to be rendered?
    void SetVisible(bool pVisible);

    /// Draw the mesh
    void Draw();

private:
    /// Name of the mesh
    QString mName;

    /// Geometry associated to the mesh
    Geometry* mGeometry;
    /// Material used to render the mesh
    Material* mMaterial;
    /// Have to be rendered?
    bool mVisible;
};

#endif
