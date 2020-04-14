/// \file SceneNode.h
/// \class SceneNode
/// \author Xavier Bonaventura
/// \author Nicolau Sunyer
/// \author Copyright: (c) Universitat de Girona

#ifndef SCENE_NODE_H
#define SCENE_NODE_H

#include "Camera.h"
#include "Mesh.h"

#include <QtCore/QString>
#include <QtCore/QVector>

#include "glm/mat4x4.hpp"

#include <memory>

/// Class for a node of a scene
class SceneNode
{
public:
    /// Constructor with the name of the node
    explicit SceneNode(const QString &pName);
    explicit SceneNode(const SceneNode& sceneNode) = delete;
    /// Destructor
    ~SceneNode();

    /// Get the name of the node
    QString GetName() const;

    /// Set the local transform of the node (scale must be equal in x, y and z)
    void SetLocalTransform(const glm::mat4 &pTransform);
    /// Get the local transform of the node
    glm::mat4 GetLocalTransform() const;
    /// Get the global transform of the node
    glm::mat4 GetGlobalTransform() const;

    /// Get the number of vertices in the node
    int GetNumberOfVertices() const;
    /// Get the number of polygons in the node
    int GetNumberOfPolygons() const;
    /// Get the bounding sphere of the node
    std::shared_ptr<BoundingSphere const> GetBoundingSphere() const;

    /// Add a mesh into the node
    void AddMesh(std::shared_ptr<Mesh> pMesh);
    /// Add a camera into the node
    void AddCamera(Camera* pCamera);
    /// Add a node child into the node
    void AddChild(SceneNode* pChild);

    /// Get the parent
    SceneNode* GetParent() const;
    /// Set the parent
    void SetParent(SceneNode* pParent);

    /// Obtain the number of meshes
    int GetNumMeshes() const;
    /// Obtain the mesh
    std::shared_ptr<Mesh const> GetMesh(int pPosition) const;

    /// Obtain the number of cameras
    int GetNumCameras() const;
    /// Obtain the camera
    const Camera* GetCamera(int pPosition) const;

    /// Obtain the number of childs
    int GetNumChilds() const;
    /// Obtain the child
    const SceneNode* GetChild(int pPosition) const;

private:
    /// Update global transform
    void UpdateGlobalTransform();
    /// Update geometry information
    void UpdateGeometryInformation();

    /// Name of the node
    QString mName;
    /// Local transform
    glm::mat4 mLocalTransform;
    /// Global transform
    glm::mat4 mGlobalTransform;

    /// Number of vertices
    size_t mNumberOfVertices;
    /// Number of polygons
    size_t mNumberOfPolygons;
    /// Bounding sphere
    std::shared_ptr<BoundingSphere> mBoundingSphere;

    /// Parent node
    SceneNode* mParent;
    /// Meshes of the node
    QVector<std::shared_ptr<Mesh>> mMeshes;
    /// Cameras of the node
    QVector<Camera*> mCameras;
    /// Childs of the node
    QVector<SceneNode*> mChilds;
};
#endif
