//Definition include
#include "Mesh.h"

Mesh::Mesh(Geometry * pGeometry, Material * pMaterial):
    mGeometry(pGeometry), mMaterial(pMaterial), mVisible(true), mName("")
{

}

Mesh::Mesh(const Mesh &pMesh):
    mName(pMesh.mName), mVisible(pMesh.mVisible)
{
    if(pMesh.mMaterial != nullptr)
    {
        mMaterial = new Material(*pMesh.mMaterial);
    }
    else
    {
        mMaterial = nullptr;
    }
    if(pMesh.mGeometry != nullptr)
    {
        mGeometry = new Geometry(*pMesh.mGeometry);
    }
    else
    {
        mGeometry = nullptr;
    }
}

Mesh::~Mesh()
{

}

void Mesh::SetName(const QString &pName)
{
    mName = pName;
}

void Mesh::SetMaterial(Material *pMaterial)
{
    mMaterial = pMaterial;
}

Material* Mesh::GetMaterial() const
{
    return mMaterial;
}

Geometry* Mesh::GetGeometry() const
{
    return mGeometry;
}

void Mesh::SetVisible(bool pVisible)
{
    mVisible = pVisible;
}

void Mesh::Draw()
{
    if(mVisible)
    {
        mGeometry->Draw();
    }
}
