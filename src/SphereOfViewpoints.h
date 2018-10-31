/// \file SphereOfViewpoints.h
/// \class SphereOfViewpoints
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef SPHERE_OF_VIEWPOINTS_H
#define SPHERE_OF_VIEWPOINTS_H

//Dependency includes
#include "glm/vec3.hpp"

//Project includes
#include "Camera.h"
#include "SpherePointCloud.h"

class SphereOfViewpoints : public SpherePointCloud
{
public:
    /// Constructor
    SphereOfViewpoints(float pAngle, float pAspectRatio);
    /// Destructor
    ~SphereOfViewpoints();

    /// Generates 4 viewpoints uniformly distributed
    void SetToUniform4();
    /// Generates 6 viewpoints uniformly distributed
    void SetToUniform6();
    /// Generates 8 viewpoints uniformly distributed
    void SetToUniform8();
    /// Generates 12 viewpoints uniformly distributed
    void SetToUniform12();
    /// Generates 20 viewpoints uniformly distributed
    void SetToUniform20();
    /// Generate 10*4^depth+2 viewpoints quasi-uniformly distributed using the recursive
    /// subdivision of faces, starting from an icosahedron
    void SetToQuasiUniform(unsigned char pDepth);
    /// Get the pIndex viewpoint
    Camera* GetViewpoint(int pIndex) const;
    /// Return the numer of viewpoints over the sphere
    int GetNumberOfViewpoints() const;
    /// Return the aspect ratio of the viewpoints
    float GetAspectRatio() const;
    /// Apply a transform acording to the radius and the center of the scene
    void ApplyTransform(float pSceneRadius, float pSphereRadius, const glm::vec3& pCenter);

protected:
    /// Set a camera for every point
    void SetCameras();
    /// Field of view
    float mAngle;
    /// Aspect ratio
    float mAspectRatio;
    /// Array of viewpoints
    QVector< Camera * > mCameras;
};

#endif
