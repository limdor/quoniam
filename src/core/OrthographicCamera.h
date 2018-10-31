/// \file OrthographicCamera.h
/// \class OrthographicCamera
/// \author Xavier Bonaventura
/// \author Nicolau Sunyer
/// \author Copyright: (c) Universitat de Girona

#ifndef _ORTHOGRAPHIC_CAMERA_H_
#define _ORTHOGRAPHIC_CAMERA_H_

//Dependency includes
#include "glm/vec3.hpp"

//Project includes
#include "Camera.h"

/// Camera class with methods to configure an orthographic camera
class OrthographicCamera : public Camera
{
public:
    /// Constructor
    OrthographicCamera(float pNearPlane, float pFarPlane, glm::vec3 pLookAt, glm::vec3 pUp, glm::vec3 pPosition, float pTop, float pBottom, float pLeft, float pRight);
    /// Copy constructor
    OrthographicCamera(const OrthographicCamera& pOrthographicCamera);
    /// Destructor
    ~OrthographicCamera();

    /// Update the projection matrix
    void UpdateProjection();

    /// Method to clone the camera
    OrthographicCamera* Clone() const;

    /// Set the top value
    void SetTop(float pTop);
    /// Set the bottom value
    void SetBottom(float pBottom);
    /// Set the left value
    void SetLeft(float pLeft);
    /// Set the right value
    void SetRight(float pRight);

private:
    /// Top value
    float mTop;
    /// Bottom value
    float mBottom;
    /// Left value
    float mLeft;
    /// Right value
    float mRight;

    /// Create the mesh of the gizmo with the indexs, positions and colors
    void CreateMesh();

    /// Update the positions of the vertices of the mesh
    void UpdatePositions();
};

#endif
