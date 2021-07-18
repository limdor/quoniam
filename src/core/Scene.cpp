#include "Scene.h"

#include "Debug.h"

#include "glm/vec3.hpp"

Scene::Scene(const std::string &pName, std::shared_ptr<SceneNode> pSceneRoot, const std::vector<std::shared_ptr<Material>>& pMaterials, const std::vector<std::shared_ptr<Geometry>>& pGeometries, const std::vector<std::shared_ptr<Mesh>>& pMeshes ):
    mName(pName), mRootNode(std::move(pSceneRoot)), mMaterials(pMaterials), mGeometries(pGeometries), mMeshes(pMeshes)
{

}

std::string Scene::GetName() const
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
    Debug::Log( "Scene: " + mName );
    Debug::Log( "  Number of vertices: " + std::to_string(GetNumberOfVertices()) );
    Debug::Log( "  Number of polygons: " + std::to_string(GetNumberOfPolygons()) );
    Debug::Log( "  Center: (" + std::to_string(sceneCenter.x) + ", " + std::to_string(sceneCenter.y) + ", " + std::to_string(sceneCenter.z) + ")");
    Debug::Log( "  Radius: " + std::to_string(boundingSphere->GetRadius()) );
}
