/// \file GPUSceneNode.h
/// \class GPUSceneNode
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef GPU_SCENE_NODE_H
#define GPU_SCENE_NODE_H

//GLEW has to be included before any OpenGL include
#include "glew.h"

#include "GPUGeometry.h"
#include "Material.h"

#include "glm/mat4x4.hpp"

#include <memory>

class GPUSceneNode
{
public:
    /// Constructor
    GPUSceneNode(std::shared_ptr<GPUGeometry const> pGeometry, Material* pMaterial);
    /// Destructor
    ~GPUSceneNode();

    /// Set the model matrix
    void SetModelMatrix(const glm::mat4 &pModelMatrix);
    /// Set the polygonal offset
    void SetPolygonalOffset(int pPolygonalOffset);

    /// Get the model matrix
    glm::mat4 GetModelMatrix() const;
    /// Get the gpu geometry
    std::shared_ptr<GPUGeometry const> GetGeometry();
    /// Get the material
    Material* GetMaterial();
    /// Get the polygonal offset
    int GetPolygonalOffset() const;

private:
    /// Model matrix
    glm::mat4 mModelMatrix;
    /// Geometry in the gpu
    std::shared_ptr<GPUGeometry const> mGeometry;
    /// Material
    Material* mMaterial;
    /// Polygonal offset in the scene
    int mPolygonalOffset;
};

#endif
