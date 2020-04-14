#ifndef GPU_SCENE_H
#define GPU_SCENE_H

#include "GPUSceneNode.h"
#include "Scene.h"

#include <memory>

class GPUScene
{
public:
    explicit GPUScene(std::shared_ptr<Scene const> pScene);

    /// Get the number of scene nodes
    int GetNumberOfSceneNodes() const;
    /// Get the scene node
    std::shared_ptr<GPUSceneNode> GetSceneNode(int pNode) const;

private:
    /// Create the gpu scene nodes given a scene node
    QVector<std::shared_ptr<GPUSceneNode>> CreateGPUSceneNodes(std::shared_ptr<SceneNode const> pSceneNode, int &pPolygonalOffset);

    /// List of gpu scene nodes
    QVector<std::shared_ptr<GPUSceneNode>> mSceneNodes;
};

#endif
