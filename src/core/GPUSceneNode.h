/// \file GPUSceneNode.h
/// \class GPUSceneNode
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef _GPU_SCENE_NODE_H_
#define _GPU_SCENE_NODE_H_

//GLEW has to be included before any OpenGL include
#include "glew.h"

//Dependency includes
#include "glm/mat4x4.hpp"

//Project includes
#include "GPUGeometry.h"
#include "Material.h"

class GPUSceneNode
{
public:
    /// Constructor
    GPUSceneNode(const GPUGeometry* pGeometry, Material* pMaterial);
    /// Destructor
    ~GPUSceneNode();

    /// Set the model matrix
    void SetModelMatrix(const glm::mat4 &pModelMatrix);
    /// Set the polygonal offset
    void SetPolygonalOffset(int pPolygonalOffset);

    /// Get the model matrix
    glm::mat4 GetModelMatrix() const;
    /// Get the gpu geometry
    const GPUGeometry* GetGeometry();
    /// Get the material
    Material* GetMaterial();
    /// Get the polygonal offset
    int GetPolygonalOffset() const;

private:
    /// Model matrix
    glm::mat4 mModelMatrix;
    /// Geometry in the gpu
    const GPUGeometry* mGeometry;
    /// Material
    Material* mMaterial;
    /// Polygonal offset in the scene
    int mPolygonalOffset;
};

#endif
