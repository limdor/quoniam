//Definition include
#include "OrthographicCamera.h"

//Dependency includes
#include "glm/geometric.hpp"
#include "glm/gtc/matrix_transform.hpp"

//Project includes
#include "Geometry.h"

OrthographicCamera::OrthographicCamera(float pNearPlane, float pFarPlane, glm::vec3 pLookAt, glm::vec3 pUp, glm::vec3 pPosition, float pTop, float pBottom, float pLeft, float pRight): Camera(pNearPlane, pFarPlane, pLookAt, pUp, pPosition, (pRight - pLeft) / (pTop - pBottom)),
    mTop(pTop), mBottom(pBottom), mLeft(pLeft), mRight(pRight)
{
    CreateMesh();
}

OrthographicCamera::OrthographicCamera(const OrthographicCamera& pOrthographicCamera): Camera(pOrthographicCamera)
{
    mTop = pOrthographicCamera.mTop;
    mBottom = pOrthographicCamera.mBottom;
    mLeft = pOrthographicCamera.mLeft;
    mRight = pOrthographicCamera.mRight;
}

OrthographicCamera::~OrthographicCamera()
{
}

void OrthographicCamera::SetTop(float pTop)
{
	mTop = pTop;
	mUpdatedProjection = false;
    UpdatePositions();
}

void OrthographicCamera::SetBottom(float pBottom)
{
	mBottom = pBottom;
	mUpdatedProjection = false;
    UpdatePositions();
}

void OrthographicCamera::SetLeft(float pLeft)
{
	mLeft = pLeft;
	mUpdatedProjection = false;
    UpdatePositions();
}

void OrthographicCamera::SetRight(float pRight)
{
	mRight = pRight;
	mUpdatedProjection = false;
    UpdatePositions();
}

void OrthographicCamera::UpdateProjection()
{
	mProjectionMatrix = glm::ortho(mLeft, mRight, mBottom, mTop, mNearPlane, mFarPlane);
	mUpdatedProjection = true;
}

OrthographicCamera* OrthographicCamera::Clone() const
{
    return new OrthographicCamera(*this);
}

void OrthographicCamera::CreateMesh()
{
    /// Creation of the mesh
    mGizmo = new Geometry("OrthographicCamera", Geometry::Lines);

    /// Set the positions
    mPositionOfVertices.resize(8);
    UpdatePositions();

    /// Set the colors
    QVector<glm::vec4> colors(mPositionOfVertices.size(), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
    colors[4] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    colors[5] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    mGizmo->SetColorData(colors.size(), colors.data());

    /// Set the indexs
    QVector<unsigned int> indexs(24);
    indexs[0] = 0; indexs[1] = 1;
    indexs[2] = 1; indexs[3] = 2;
    indexs[4] = 2; indexs[5] = 3;
    indexs[6] = 3; indexs[7] = 0;
    indexs[8] = 4; indexs[9] = 5;
    indexs[10] = 5; indexs[11] = 6;
    indexs[12] = 6; indexs[13] = 7;
    indexs[14] = 7; indexs[15] = 4;
    indexs[16] = 0; indexs[17] = 4;
    indexs[18] = 1; indexs[19] = 5;
    indexs[20] = 2; indexs[21] = 6;
    indexs[22] = 3; indexs[23] = 7;
    mGizmo->SetIndexsData(indexs.size(), indexs.data());
}

void OrthographicCamera::UpdatePositions()
{
    glm::vec3 frontVector = glm::normalize(mLookAt - mPosition);
    glm::vec3 upVector = glm::normalize(mUp);
    glm::vec3 leftVector = glm::normalize(glm::cross(upVector, frontVector));
    upVector = glm::normalize(glm::cross(frontVector, leftVector));
    mPositionOfVertices[0] = glm::vec4(mPosition + upVector * mTop + leftVector * mLeft, 1.0f);
    mPositionOfVertices[1] = glm::vec4(mPosition + upVector * mTop - leftVector * mRight, 1.0f);
    mPositionOfVertices[2] = glm::vec4(mPosition - upVector * mBottom - leftVector * mRight, 1.0f);
    mPositionOfVertices[3] = glm::vec4(mPosition - upVector * mBottom + leftVector * mLeft, 1.0f);
    mPositionOfVertices[4] = glm::vec4(mPosition + upVector * mTop + leftVector * mLeft + frontVector, 1.0f);
    mPositionOfVertices[5] = glm::vec4(mPosition + upVector * mTop - leftVector * mRight + frontVector, 1.0f);
    mPositionOfVertices[6] = glm::vec4(mPosition - upVector * mBottom - leftVector * mRight + frontVector, 1.0f);
    mPositionOfVertices[7] = glm::vec4(mPosition - upVector * mBottom + leftVector * mLeft + frontVector, 1.0f);

    mGizmo->SetVerticesData(mPositionOfVertices.size(), mPositionOfVertices.data());
}
