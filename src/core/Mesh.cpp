#include "Mesh.h"

Mesh::Mesh(Geometry * pGeometry, Material * pMaterial):
    mGeometry(pGeometry), mMaterial(pMaterial), mVisible(true), mName("")
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
