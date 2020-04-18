#include "Scene.h"

#include "Debug.h"

#include "glm/vec3.hpp"

Scene::Scene(const QString &pName, std::shared_ptr<SceneNode> pSceneRoot, const std::vector<std::shared_ptr<Material>>& pMaterials, const std::vector<std::shared_ptr<Geometry>>& pGeometries, const std::vector<std::shared_ptr<Mesh>>& pMeshes ):
    mName(pName), mRootNode(std::move(pSceneRoot)), mMaterials(pMaterials), mGeometries(pGeometries), mMeshes(pMeshes)
{

}

QString Scene::GetName() const
{
    return mName;
}

std::shared_ptr<SceneNode const> Scene::GetRootNode() const
{
    return mRootNode;
}

std::shared_ptr<BoundingSphere const> Scene::GetBoundingSphere() const
{
    return mRootNode->GetBoundingSphere();
}

size_t Scene::GetNumberOfPolygons() const
{
    return mRootNode->GetNumberOfPolygons();
}

size_t Scene::GetNumberOfVertices() const
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
