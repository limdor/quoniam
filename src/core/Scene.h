/// \file Scene.h
/// \class Scene
/// \author Xavier Bonaventura
/// \author Nicolau Sunyer
/// \author Copyright: (c) Universitat de Girona

#ifndef SCENE_H
#define SCENE_H

//Qt includes
#include <QString>

//Project includes
#include "BoundingSphere.h"
#include "SceneNode.h"

/// Class representing a scene
class Scene
{
public:
    /// Create an scene given the name and the root scene node
    Scene(const QString &pName, SceneNode *pSceneRoot, const QVector<Material*>& pMaterials, const QVector<Geometry*>& pGeometries, const QVector<Mesh*>& pMeshes );
    /// Copy constructor
    Scene(const Scene& pScene);
    /// Destructor
    ~Scene();

    /// Get the name of the scene
    QString GetName() const;
    /// Get the root scene node
    const SceneNode* GetRootNode() const;
    /// Get the bounding sphere
    const BoundingSphere* GetBoundingSphere() const;
    /// Get the number of polygons
    int GetNumberOfPolygons() const;
    /// Get the number of vertices
    int GetNumberOfVertices() const;
    /// Show the information of the scene
    void ShowInformation() const;

private:
    /// Name of the scene
    QString mName;
    /// Root scene node
    SceneNode* mSceneRoot;
    /// List of materials used
    QVector<Material*> mMaterials;
    /// List of geometries used
    QVector<Geometry*> mGeometries;
    /// List of meshes used
    QVector<Mesh*> mMeshes;
};

#endif
