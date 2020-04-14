/// \file PerspectiveCamera.h
/// \class PerspectiveCamera
/// \author Xavier Bonaventura
/// \author Nicolau Sunyer
/// \author Copyright: (c) Universitat de Girona

#ifndef PERSPECTIVE_CAMERA_H
#define PERSPECTIVE_CAMERA_H

#include "Camera.h"

#include "glm/vec3.hpp"

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
    void UpdateProjection() override;

    /// Method to clone the camera
    std::unique_ptr<Camera> Clone() const override;

    /// Set the field of view angle
    void SetAngle(float pAngle);
    /// Get the field of view angle
    float GetAngle() const;

protected:
    /// Field of view angle
    float mAngle;

    /// Create the mesh of the gizmo with the indexs, positions and colors
    void CreateMesh() override;

    /// Update the positions of the vertices of the mesh
    void UpdatePositions() override;
};

#endif
