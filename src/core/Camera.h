/// \file Camera.h
/// \class Camera
/// \author Xavier Bonaventura
/// \author Nicolau Sunyer
/// \author Copyright: (c) Universitat de Girona

#ifndef CAMERA_H
#define CAMERA_H

#include "Gizmo.h"

#include <QtCore/QString>

#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"

/// Abstract camera class with methods to configure a generic camera
class Camera : public Gizmo
{
public:
    /// Constructor
    Camera(float pNearPlane, float pFarPlane, const glm::vec3 &pLookAt, const glm::vec3 &pUp, const glm::vec3 &pPosition, float pAspectRatio);
    /// Copy constructor
    Camera(const Camera& pCamera) = default;
    /// Move constructor
    Camera(Camera&& pCamera) = default;
    /// Destructor
    virtual ~Camera() = default;

    /// Get the view matrix
    glm::mat4 GetViewMatrix();
    /// Get the projection matrix
    glm::mat4 GetProjectionMatrix();

    /// Set the near plane
    void SetNearPlane(float pNearPlane);
    /// Set the far plane
    void SetFarPlane(float pFarPlane);
    /// Set the position where the camera is looking
    void SetLookAt(const glm::vec3 &pLookAt);
    /// Set the up vector
    void SetUp(const glm::vec3 &pUp);
    /// Set the position
    void SetPosition(const glm::vec3 &pPosition);
    /// Set the aspect ratio
    void SetAspectRatio(float pAspectRatio);

    /// Get the near plane
    float GetNearPlane() const;
    /// Get the far plane
    float GetFarPlane() const;
    /// Get the position where the camera is looking
    glm::vec3 GetLookAt() const;
    /// Get the up vector
    glm::vec3 GetUp() const;
    /// Get the position
    glm::vec3 GetPosition() const;
    /// Get the aspect ratio
    float GetAspectRatio() const;

    /// Update the view matrix
    void UpdateView();
    /// Update the projection matrix
    virtual void UpdateProjection() = 0;

    /// Method to clone a camera
    virtual Camera* Clone() const = 0;

    /// Name of the camera
    QString mName;

protected:
    /// Boolean to know if the view matrix is updated
    bool mUpdatedView;
    /// Boolean to know if the projection matrix is updated
    bool mUpdatedProjection;
    /// View matrix
    glm::mat4 mViewMatrix;
    /// Projection matrix
    glm::mat4 mProjectionMatrix;

    /// Near plane
    float mNearPlane;
    /// Far plane
    float mFarPlane;
    /// Position where the camera is looking
    glm::vec3 mLookAt;
    /// Up vector
    glm::vec3 mUp;
    /// Position
    glm::vec3 mPosition;
    /// Aspect ratio
    float mAspectRatio;
};

#endif
