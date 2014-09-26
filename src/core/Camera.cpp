//Definition include
#include "Camera.h"

//Dependency includes
#include <glm/ext.hpp>

Camera::Camera(float pNearPlane, float pFarPlane, const glm::vec3 &pLookAt, const glm::vec3 &pUp, const glm::vec3 &pPosition, float pAspectRatio): Gizmo(),
    mNearPlane(pNearPlane), mFarPlane(pFarPlane), mLookAt(pLookAt), mUp(pUp), mPosition(pPosition), mAspectRatio(pAspectRatio),
    mUpdatedView(false), mUpdatedProjection(false)
{

}

Camera::Camera(const Camera& pCamera): Gizmo(pCamera),
    mNearPlane(pCamera.mNearPlane), mFarPlane(pCamera.mFarPlane), mLookAt(pCamera.mLookAt), mUp(pCamera.mUp), mPosition(pCamera.mPosition), mAspectRatio(pCamera.mAspectRatio),
    mUpdatedView(pCamera.mUpdatedView), mUpdatedProjection(pCamera.mUpdatedProjection),
    mViewMatrix(pCamera.mViewMatrix), mProjectionMatrix(pCamera.mProjectionMatrix)
{

}

Camera::~Camera()
{
}

glm::mat4 Camera::GetViewMatrix()
{
    if(!mUpdatedView)
    {
        UpdateView();
    }

    return mViewMatrix;
}

glm::mat4 Camera::GetProjectionMatrix()
{
    if(!mUpdatedProjection)
    {
        UpdateProjection();
    }

    return mProjectionMatrix;
}

void Camera::SetNearPlane(float pNearPlane)
{
    mNearPlane = pNearPlane;
    mUpdatedProjection = false;
    UpdatePositions();
}

void Camera::SetFarPlane(float pFarPlane)
{
    mFarPlane = pFarPlane;
    mUpdatedProjection = false;
    UpdatePositions();
}

void Camera::SetLookAt(const glm::vec3 &pLookAt)
{
    mLookAt = pLookAt;
    mUpdatedView = false;
    UpdatePositions();
}

void Camera::SetUp(const glm::vec3 &pUp)
{
    mUp = pUp;
    mUpdatedView = false;
    UpdatePositions();
}

void Camera::SetPosition(const glm::vec3 &pPosition)
{
    mPosition = pPosition;
    mUpdatedView = false;
    UpdatePositions();
}

void Camera::SetAspectRatio(float pRatio)
{
    mAspectRatio = pRatio;
    mUpdatedProjection = false;
    UpdatePositions();
}

float Camera::GetNearPlane() const
{
    return mNearPlane;
}

float Camera::GetFarPlane() const
{
    return mFarPlane;
}

glm::vec3 Camera::GetLookAt() const
{
    return mLookAt;
}

glm::vec3 Camera::GetUp() const
{
    return mUp;
}

glm::vec3 Camera::GetPosition() const
{
    return mPosition;
}

float Camera::GetAspectRatio() const
{
    return mAspectRatio;
}

void Camera::UpdateView()
{
    mViewMatrix = glm::lookAt(mPosition, mLookAt, mUp);

    mUpdatedView = true;
}
