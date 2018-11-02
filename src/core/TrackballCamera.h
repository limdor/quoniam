/// \file TrackballCamera.h
/// \class TrackballCamera
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef TRACKBALL_CAMERA_H
#define TRACKBALL_CAMERA_H

//Dependency includes
#include "glm/vec3.hpp"

//Project includes
#include "CameraController.h"

class TrackballCamera : public CameraController
{
public:
    TrackballCamera(GLCanvas* pGLCanvas, Camera* pCamera);
    void SetCenter(const glm::vec3& pCenter);
    void SetRadius(float pRadius);
    QPoint MoveCamera(QPoint pMouseInitialPosition, QPoint pMouseFinalPosition);

private:
    float mRadius;
    glm::vec3 mCenter;
};

#endif
