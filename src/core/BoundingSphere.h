#ifndef BOUNDING_SPHERE_H
#define BOUNDING_SPHERE_H

#include "Gizmo.h"

#include "glm/vec3.hpp"

#include <memory>

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
    static std::shared_ptr<BoundingSphere> Merge(std::shared_ptr<BoundingSphere const> pBS0, std::shared_ptr<BoundingSphere const> pBS1);

protected:
    /// Center
    glm::vec3 mCenter;
    /// Radius
    float mRadius;

    /// Create the mesh of the gizmo with the indexs, positions and colors
    void CreateMesh() override;

    /// Update the positions of the vertices of the mesh
    void UpdatePositions() override;
};

#endif
