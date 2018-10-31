/// \file Gizmo.h
/// \class Gizmo
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef _GIZMO_H_
#define _GIZMO_H_

//Qt includes
#include <QVector>

//Dependency includes
#include "glm/vec4.hpp"

class Geometry;

/// A gizmo is a special element that can be rendered but without any kind of illumination.
class Gizmo
{
public:
    /// Constructor
    Gizmo();
    /// Copy constructor
    Gizmo( const Gizmo& pGizmo );
    /// Destructor
    ~Gizmo();
    /// Draw the gizmo
    void Draw();
    /// Get mesh
    Geometry* GetMesh() const;

protected:
    /// Mesh that represents the gizmo
    Geometry* mGizmo;

    /// Position of the vertices of the gizmo
    QVector<glm::vec4> mPositionOfVertices;

    /// Create the mesh of the gizmo with the indexs, positions and colors
    virtual void CreateMesh() = 0;

    /// Update the positions of the vertices of the mesh
    virtual void UpdatePositions() = 0;
};

#endif
