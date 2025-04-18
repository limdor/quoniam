#ifndef GPU_SCENE_NODE_H
#define GPU_SCENE_NODE_H

//GLEW has to be included before any OpenGL include
#include "GL/glew.h"

#include "GPUGeometry.h"
#include "Material.h"

#include "glm/mat4x4.hpp"

#include <memory>

class GPUSceneNode
{
public:
    GPUSceneNode(std::shared_ptr<GPUGeometry const> pGeometry, std::shared_ptr<Material> pMaterial);

    /// Set the model matrix
    void SetModelMatrix(const glm::mat4 &pModelMatrix);
    /// Set the polygonal offset
    void SetPolygonalOffset(size_t pPolygonalOffset);

    /// Get the model matrix
    glm::mat4 GetModelMatrix() const;
    /// Get the gpu geometry
    std::shared_ptr<GPUGeometry const> GetGeometry();
    /// Get the material
    std::shared_ptr<Material> GetMaterial();
    /// Get the polygonal offset
    size_t GetPolygonalOffset() const;

private:
    /// Model matrix
    glm::mat4 mModelMatrix;
    /// Geometry in the gpu
    std::shared_ptr<GPUGeometry const> mGeometry;
    /// Material
    std::shared_ptr<Material> mMaterial;
    /// Polygonal offset in the scene
    size_t mPolygonalOffset;
};

#endif
