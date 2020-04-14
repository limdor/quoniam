#ifndef AXIS_ALIGNED_BOUNDING_BOX_H
#define AXIS_ALIGNED_BOUNDING_BOX_H

#include "Gizmo.h"

#include "glm/vec3.hpp"

#include <memory>

/// BoundingVolume class with methods to use an axis-aligned bounding box
class AxisAlignedBoundingBox : public Gizmo
{
public:
    /// Constructor
    AxisAlignedBoundingBox();
    /// Destructor
    ~AxisAlignedBoundingBox();

    /// Set minimum of every axis
    void SetMin(const glm::vec3 &pValue);
    /// Set maximum of every axis
    void SetMax(const glm::vec3 &pValue);

    /// Get minimum of every axis
    glm::vec3 GetMin() const;
    /// Get maximum of every axis
    glm::vec3 GetMax() const;

    /// Create the minimum axis-aligned bounding box that includes pAABB0 and pAABB1
    static std::shared_ptr<AxisAlignedBoundingBox> Merge(std::shared_ptr<AxisAlignedBoundingBox const> pAABB0, std::shared_ptr<AxisAlignedBoundingBox const> pAABB1);

protected:
    /// Minimum of every axis
    glm::vec3 mMin;
    /// Maximum of every axis
    glm::vec3 mMax;

    /// Create the mesh of the gizmo with the indexs, positions and colors
    void CreateMesh() override;

    /// Update the positions of the vertices of the mesh
    void UpdatePositions() override;
};

#endif
