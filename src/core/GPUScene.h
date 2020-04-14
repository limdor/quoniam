#ifndef GPU_SCENE_H
#define GPU_SCENE_H

#include "GPUSceneNode.h"
#include "Scene.h"

class GPUScene
{
public:
    /// Constructor
    explicit GPUScene(std::shared_ptr<Scene const> pScene);
    /// Destructor
    ~GPUScene();
    /// Get the number of scene nodes
    int GetNumberOfSceneNodes() const;
    /// Get the scene node
    GPUSceneNode* GetSceneNode(int pNode) const;

private:
    /// Create the gpu scene nodes given a scene node
    QVector<GPUSceneNode*> CreateGPUSceneNodes(std::shared_ptr<SceneNode const> pSceneNode, int &pPolygonalOffset);

    /// List of gpu scene nodes
    QVector<GPUSceneNode*> mSceneNodes;
};

#endif
