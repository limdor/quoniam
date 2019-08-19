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

class Scene
{
public:
    /// Create an scene given the name and the root scene node
    Scene(const QString &pName, SceneNode *pSceneRoot, const QVector<Material*>& pMaterials, const QVector<Geometry*>& pGeometries, const QVector<Mesh*>& pMeshes );
    Scene(const Scene& pScene);
    ~Scene();

    QString GetName() const;
    const SceneNode* GetRootNode() const;
    const BoundingSphere* GetBoundingSphere() const;
    int GetNumberOfPolygons() const;
    int GetNumberOfVertices() const;
    void ShowInformation() const;

private:
    QString mName;
    SceneNode* mRootNode;
    /// List of materials used
    QVector<Material*> mMaterials;
    /// List of geometries used
    QVector<Geometry*> mGeometries;
    /// List of meshes used
    QVector<Mesh*> mMeshes;
};

#endif
