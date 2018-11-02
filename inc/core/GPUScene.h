/// \file GPUScene.h
/// \class GPUScene
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef GPU_SCENE_H
#define GPU_SCENE_H

//Project includes
#include "GPUSceneNode.h"
#include "Scene.h"

class GPUScene
{
public:
    /// Constructor
    explicit GPUScene(const Scene *pScene);
    /// Destructor
    ~GPUScene();
    /// Get the number of scene nodes
    int GetNumberOfSceneNodes() const;
    /// Get the scene node
    GPUSceneNode* GetSceneNode(int pNode) const;

private:
    /// Create the gpu scene nodes given a scene node
    QVector<GPUSceneNode*> CreateGPUSceneNodes(const SceneNode* pSceneNode, int &pPolygonalOffset);

    /// List of gpu scene nodes
    QVector<GPUSceneNode*> mSceneNodes;
};

#endif
