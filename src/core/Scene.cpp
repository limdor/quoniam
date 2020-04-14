#include "Scene.h"

#include "Debug.h"

#include "glm/vec3.hpp"

Scene::Scene(const QString &pName, SceneNode *pSceneRoot, const QVector<std::shared_ptr<Material>>& pMaterials, const QVector<std::shared_ptr<Geometry>>& pGeometries, const QVector<std::shared_ptr<Mesh>>& pMeshes ):
    mName(pName), mRootNode(pSceneRoot), mMaterials(pMaterials), mGeometries(pGeometries), mMeshes(pMeshes)
{

}

Scene::~Scene()
{
    delete mRootNode;
}

QString Scene::GetName() const
{
    return mName;
}

const SceneNode* Scene::GetRootNode() const
{
    return mRootNode;
}

std::shared_ptr<BoundingSphere const> Scene::GetBoundingSphere() const
{
    return mRootNode->GetBoundingSphere();
}

int Scene::GetNumberOfPolygons() const
{
    return mRootNode->GetNumberOfPolygons();
}

int Scene::GetNumberOfVertices() const
{
    return mRootNode->GetNumberOfVertices();
}

void Scene::ShowInformation() const
{
    auto boundingSphere = GetBoundingSphere();
    glm::vec3 sceneCenter = boundingSphere->GetCenter();
    Debug::Log( QString("Scene: %1").arg(mName) );
    Debug::Log( QString("  Number of vertices: %1").arg(GetNumberOfVertices()) );
    Debug::Log( QString("  Number of polygons: %1").arg(GetNumberOfPolygons()) );
    Debug::Log( QString("  Center: (%1, %2, %3)").arg(sceneCenter.x).arg(sceneCenter.y).arg(sceneCenter.z) );
    Debug::Log( QString("  Radius: %1").arg(boundingSphere->GetRadius()) );
}
