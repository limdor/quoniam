/// \file BoundingSphere.h
/// \class BoundingSphere
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef _BOUNDING_SPHERE_H_
#define _BOUNDING_SPHERE_H_

//Dependency includes
#include "glm/vec3.hpp"

//Project includes
#include "Gizmo.h"

/// BoundingVolume class with methods to use a bounding sphere
class BoundingSphere : public Gizmo
{
public:
    /// Constructor
    BoundingSphere();
    /// Destructor
    ~BoundingSphere();

    /// Get the center
    glm::vec3 GetCenter() const;
    /// Get the radius
    float GetRadius() const;

    /// Set the center
    void SetCenter(const glm::vec3 &pValue);
    /// Set the radius
    void SetRadius(float pValue);

    /// Create the minimum bounding sphere that includes pBS0 and pBS1
    static BoundingSphere* Merge(const BoundingSphere* pBS0, const BoundingSphere* pBS1);

protected:
    /// Center
    glm::vec3 mCenter;
    /// Radius
    float mRadius;

    /// Create the mesh of the gizmo with the indexs, positions and colors
    void CreateMesh();

    /// Update the positions of the vertices of the mesh
    void UpdatePositions();
};

#endif
