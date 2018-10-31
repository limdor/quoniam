/// \file CameraController.h
/// \class CameraController
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef _CAMERA_CONTROLLER_H_
#define _CAMERA_CONTROLLER_H_

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
