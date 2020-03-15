/// \file SceneLoader.h
/// \class SceneLoader
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef SCENE_LOADER_H
#define SCENE_LOADER_H

#include "Geometry.h"
#include "Material.h"
#include "Mesh.h"
#include "Scene.h"

#include <QtCore/QString>
#include <QtCore/QVector>

#include "assimp/material.h"
#include "assimp/scene.h"

/// Class to load scenes from files
class SceneLoader
{
public:
    /// Create a scene from the given file
    static Scene * LoadScene(const QString &pPath);

private:
    /// Create an SceneNode given an Assimp node
    /// \param pNode Pointer to the Assimp node to be loaded
    /// \param pSceneMeshes List of the meshes used in the node
    static SceneNode* LoadSceneNode(const QVector<Mesh*>& pSceneMeshes, const aiNode* pNode);

    /// Load the materials in pScene
    static QVector<Material*> LoadMaterials(const aiScene* pAiScene, const QString& pScenePath);
    /// Convert a aiMaterial to a Material
    static Material* LoadMaterial(const aiMaterial* pAiMaterial, const QString& pScenePath);

    /// Load the geometries in pAiScene
    static QVector<Geometry*> LoadGeometries(const aiScene* pAiScene);
    /// Convert a aiMesh to a Geometry
    static Geometry* LoadGeometry(const aiMesh* pAiMesh);

    /// Load the meshes in pAiScene
    static QVector<Mesh*> LoadMeshes(const aiScene* pAiScene, const QVector<Material*>& pMaterials, const QVector<Geometry*>& pGeometries);
    /// Convert a aiMesh to a Mesh
    static Mesh* LoadMesh(const aiMesh* pAiMesh, int pAiMeshIndex, const QVector<Material*>& pMaterials, const QVector<Geometry*>& pGeometries);

    /// Show the information of the material
    static void ShowMaterialInformation(const aiMaterial *pMaterial);
    /// Show the information of the texture
    static void ShowTextureInformation(const aiMaterial* pMaterial, aiTextureType pType, unsigned int pTextureNumber);
};
#endif
