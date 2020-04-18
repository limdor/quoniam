#ifndef SCENE_NODE_H
#define SCENE_NODE_H

#include "Camera.h"
#include "Mesh.h"

#include <QtCore/QString>

#include "glm/mat4x4.hpp"

#include <memory>
#include <vector>

/// Class for a node of a scene
class SceneNode : public std::enable_shared_from_this<SceneNode>
{
public:
    explicit SceneNode(const QString &pName);
    explicit SceneNode(const SceneNode& sceneNode) = delete;
    ~SceneNode() = default;

    /// Get the name of the node
    QString GetName() const;

    /// Set the local transform of the node (scale must be equal in x, y and z)
    void SetLocalTransform(const glm::mat4 &pTransform);
    /// Get the local transform of the node
    glm::mat4 GetLocalTransform() const;
    /// Get the global transform of the node
    glm::mat4 GetGlobalTransform() const;

    /// Get the number of vertices in the node
    size_t GetNumberOfVertices() const;
    /// Get the number of polygons in the node
    size_t GetNumberOfPolygons() const;
    /// Get the bounding sphere of the node
    std::shared_ptr<BoundingSphere const> GetBoundingSphere() const;

    /// Add a mesh into the node
    void AddMesh(std::shared_ptr<Mesh> pMesh);
    /// Add a camera into the node
    void AddCamera(Camera* pCamera);
    /// Add a node child into the node
    void AddChild(std::shared_ptr<SceneNode> pChild);

    /// Get the parent
    std::shared_ptr<SceneNode> GetParent() const;
    /// Set the parent
    void SetParent(std::shared_ptr<SceneNode> pParent);

    /// Obtain the number of meshes
    size_t GetNumMeshes() const;
    /// Obtain the mesh
    std::shared_ptr<Mesh const> GetMesh(size_t pPosition) const;

    /// Obtain the number of cameras
    size_t GetNumCameras() const;
    /// Obtain the camera
    const Camera* GetCamera(size_t pPosition) const;

    /// Obtain the number of childs
    size_t GetNumChilds() const;
    /// Obtain the child
    std::shared_ptr<SceneNode const> GetChild(size_t pPosition) const;

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
    std::shared_ptr<SceneNode> mParent;
    /// Meshes of the node
    std::vector<std::shared_ptr<Mesh>> mMeshes;
    /// Cameras of the node
    std::vector<Camera*> mCameras;
    /// Childs of the node
    std::vector<std::shared_ptr<SceneNode>> mChilds;
};
#endif
