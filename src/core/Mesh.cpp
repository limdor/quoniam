#include "Mesh.h"

Mesh::Mesh(std::shared_ptr<Geometry> pGeometry, std::shared_ptr<Material> pMaterial):
    mGeometry(pGeometry), mMaterial(pMaterial), mVisible(true), mName("")
{

}

void Mesh::SetName(const QString &pName)
{
    mName = pName;
}

void Mesh::SetMaterial(std::shared_ptr<Material> pMaterial)
{
    mMaterial = pMaterial;
}

std::shared_ptr<Material> Mesh::GetMaterial() const
{
    return mMaterial;
}

std::shared_ptr<Geometry> Mesh::GetGeometry() const
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
