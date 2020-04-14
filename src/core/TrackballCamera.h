#ifndef TRACKBALL_CAMERA_H
#define TRACKBALL_CAMERA_H

#include "Camera.h"

class TrackballCamera
{
public:
    static void MoveCamera(const glm::vec2& pMouseInitialPosition, const glm::vec2& pMouseFinalPosition, Camera& pCamera, const glm::vec3& pCenter);
};

#endif
