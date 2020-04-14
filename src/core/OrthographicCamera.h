#ifndef ORTHOGRAPHIC_CAMERA_H
#define ORTHOGRAPHIC_CAMERA_H

#include "Camera.h"

#include "glm/vec3.hpp"

/// Camera class with methods to configure an orthographic camera
class OrthographicCamera : public Camera
{
public:
    OrthographicCamera(float pNearPlane, float pFarPlane, glm::vec3 pLookAt, glm::vec3 pUp, glm::vec3 pPosition, float pTop, float pBottom, float pLeft, float pRight);
    OrthographicCamera(const OrthographicCamera& pOrthographicCamera);
    ~OrthographicCamera();

    /// Update the projection matrix
    void UpdateProjection() override;

    /// Method to clone the camera
    std::unique_ptr<Camera> Clone() const override;

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
    void CreateMesh() override;

    /// Update the positions of the vertices of the mesh
    void UpdatePositions() override;
};

#endif
