#include "SceneNode.h"

#include "BoundingSphere.h"
#include "Debug.h"

SceneNode::SceneNode(const std::string &pName):
    mName(pName), mLocalTransform(), mGlobalTransform(),
    mNumberOfVertices(0), mNumberOfPolygons(0), mBoundingSphere(nullptr),
    mParent(nullptr)
{

}

std::string SceneNode::GetName() const
{
    return mName;
}

void SceneNode::SetLocalTransform(const glm::mat4 &pTransform)
{
    mLocalTransform = pTransform;
    UpdateGlobalTransform();
}

glm::mat4 SceneNode::GetLocalTransform() const
{
    return mLocalTransform;
}

glm::mat4 SceneNode::GetGlobalTransform() const
{
    return mGlobalTransform;
}

size_t SceneNode::GetNumberOfVertices() const
{
    return mNumberOfVertices;
}

size_t SceneNode::GetNumberOfPolygons() const
{
    return mNumberOfPolygons;
}

std::shared_ptr<BoundingSphere const> SceneNode::GetBoundingSphere() const
{
    return mBoundingSphere;
}

void SceneNode::AddMesh(std::shared_ptr<Mesh> pMesh)
{
    mMeshes.push_back(pMesh);

    auto geometry = pMesh->GetGeometry();
    mNumberOfVertices += geometry->GetNumVertices();
    mNumberOfPolygons += geometry->GetNumFaces();
    mBoundingSphere = BoundingSphere::Merge(mBoundingSphere, geometry->GetBoundingSphere());
    if(mParent != nullptr)
    {
        mParent->UpdateGeometryInformation();
    }
}

void SceneNode::AddCamera(Camera* pCamera)
{
    mCameras.push_back(pCamera);
}

void SceneNode::AddChild(std::shared_ptr<SceneNode> pChild)
{
    if(pChild != nullptr)
    {
        mChilds.push_back(std::move(pChild));
        mChilds.back()->SetParent(shared_from_this());
    }
}

std::shared_ptr<SceneNode> SceneNode::GetParent() const
{
    return mParent;
}

void SceneNode::SetParent(std::shared_ptr<SceneNode> pParent)
{
    mParent = std::move(pParent);
    UpdateGlobalTransform();
    if(mParent != nullptr)
    {
        mParent->UpdateGeometryInformation();
    }
}

size_t SceneNode::GetNumMeshes() const
{
    return mMeshes.size();
}

std::shared_ptr<Mesh const> SceneNode::GetMesh(size_t pPosition) const
{
    return mMeshes.at(pPosition);
}

size_t SceneNode::GetNumCameras() const
{
    return mCameras.size();
}

const Camera* SceneNode::GetCamera(size_t pPosition) const
{
    return mCameras.at(pPosition);
}

size_t SceneNode::GetNumChilds() const
{
    return mChilds.size();
}

std::shared_ptr<SceneNode const> SceneNode::GetChild(size_t pPosition) const
{
    return mChilds.at(pPosition);
}

void SceneNode::UpdateGlobalTransform()
{
    if(mParent != nullptr)
    {
        mGlobalTransform = mParent->GetGlobalTransform() * mLocalTransform;
    }
    else
    {
        mGlobalTransform = mLocalTransform;
    }
    for(int i = 0; i < mChilds.size(); i++ )
    {
        mChilds.at(i)->UpdateGlobalTransform();
    }
}

void SceneNode::UpdateGeometryInformation()
{
    mNumberOfVertices = 0;
    mNumberOfPolygons = 0;
    auto childBoundingSphere = std::make_shared<BoundingSphere>();
    for( int i = 0; i < mChilds.size(); i++ )
    {
        auto child = mChilds.at(i);
        if(child->GetNumMeshes() != 0)
        {
            mNumberOfVertices += child->GetNumberOfVertices();
            mNumberOfPolygons += child->GetNumberOfPolygons();

            glm::mat4 childTransform = child->GetLocalTransform();
            glm::vec4 aux = childTransform * glm::vec4(child->GetBoundingSphere()->GetCenter(), 1.0);
            childBoundingSphere->SetCenter(glm::vec3(aux.x, aux.y, aux.z));
            childBoundingSphere->SetRadius(childTransform[0][0] * child->GetBoundingSphere()->GetRadius());

            mBoundingSphere = BoundingSphere::Merge(mBoundingSphere, childBoundingSphere);
        }
    }
    if(mParent != nullptr)
    {
        mParent->UpdateGeometryInformation();
    }
}
