#ifndef SCENE_LOADER_H
#define SCENE_LOADER_H

#include "Geometry.h"
#include "Material.h"
#include "Mesh.h"
#include "Scene.h"

#include "assimp/material.h"
#include "assimp/scene.h"

#include <filesystem>
#include <memory>
#include <vector>

/// Class to load scenes from files
class SceneLoader
{
public:
    /// Create a scene from the given file
    static std::unique_ptr<Scene> LoadScene(const std::filesystem::path &pPath);

private:
    /// Create an SceneNode given an Assimp node
    /// \param pNode Pointer to the Assimp node to be loaded
    /// \param pSceneMeshes List of the meshes used in the node
    static std::shared_ptr<SceneNode> LoadSceneNode(const std::vector<std::shared_ptr<Mesh>>& pSceneMeshes, const aiNode* pNode);

    /// Load the materials in pScene
    static std::vector<std::shared_ptr<Material>> LoadMaterials(const aiScene* pAiScene, const std::filesystem::path& pScenePath);
    /// Convert a aiMaterial to a Material
    static std::unique_ptr<Material> LoadMaterial(const aiMaterial* pAiMaterial, const std::filesystem::path& pScenePath);

    /// Load the geometries in pAiScene
    static std::vector<std::shared_ptr<Geometry>> LoadGeometries(const aiScene* pAiScene);
    /// Convert a aiMesh to a Geometry
    static std::unique_ptr<Geometry> LoadGeometry(const aiMesh* pAiMesh);

    /// Load the meshes in pAiScene
    static std::vector<std::shared_ptr<Mesh>> LoadMeshes(const aiScene* pAiScene, const std::vector<std::shared_ptr<Material>>& pMaterials, const std::vector<std::shared_ptr<Geometry>>& pGeometries);
    /// Convert a aiMesh to a Mesh
    static std::unique_ptr<Mesh> LoadMesh(const aiMesh* pAiMesh, int pAiMeshIndex, const std::vector<std::shared_ptr<Material>>& pMaterials, const std::vector<std::shared_ptr<Geometry>>& pGeometries);

    /// Show the information of the material
    static void ShowMaterialInformation(const aiMaterial *pMaterial);
    /// Show the information of the texture
    static void ShowTextureInformation(const aiMaterial* pMaterial, aiTextureType pType, unsigned int pTextureNumber);
};
#endif
