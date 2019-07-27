/// \file CameraController.h
/// \class CameraController
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

//Project includes
#include "Camera.h"
#include "GLCanvas.h"

/// Generic class to control a camera
class CameraController
{
protected:
    CameraController(){}
    CameraController(GLCanvas* pGLCanvas, Camera* pCamera);

protected:
    Camera* mCamera;
    GLCanvas* mGLCanvas;
};

#endif
