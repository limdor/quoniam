#include "GPUScene.h"

#include "glm/mat4x4.hpp"

GPUScene::GPUScene(const Scene *pScene)
{
    int polygonalOffset = 0;
    mSceneNodes = CreateGPUSceneNodes(pScene->GetRootNode(), polygonalOffset);
}

GPUScene::~GPUScene()
{
    for( int i = 0; i < mSceneNodes.size(); i++ )
    {
        delete mSceneNodes.at(i);
    }
}

int GPUScene::GetNumberOfSceneNodes() const
{
    return mSceneNodes.size();
}

GPUSceneNode* GPUScene::GetSceneNode(int pNode) const
{
    return mSceneNodes.at(pNode);
}

QVector<GPUSceneNode*> GPUScene::CreateGPUSceneNodes(const SceneNode* pSceneNode, int &pPolygonalOffset)
{
    QVector<GPUSceneNode*> sceneNodes;

    glm::mat4 modelMatrix = pSceneNode->GetGlobalTransform();
    for( int i = 0; i < pSceneNode->GetNumMeshes(); i++ )
    {
        const Mesh* mesh = pSceneNode->GetMesh(i);
        GPUSceneNode* gpuSceneNode = new GPUSceneNode(mesh->GetGeometry()->GetGPUGeometry(), mesh->GetMaterial());
        gpuSceneNode->SetModelMatrix(modelMatrix);
        gpuSceneNode->SetPolygonalOffset(pPolygonalOffset);
        sceneNodes += gpuSceneNode;
        pPolygonalOffset += mesh->GetGeometry()->GetNumFaces();
    }

    for( int i = 0; i < pSceneNode->GetNumChilds(); i++ )
    {
        sceneNodes += CreateGPUSceneNodes(pSceneNode->GetChild(i), pPolygonalOffset);
    }
    return sceneNodes;
}
