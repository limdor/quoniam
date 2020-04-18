#ifndef GIZMO_H
#define GIZMO_H

#include "Geometry.h"

#include "glm/vec4.hpp"

#include <vector>

/// A gizmo is a special element that can be rendered but without any kind of illumination.
class Gizmo
{
public:
    /// Draw the gizmo
    void Draw();

protected:
    /// Mesh that represents the gizmo
    Geometry mGizmo;

    /// Position of the vertices of the gizmo
    std::vector<glm::vec4> mPositionOfVertices;

    /// Create the mesh of the gizmo with the indexs, positions and colors
    virtual void CreateMesh() = 0;

    /// Update the positions of the vertices of the mesh
    virtual void UpdatePositions() = 0;
};

#endif
