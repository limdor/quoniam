#ifndef GPU_SCENE_H
#define GPU_SCENE_H

#include "GPUSceneNode.h"
#include "Scene.h"

#include <memory>
#include <vector>

class GPUScene
{
public:
    explicit GPUScene(std::shared_ptr<Scene const> pScene);

    /// Get the number of scene nodes
    size_t GetNumberOfSceneNodes() const;
    /// Get the scene node
    std::shared_ptr<GPUSceneNode> GetSceneNode(size_t pNode) const;

private:
    /// Create the gpu scene nodes given a scene node
    std::vector<std::shared_ptr<GPUSceneNode>> CreateGPUSceneNodes(std::shared_ptr<SceneNode const> pSceneNode, size_t &pPolygonalOffset);

    /// List of gpu scene nodes
    std::vector<std::shared_ptr<GPUSceneNode>> mSceneNodes;
};

#endif
