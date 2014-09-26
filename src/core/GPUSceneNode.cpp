//Definition include
#include "GPUSceneNode.h"

GPUSceneNode::GPUSceneNode(const GPUGeometry *pGeometry, Material* pMaterial):
    mGeometry(pGeometry), mMaterial(pMaterial), mModelMatrix(), mPolygonalOffset(0)
{

}

GPUSceneNode::~GPUSceneNode()
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

const GPUGeometry* GPUSceneNode::GetGeometry()
{
    return mGeometry;
}

Material* GPUSceneNode::GetMaterial()
{
    return mMaterial;
}

int GPUSceneNode::GetPolygonalOffset() const
{
    return mPolygonalOffset;
}
