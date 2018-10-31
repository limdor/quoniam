/// \file PerspectiveCamera.h
/// \class PerspectiveCamera
/// \author Xavier Bonaventura
/// \author Nicolau Sunyer
/// \author Copyright: (c) Universitat de Girona

#ifndef _PERSPECTIVE_CAMERA_H_
#define _PERSPECTIVE_CAMERA_H_

//Dependency includes
#include "glm/vec3.hpp"

//Project includes
#include "Camera.h"

/// Camera class with methods to configure a perspective camera
class PerspectiveCamera : public Camera
{
public:
    /// Constructor
    PerspectiveCamera(float pNearPlane, float pFarPlane, glm::vec3 pLookAt, glm::vec3 pUp, glm::vec3 pPosition, float pAngle, float pAspectRatio);
    /// Copy constructor
    PerspectiveCamera(const PerspectiveCamera& pPerspectiveCamera);
    /// Destructor
    ~PerspectiveCamera();

    /// Update the projection matrix
    void UpdateProjection();

    /// Method to clone the camera
    PerspectiveCamera* Clone() const;

    /// Set the field of view angle
    void SetAngle(float pAngle);
    /// Get the field of view angle
    float GetAngle() const;

protected:
    /// Field of view angle
    float mAngle;

    /// Create the mesh of the gizmo with the indexs, positions and colors
    void CreateMesh();

    /// Update the positions of the vertices of the mesh
    void UpdatePositions();
};

#endif
