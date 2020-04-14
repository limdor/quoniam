#include "GPUScene.h"

#include "glm/mat4x4.hpp"

GPUScene::GPUScene(std::shared_ptr<Scene const> pScene)
{
    int polygonalOffset = 0;
    mSceneNodes = CreateGPUSceneNodes(pScene->GetRootNode(), polygonalOffset);
}

int GPUScene::GetNumberOfSceneNodes() const
{
    return mSceneNodes.size();
}

std::shared_ptr<GPUSceneNode> GPUScene::GetSceneNode(int pNode) const
{
    return mSceneNodes.at(pNode);
}

QVector<std::shared_ptr<GPUSceneNode>> GPUScene::CreateGPUSceneNodes(std::shared_ptr<SceneNode const> pSceneNode, int &pPolygonalOffset)
{
    QVector<std::shared_ptr<GPUSceneNode>> sceneNodes;

    glm::mat4 modelMatrix = pSceneNode->GetGlobalTransform();
    for( int i = 0; i < pSceneNode->GetNumMeshes(); i++ )
    {
        auto mesh = pSceneNode->GetMesh(i);
        auto gpuSceneNode = std::make_shared<GPUSceneNode>(mesh->GetGeometry()->GetGPUGeometry(), mesh->GetMaterial());
        gpuSceneNode->SetModelMatrix(modelMatrix);
        gpuSceneNode->SetPolygonalOffset(pPolygonalOffset);
        sceneNodes += std::move(gpuSceneNode);
        pPolygonalOffset += mesh->GetGeometry()->GetNumFaces();
    }

    for( int i = 0; i < pSceneNode->GetNumChilds(); i++ )
    {
        sceneNodes += CreateGPUSceneNodes(pSceneNode->GetChild(i), pPolygonalOffset);
    }
    return sceneNodes;
}
