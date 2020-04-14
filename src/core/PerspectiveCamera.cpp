#include "PerspectiveCamera.h"

#include "Geometry.h"

#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#include "glm/gtc/matrix_transform.hpp"

PerspectiveCamera::PerspectiveCamera(float pNearPlane, float pFarPlane, glm::vec3 pLookAt, glm::vec3 pUp, glm::vec3 pPosition, float pAngle, float pAspectRatio): Camera(pNearPlane, pFarPlane, pLookAt, pUp, pPosition, pAspectRatio),
    mAngle(pAngle)
{
    CreateMesh();
}

PerspectiveCamera::PerspectiveCamera(const PerspectiveCamera& pPerspectiveCamera): Camera(pPerspectiveCamera)
{
    mAngle = pPerspectiveCamera.mAngle;
}

PerspectiveCamera::~PerspectiveCamera()
{
}

void PerspectiveCamera::SetAngle(float pAngle)
{
	mAngle = pAngle;
	mUpdatedProjection = false;
    UpdatePositions();
}

float PerspectiveCamera::GetAngle() const
{
    return mAngle;
}

void PerspectiveCamera::UpdateProjection()
{
    mProjectionMatrix = glm::perspective(glm::radians(mAngle), mAspectRatio, mNearPlane, mFarPlane);
	mUpdatedProjection = true;
}

std::unique_ptr<Camera> PerspectiveCamera::Clone() const
{
    return std::make_unique<PerspectiveCamera>(*this);
}

void PerspectiveCamera::CreateMesh()
{
    /// Creation of the mesh
    mGizmo = Geometry{"PerspectiveCamera", GeometryTopology::Lines};

    /// Set the positions
    mPositionOfVertices.resize(5);
    UpdatePositions();

    /// Set the colors
    QVector<glm::vec4> colors(mPositionOfVertices.size(), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
    colors[0] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    colors[1] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    mGizmo.SetColorData(colors.size(), colors.data());

    /// Set the indexs
    QVector<unsigned int> indexs(16);
    indexs[0] = 0; indexs[1] = 1;
    indexs[2] = 1; indexs[3] = 2;
    indexs[4] = 2; indexs[5] = 3;
    indexs[6] = 3; indexs[7] = 0;
    indexs[8] = 0; indexs[9] = 4;
    indexs[10] = 1; indexs[11] = 4;
    indexs[12] = 2; indexs[13] = 4;
    indexs[14] = 3; indexs[15] = 4;
    mGizmo.SetIndexsData(indexs.size(), indexs.data());
}

void PerspectiveCamera::UpdatePositions()
{
    glm::vec3 frontVector = glm::normalize(mLookAt - mPosition);
    glm::vec3 upVector = glm::normalize(mUp);
    glm::vec3 leftVector = glm::normalize(glm::cross(upVector, frontVector));
    upVector = glm::normalize(glm::cross(frontVector, leftVector));
    frontVector = frontVector / glm::tan(glm::radians(mAngle/2.0f));
    mPositionOfVertices[0] = glm::vec4(mPosition + frontVector + upVector + leftVector * mAspectRatio, 1.0f);
    mPositionOfVertices[1] = glm::vec4(mPosition + frontVector + upVector - leftVector * mAspectRatio, 1.0f);
    mPositionOfVertices[2] = glm::vec4(mPosition + frontVector - upVector - leftVector * mAspectRatio, 1.0f);
    mPositionOfVertices[3] = glm::vec4(mPosition + frontVector - upVector + leftVector * mAspectRatio, 1.0f);
    mPositionOfVertices[4] = glm::vec4(mPosition, 1.0f);

    mGizmo.SetVerticesData(mPositionOfVertices.size(), mPositionOfVertices.data());
}
