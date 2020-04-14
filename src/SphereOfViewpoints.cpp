//Definition include
#include "SphereOfViewpoints.h"

//Project includes
#include "PerspectiveCamera.h"

SphereOfViewpoints::SphereOfViewpoints(float pAngle, float pAspectRatio):
    mAngle(pAngle), mAspectRatio(pAspectRatio)
{
    mMesh->SetName("Sphere of viewpoints");
}

void SphereOfViewpoints::SetToUniform4()
{
    SpherePointCloud::SetToUniform4();
    SetCameras();
}

void SphereOfViewpoints::SetToUniform6()
{
    SpherePointCloud::SetToUniform6();
    SetCameras();
}

void SphereOfViewpoints::SetToUniform8()
{
    SpherePointCloud::SetToUniform8();
    SetCameras();
}

void SphereOfViewpoints::SetToUniform12()
{
    SpherePointCloud::SetToUniform12();
    SetCameras();
}

void SphereOfViewpoints::SetToUniform20()
{
    SpherePointCloud::SetToUniform20();
    SetCameras();
}

void SphereOfViewpoints::SetToQuasiUniform(unsigned char pDepth)
{
    SpherePointCloud::SetToQuasiUniform(pDepth);
    SetCameras();
}

std::shared_ptr<Camera> SphereOfViewpoints::GetViewpoint(int pIndex) const
{
    return mCameras.at(pIndex);
}

int SphereOfViewpoints::GetNumberOfViewpoints() const
{
    return mCameras.size();
}

float SphereOfViewpoints::GetAspectRatio() const
{
    return mAspectRatio;
}

void SphereOfViewpoints::ApplyTransform(float pSceneRadius, float pSphereRadius, const glm::vec3& pCenter)
{
    for( int i = 0; i < mNumberOfPoints; i++ )
    {
        glm::vec3 cameraPosition= mVertices.at(i) * pSphereRadius + pCenter;
        mVertices[i] = mVertices.at(i) * pSceneRadius * 2.0f + pCenter;
        auto cam = mCameras.at(i);
        cam->SetNearPlane(qMax(pSphereRadius - 2.0f * pSceneRadius, pSphereRadius / 1000.0f));
        cam->SetFarPlane(pSphereRadius * 2.0f);
        cam->SetLookAt(pCenter);
        cam->SetPosition(cameraPosition);
    }
    SetMeshInformation();
}

void SphereOfViewpoints::SetCameras()
{
    mCameras.resize(mNumberOfPoints);
    for( int i = 0; i < mNumberOfPoints; i++ )
    {
        mCameras[i] = std::make_shared<PerspectiveCamera>( 0.05f, 2.0f, glm::vec3(0.0f), SpherePointCloud::Up(mVertices.at(i)), mVertices.at(i), mAngle, mAspectRatio );
        mCameras[i]->mName = QString("%1").arg(i);
    }
}
