//Definition include
#include "SceneNode.h"

//Project includes
#include "Debug.h"

SceneNode::SceneNode(const QString &pName):
    mName(pName), mLocalTransform(), mGlobalTransform(),
    mNumberOfVertices(0), mNumberOfPolygons(0), mBoundingSphere(NULL),
    mParent(NULL)
{

}

SceneNode::~SceneNode()
{
    for( int i = 0; i < mChilds.size(); i++ )
    {
        delete mChilds[i];
    }
    delete mBoundingSphere;
}

QString SceneNode::GetName() const
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

int SceneNode::GetNumberOfVertices() const
{
    return mNumberOfVertices;
}

int SceneNode::GetNumberOfPolygons() const
{
    return mNumberOfPolygons;
}

const BoundingSphere* SceneNode::GetBoundingSphere() const
{
    return mBoundingSphere;
}

void SceneNode::AddMesh(Mesh* pMesh)
{
    mMeshes.push_back(pMesh);

    Geometry* geometry = pMesh->GetGeometry();
    mNumberOfVertices += geometry->GetNumVertices();
    mNumberOfPolygons += geometry->GetNumFaces();
    BoundingSphere* previousBoundingSphere = mBoundingSphere;
    mBoundingSphere = BoundingSphere::Merge(mBoundingSphere, geometry->GetBoundingSphere());
    if(previousBoundingSphere != NULL)
    {
        delete previousBoundingSphere;
    }
    if(mParent != NULL)
    {
        mParent->UpdateGeometryInformation();
    }
}

void SceneNode::AddCamera(Camera* pCamera)
{
    mCameras.push_back(pCamera);
}

void SceneNode::AddChild(SceneNode* pChild)
{
    mChilds.push_back(pChild);
    pChild->SetParent(this);
}

SceneNode* SceneNode::GetParent() const
{
    return mParent;
}

void SceneNode::SetParent(SceneNode *pParent)
{
    mParent = pParent;
    UpdateGlobalTransform();
    if(mParent != NULL)
    {
        mParent->UpdateGeometryInformation();
    }
}

int SceneNode::GetNumMeshes() const
{
    return mMeshes.size();
}

const Mesh* SceneNode::GetMesh(int pPosition) const
{
    return mMeshes.at(pPosition);
}

int SceneNode::GetNumCameras() const
{
    return mCameras.size();
}

const Camera* SceneNode::GetCamera(int pPosition) const
{
    return mCameras.at(pPosition);
}

int SceneNode::GetNumChilds() const
{
    return mChilds.size();
}

const SceneNode* SceneNode::GetChild(int pPosition) const
{
    return mChilds.at(pPosition);
}

void SceneNode::UpdateGlobalTransform()
{
    if(mParent != NULL)
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
    if(mBoundingSphere != NULL)
    {
        delete mBoundingSphere;
    }
    mBoundingSphere = NULL;
    BoundingSphere* childBoundingSphere = new BoundingSphere();
    for( int i = 0; i < mChilds.size(); i++ )
    {
        const SceneNode* child = mChilds.at(i);
        if(child->GetNumMeshes() != 0)
        {
            mNumberOfVertices += child->GetNumberOfVertices();
            mNumberOfPolygons += child->GetNumberOfPolygons();

            glm::mat4 childTransform = child->GetLocalTransform();
            glm::vec4 aux = childTransform * glm::vec4(child->GetBoundingSphere()->GetCenter(), 1.0);
            childBoundingSphere->SetCenter(glm::vec3(aux.x, aux.y, aux.z));
            childBoundingSphere->SetRadius(childTransform[0][0] * child->GetBoundingSphere()->GetRadius());

            BoundingSphere* previousBoundingSphere = mBoundingSphere;
            mBoundingSphere = BoundingSphere::Merge(mBoundingSphere, childBoundingSphere);
            if(previousBoundingSphere != NULL)
            {
                delete previousBoundingSphere;
            }
        }
    }
    delete childBoundingSphere;
    if(mParent != NULL)
    {
        mParent->UpdateGeometryInformation();
    }
}
