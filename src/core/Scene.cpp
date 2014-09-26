//Definition include
#include "Scene.h"

//System includes
#include <algorithm>

//Qt includes
#include <QApplication>
#include <QProgressDialog>
#include <QTime>

//Project includes
#include "Debug.h"
#include "MainWindow.h"
#include "Tools.h"

Scene::Scene(const QString &pName, SceneNode *pSceneRoot, const QVector<Material*>& pMaterials, const QVector<Geometry*>& pGeometries, const QVector<Mesh*>& pMeshes ):
    mName(pName), mSceneRoot(pSceneRoot), mMaterials(pMaterials), mGeometries(pGeometries), mMeshes(pMeshes)
{

}

Scene::Scene(const Scene& pScene):
    mName(pScene.mName)
{
    mSceneRoot = new SceneNode(*pScene.mSceneRoot);
}

Scene::~Scene()
{
    delete mSceneRoot;
    for(int i = 0; i < mMeshes.size(); i++)
    {
        delete mMeshes.at(i);
    }
    for( int i = 0; i < mGeometries.size(); i++ )
    {
        delete mGeometries.at(i);
    }
    for( int i = 0; i < mMaterials.size(); i++ )
    {
        delete mMaterials.at(i);
    }
}

QString Scene::GetName() const
{
    return mName;
}

const SceneNode* Scene::GetRootNode() const
{
    return mSceneRoot;
}

const BoundingSphere* Scene::GetBoundingSphere() const
{
    return mSceneRoot->GetBoundingSphere();
}

int Scene::GetNumberOfPolygons() const
{
    return mSceneRoot->GetNumberOfPolygons();
}

int Scene::GetNumberOfVertices() const
{
    return mSceneRoot->GetNumberOfVertices();
}

void Scene::ShowInformation() const
{
    const BoundingSphere* boundingSphere = GetBoundingSphere();
    glm::vec3 sceneCenter = boundingSphere->GetCenter();
    Debug::Log( QString("Scene: %1").arg(mName) );
    Debug::Log( QString("  Number of vertices: %1").arg(GetNumberOfVertices()) );
    Debug::Log( QString("  Number of polygons: %1").arg(GetNumberOfPolygons()) );
    Debug::Log( QString("  Center: (%1, %2, %3)").arg(sceneCenter.x).arg(sceneCenter.y).arg(sceneCenter.z) );
    Debug::Log( QString("  Radius: %1").arg(boundingSphere->GetRadius()) );
}
