#include "GPUSceneNode.h"

GPUSceneNode::GPUSceneNode(std::shared_ptr<GPUGeometry const> pGeometry, std::shared_ptr<Material> pMaterial):
    mGeometry(pGeometry), mMaterial(pMaterial), mModelMatrix(), mPolygonalOffset(0)
{

}

void GPUSceneNode::SetModelMatrix(const glm::mat4 &pModelMatrix)
{
    mModelMatrix = pModelMatrix;
}

void GPUSceneNode::SetPolygonalOffset(int pPolygonalOffset)
{
    mPolygonalOffset = pPolygonalOffset;
}

glm::mat4 GPUSceneNode::GetModelMatrix() const
{
    return mModelMatrix;
}

std::shared_ptr<GPUGeometry const> GPUSceneNode::GetGeometry()
{
    return mGeometry;
}

std::shared_ptr<Material> GPUSceneNode::GetMaterial()
{
    return mMaterial;
}

int GPUSceneNode::GetPolygonalOffset() const
{
    return mPolygonalOffset;
}
