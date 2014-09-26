#include "TrackballCamera.h"

#define GLM_FORCE_RADIANS
#include "glm/exponential.hpp"
#include "glm/geometric.hpp"
#include "glm/gtx/transform.hpp"

TrackballCamera::TrackballCamera(GLCanvas *pGLCanvas, Camera *pCamera):CameraController(pGLCanvas, pCamera)
{
}

void TrackballCamera::SetCenter(const glm::vec3& pCenter)
{
    mCenter = pCenter;
}

void TrackballCamera::SetRadius(float pRadius)
{
    mRadius = pRadius;
}

QPoint TrackballCamera::MoveCamera(QPoint pMouseInitialPosition, QPoint pMouseFinalPosition)
{
    QPoint moviment = pMouseFinalPosition - pMouseInitialPosition;
    if( moviment != QPoint(0,0) )
    {
        glm::vec3 prevCamPosition = mCamera->GetPosition();
        glm::vec3 prevCamUpVector = glm::normalize( mCamera->GetUp() );
        glm::vec3 prevCamFrontVector = glm::normalize( mCamera->GetLookAt() - prevCamPosition );

        float rotationAmount = glm::sqrt(moviment.x()*(float)moviment.x() + moviment.y()*moviment.y()) / 5.0f;
        glm::vec3 leftVector = glm::normalize( glm::cross(prevCamUpVector, prevCamFrontVector) );
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), mCenter);
        glm::vec3 rotationVector = glm::normalize(prevCamUpVector*-(float)moviment.x() + leftVector*(float)moviment.y());
        transform = glm::rotate(transform, glm::radians((float)rotationAmount), rotationVector);
        transform = glm::translate(transform, -mCenter);
        glm::vec4 aux = transform * glm::vec4(prevCamUpVector, 0.0f);
        mCamera->SetUp(glm::vec3(aux.x, aux.y, aux.z));
        aux = transform * glm::vec4(prevCamPosition, 1.0f);
        mCamera->SetPosition(glm::vec3(aux.x, aux.y, aux.z));
    }
    return moviment;
}
