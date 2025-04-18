#include "TrackballCamera.h"

#include "glm/exponential.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#include "glm/gtc/matrix_transform.hpp"

void TrackballCamera::MoveCamera(const glm::vec2& pMouseInitialPosition, const glm::vec2& pMouseFinalPosition, Camera& pCamera, const glm::vec3& pCenter)
{
    const glm::vec2 moviment{pMouseFinalPosition - pMouseInitialPosition};
    if( moviment != glm::vec2{0, 0} )
    {
        const glm::vec3 prevCamPosition = pCamera.GetPosition();
        const glm::vec3 prevCamUpVector = glm::normalize( pCamera.GetUp() );
        const glm::vec3 prevCamFrontVector = glm::normalize( pCamera.GetLookAt() - prevCamPosition );

        const float rotationAmount = glm::sqrt(moviment.x*(float)moviment.x + moviment.y*moviment.y / 5.0f);
        const glm::vec3 leftVector = glm::normalize( glm::cross(prevCamUpVector, prevCamFrontVector) );
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), pCenter);
        const glm::vec3 rotationVector = glm::normalize(prevCamUpVector*-(float)moviment.x + leftVector*(float)moviment.y);
        transform = glm::rotate(transform, glm::radians((float)rotationAmount), rotationVector);
        transform = glm::translate(transform, -pCenter);
        glm::vec4 aux = transform * glm::vec4(prevCamUpVector, 0.0f);
        pCamera.SetUp(glm::vec3(aux.x, aux.y, aux.z));
        aux = transform * glm::vec4(prevCamPosition, 1.0f);
        pCamera.SetPosition(glm::vec3(aux.x, aux.y, aux.z));
    }
}
