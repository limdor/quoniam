#include "SceneLoader.h"

#include "Debug.h"

#include <QtCore/QFileInfo>
#include <QtGui/QImageReader>

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

#include "glm/gtc/type_ptr.hpp"
#include "glm/matrix.hpp"

std::unique_ptr<Scene> SceneLoader::LoadScene(const QString &pPath)
{
    std::unique_ptr<Scene> sceneLoaded;

    Assimp::Importer importer;
    importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE,
                                aiPrimitiveType_POINT |			//remove points and
                                aiPrimitiveType_LINE );			//lines
    // Import the scene with the following parameters
    //   aiProcess_GenNormals
    //   aiProcess_CalcTangentSpace
    //   aiProcess_Triangulate
    //   aiProcess_JoinIdenticalVertices
    //   aiProcess_PreTransformVertices
    //   aiProcess_RemoveRedundantMaterials
    //   aiProcess_OptimizeMeshes
    const aiScene* scene = importer.ReadFile(pPath.toLatin1().data(),
                                             aiProcess_GenNormals               |
                                             aiProcess_CalcTangentSpace         |
                                             aiProcess_Triangulate              |
                                             aiProcess_JoinIdenticalVertices    |
                                             aiProcess_PreTransformVertices     |
                                             aiProcess_RemoveRedundantMaterials |
                                             aiProcess_OptimizeMeshes           |
                                             aiProcess_SortByPType              |
                                             aiProcess_ImproveCacheLocality );

    if(scene)
    {
        const QFileInfo fileInfo(pPath);

        Debug::Log("Load materials");
        const std::vector<std::shared_ptr<Material>> materials = LoadMaterials(scene, fileInfo.absolutePath());
        Debug::Log("Load geometries");
        const std::vector<std::shared_ptr<Geometry>> geometries = LoadGeometries(scene);
        Debug::Log("Load meshes");
        const std::vector<std::shared_ptr<Mesh>> meshes = LoadMeshes(scene, materials, geometries);

        Debug::Log("Load scene");
        auto rootNode = LoadSceneNode( meshes, scene->mRootNode );
        sceneLoaded = std::make_unique<Scene>( fileInfo.baseName().toStdString(), std::move(rootNode), materials, geometries, meshes );
    }
    else
    {
        Debug::Error(std::string{"Impossible to load with AssimpLoader: "} + importer.GetErrorString());
        sceneLoaded = std::make_unique<Scene>("Default", std::make_unique<SceneNode>("Default"), std::vector<std::shared_ptr<Material>>(), std::vector<std::shared_ptr<Geometry>>(), std::vector<std::shared_ptr<Mesh>>());
    }
    return sceneLoaded;
}

std::shared_ptr<SceneNode> SceneLoader::LoadSceneNode(const std::vector<std::shared_ptr<Mesh>>& pSceneMeshes, const aiNode* pNode)
{
    // Store the node's name.
    auto nodeLoaded = std::make_shared<SceneNode>( pNode->mName.data );

    // Load the transformation matrix.
    aiMatrix4x4 mat = pNode->mTransformation;
    // Transpose matrix to go from row-major to column major
    nodeLoaded->SetLocalTransform( glm::transpose( glm::make_mat4(&mat[0][0]) ) );

    // For each mesh
    for( int currentMesh = 0; currentMesh < (int)pNode->mNumMeshes; currentMesh++ )
    {
        // Load the mesh
        nodeLoaded->AddMesh( pSceneMeshes.at(pNode->mMeshes[currentMesh]) );
    }

    // For each child
    for( int currentNode = 0; currentNode < (int)pNode->mNumChildren; currentNode++ )
    {
        // Call the method recursively
        nodeLoaded->AddChild( LoadSceneNode(pSceneMeshes, pNode->mChildren[currentNode]) );
    }

    return nodeLoaded;
}

std::vector<std::shared_ptr<Material>> SceneLoader::LoadMaterials(const aiScene* pAiScene, const QString& pScenePath)
{
    int numberOfMaterials = (int)pAiScene->mNumMaterials;
    std::vector<std::shared_ptr<Material>> materials(numberOfMaterials);
    for( int i = 0; i < numberOfMaterials; i++ )
    {
        materials[i] = LoadMaterial(pAiScene->mMaterials[i], pScenePath);
    }
    return materials;
}

std::unique_ptr<Material> SceneLoader::LoadMaterial(const aiMaterial* pAiMaterial, const QString& pScenePath)
{
    std::unique_ptr<Material> material;

    ShowMaterialInformation(pAiMaterial);
    aiString name;
    if( AI_SUCCESS == pAiMaterial->Get(AI_MATKEY_NAME, name) )
    {
        material = std::make_unique<Material>( name.data );
    }
    else
    {
        material = std::make_unique<Material>( "No name" );
    }

    aiColor3D color;
    if( AI_SUCCESS == pAiMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color) )
    {
        material->SetKa( glm::vec3(color.r, color.g, color.b) );
    }
    if( AI_SUCCESS == pAiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color) )
    {
        material->SetKd( glm::vec3(color.r, color.g, color.b) );
    }
    if( AI_SUCCESS == pAiMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color) )
    {
        material->SetKs( glm::vec3(color.r, color.g, color.b) );
    }

    float value;
    if( AI_SUCCESS == pAiMaterial->Get(AI_MATKEY_SHININESS, value) )
    {
        material->SetShininess( value );
    }

    // Obtenim la textura difusa
    if( pAiMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0 )
    {
        aiString actualAiTexturePath;

        pAiMaterial->GetTexture( aiTextureType_DIFFUSE, 0, &actualAiTexturePath );
        const QString actualTexturePath( actualAiTexturePath.data );

        QString finalTexturePath = pScenePath;
        finalTexturePath.append("/").append(actualTexturePath);

        QImage imageTexture{};
        if( imageTexture.load(finalTexturePath) )
        {
            material->SetKdTexture(imageTexture);
        }
        else
        {
            QFile file(finalTexturePath);
            if(!file.exists())
            {
                Debug::Warning("Image " + finalTexturePath.toStdString() + " not found");
            }
            else
            {
                Debug::Warning("Impossible to load the image: " + finalTexturePath.toStdString());
                QFileInfo info(finalTexturePath);
                if(info.suffix() == "tga")
                {
                    Debug::Warning("RLE compression not supported in tga files");
                }
                QList<QByteArray> supportedFormats = QImageReader::supportedImageFormats();
                QString output;
                for(int k = 0; k < supportedFormats.size(); k++)
                {
                    output += supportedFormats.at(k);
                    output += ", ";
                }
                Debug::Log("Supported images: " + output.toStdString());
            }
        }
    }
    return material;
}

std::vector<std::shared_ptr<Geometry>> SceneLoader::LoadGeometries(const aiScene* pAiScene)
{
    unsigned int numberOfGeometries = pAiScene->mNumMeshes;
    std::vector<std::shared_ptr<Geometry>> geometries(numberOfGeometries);
    for( unsigned int i = 0; i < numberOfGeometries; i++ )
    {
        geometries[i] = LoadGeometry(pAiScene->mMeshes[i]);
    }
    return geometries;
}

std::unique_ptr<Geometry> SceneLoader::LoadGeometry(const aiMesh* pAiMesh)
{
    unsigned int numberOfVertices = pAiMesh->mNumVertices;

    auto geometry = std::make_unique<Geometry>(pAiMesh->mName.data, GeometryTopology::Triangles);

    if( pAiMesh->mPrimitiveTypes == aiPrimitiveType_TRIANGLE )
    {
        std::vector< glm::vec3 > vertexData;
        vertexData.reserve(numberOfVertices);
        for( unsigned int j = 0; j < numberOfVertices; ++j )
        {
            aiVector3D tmp = pAiMesh->mVertices[j];
            vertexData.emplace_back(tmp.x, tmp.y, tmp.z);
        }
        geometry->SetVerticesData(vertexData.size(), vertexData.data());

        if( pAiMesh->HasNormals() )
        {
            std::vector< glm::vec3 > normalData;
            normalData.reserve(numberOfVertices);
            for( unsigned int j = 0; j < numberOfVertices; ++j )
            {
                aiVector3D tmp = pAiMesh->mNormals[j];
                normalData.emplace_back(tmp.x, tmp.y, tmp.z);
            }
            geometry->SetNormalsData(normalData.size(), normalData.data());
        }

        if( pAiMesh->HasTangentsAndBitangents() )
        {
            std::vector< glm::vec3 > tangentData;
            std::vector< glm::vec3 > bitangentData;
            tangentData.reserve(numberOfVertices);
            bitangentData.reserve(numberOfVertices);
            for( unsigned int j = 0; j < numberOfVertices; ++j )
            {
                aiVector3D tmp = pAiMesh->mTangents[j];
                tangentData.emplace_back(tmp.x, tmp.y, tmp.z);

                tmp = pAiMesh->mBitangents[j];
                bitangentData.emplace_back(tmp.x, tmp.y, tmp.z);
            }
            geometry->SetTangentData(tangentData.size(), tangentData.data());
            geometry->SetBitangentData(bitangentData.size(), bitangentData.data());
        }

        if( pAiMesh->GetNumUVChannels() > 0 )
        {
            if( pAiMesh->HasTextureCoords(0) )
            {
                if( pAiMesh->mNumUVComponents[0] == 2 )
                {
                    // Guardem les coordenades de textura
                    std::vector< glm::vec2 > textCoordData;
                    textCoordData.reserve(numberOfVertices);
                    for( unsigned int j = 0; j < numberOfVertices; j++ )
                    {
                        textCoordData.emplace_back( pAiMesh->mTextureCoords[0][j].x, pAiMesh->mTextureCoords[0][j].y );
                    }
                    geometry->SetTextCoordsData( textCoordData.size(), textCoordData.data() );
                }
                else
                {
                    Debug::Warning("Only 2 UV components supported");
                }
            }
        }

        unsigned int numberOfFaces = pAiMesh->mNumFaces;
        std::vector< unsigned int > indexData;
        indexData.reserve(numberOfFaces * 3);
        for( unsigned int j = 0; j < numberOfFaces; ++j )
        {
            struct aiFace tmpFace = pAiMesh->mFaces[j];
            indexData.emplace_back(tmpFace.mIndices[0]);
            indexData.emplace_back(tmpFace.mIndices[1]);
            indexData.emplace_back(tmpFace.mIndices[2]);
        }
        geometry->SetIndexsData(indexData.size(), indexData.data());

        geometry->ComputeBoundingVolumes();
    }
    else
    {
        Debug::Error("SceneLoader::Unexpected primitive type");
    }
    return geometry;
}

std::vector<std::shared_ptr<Mesh>> SceneLoader::LoadMeshes(const aiScene* pAiScene, const std::vector<std::shared_ptr<Material>>& pMaterials, const std::vector<std::shared_ptr<Geometry>>& pGeometries)
{
    int numberOfMeshes = (int)pAiScene->mNumMeshes;
    std::vector<std::shared_ptr<Mesh>> meshes(numberOfMeshes);
    for( int i = 0; i < numberOfMeshes; i++ )
    {
        meshes[i] = LoadMesh(pAiScene->mMeshes[i], i, pMaterials, pGeometries);
    }
    return meshes;
}

std::unique_ptr<Mesh> SceneLoader::LoadMesh(const aiMesh* pAiMesh, int pAiMeshIndex, const std::vector<std::shared_ptr<Material>>& pMaterials, const std::vector<std::shared_ptr<Geometry>>& pGeometries)
{
    auto mesh = std::make_unique<Mesh>(pGeometries.at(pAiMeshIndex), pMaterials.at(pAiMesh->mMaterialIndex));
    mesh->SetName(pAiMesh->mName.data);

    return mesh;
}

void SceneLoader::ShowMaterialInformation(const aiMaterial* pMaterial)
{
    aiString name;
    if( AI_SUCCESS == pMaterial->Get(AI_MATKEY_NAME, name) )
    {
        Debug::Log( std::string{"Name: "} + name.data );
    }
    aiColor3D color;
    if( AI_SUCCESS == pMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color) )
    {
        Debug::Log( "   Ambient color: (" + std::to_string(color.r) + ", " + std::to_string(color.g) + ", " + std::to_string(color.b) + ")");
    }
    if( AI_SUCCESS == pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color) )
    {
        Debug::Log( "   Diffuse color: (" + std::to_string(color.r) + ", " + std::to_string(color.g) + ", " + std::to_string(color.b) + ")");
    }
    if( AI_SUCCESS == pMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, color) )
    {
        Debug::Log( "   Emissive color: (" + std::to_string(color.r) + ", " + std::to_string(color.g) + ", " + std::to_string(color.b) + ")");
    }
    if( AI_SUCCESS == pMaterial->Get(AI_MATKEY_COLOR_REFLECTIVE, color) )
    {
        Debug::Log( "   Reflective color: (" + std::to_string(color.r) + ", " + std::to_string(color.g) + ", " + std::to_string(color.b) + ")");
    }
    if( AI_SUCCESS == pMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color) )
    {
        Debug::Log( "   Specular color: (" + std::to_string(color.r) + ", " + std::to_string(color.g) + ", " + std::to_string(color.b) + ")");
    }
    float value;
    if( AI_SUCCESS == pMaterial->Get(AI_MATKEY_SHININESS, value) )
    {
        Debug::Log( "   Shininess: " + std::to_string(value) );
    }
    if( AI_SUCCESS == pMaterial->Get(AI_MATKEY_SHININESS_STRENGTH, value) )
    {
        Debug::Log( "   Shininess strength: " + std::to_string(value) );
    }
    if( AI_SUCCESS == pMaterial->Get(AI_MATKEY_COLOR_TRANSPARENT, color) )
    {
        Debug::Log( "   Transparent color: (" + std::to_string(color.r) + ", " + std::to_string(color.g) + ", " + std::to_string(color.b) + ")");
    }
    int intValue;
    if( AI_SUCCESS == pMaterial->Get(AI_MATKEY_ENABLE_WIREFRAME, intValue) )
    {
        if( intValue == 0 )
        {
            Debug::Log( "   Wireframe: Disabled" );
        }
        else if( intValue == 1 )
        {
            Debug::Log( "   Wireframe: Enabled" );
        }
        else
        {
            Debug::Warning( "   Wireframe: Unexpected value" );
        }
    }
    if( AI_SUCCESS == pMaterial->Get(AI_MATKEY_SHADING_MODEL, intValue) )
    {
        if( intValue == aiShadingMode_Flat )
        {
            Debug::Log( "   Shading model: Flat" );
        }
        else if( intValue == aiShadingMode_Gouraud )
        {
            Debug::Log( "   Shading model: Gouraud (simple)" );
        }
        else if( intValue == aiShadingMode_Phong )
        {
            Debug::Log( "   Shading model: Phong" );
        }
        else if( intValue == aiShadingMode_Blinn )
        {
            Debug::Log( "   Shading model: Blinn-Phong" );
        }
        else if( intValue == aiShadingMode_Toon )
        {
            Debug::Log( "   Shading model: Toon (comic)" );
        }
        else if( intValue == aiShadingMode_OrenNayar )
        {
            Debug::Log( "   Shading model: Oren-Nayar" );
        }
        else if( intValue == aiShadingMode_Minnaert )
        {
            Debug::Log( "   Shading model: Minnaert" );
        }
        else if( intValue == aiShadingMode_CookTorrance )
        {
            Debug::Log( "   Shading model: Cook-Torrance" );
        }
        else if( intValue == aiShadingMode_NoShading )
        {
            Debug::Log( "   Shading model: No shading" );
        }
        else if( intValue == aiShadingMode_Fresnel )
        {
            Debug::Log( "   Shading model: Fresnel" );
        }
        else
        {
            Debug::Warning( "   Shading model: Unexpected value" );
        }
    }
    unsigned int aux = pMaterial->GetTextureCount(aiTextureType_AMBIENT);
    if( aux > 0 )
    {
        Debug::Log( "   Number of ambient textures: " + std::to_string(aux));
        for( unsigned int i = 0; i < aux; i++ )
        {
            ShowTextureInformation(pMaterial, aiTextureType_AMBIENT, i);
        }
    }
    aux = pMaterial->GetTextureCount(aiTextureType_DIFFUSE);
    if( aux > 0 )
    {
        Debug::Log( "   Number of diffuse textures: " + std::to_string(aux));
        for( unsigned int i = 0; i < aux; i++ )
        {
            ShowTextureInformation(pMaterial, aiTextureType_DIFFUSE, i);
        }
    }
    aux = pMaterial->GetTextureCount(aiTextureType_DISPLACEMENT);
    if( aux > 0 )
    {
        Debug::Log( "   Number of displacement textures: " + std::to_string(aux));
        for( unsigned int i = 0; i < aux; i++ )
        {
            ShowTextureInformation(pMaterial, aiTextureType_DISPLACEMENT, i);
        }
    }
    aux = pMaterial->GetTextureCount(aiTextureType_EMISSIVE);
    if( aux > 0 )
    {
        Debug::Log( "   Number of emissive textures: " + std::to_string(aux));
        for( unsigned int i = 0; i < aux; i++ )
        {
            ShowTextureInformation(pMaterial, aiTextureType_EMISSIVE, i);
        }
    }
    aux = pMaterial->GetTextureCount(aiTextureType_HEIGHT);
    if( aux > 0 )
    {
        Debug::Log( "   Number of height textures: " + std::to_string(aux));
        for( unsigned int i = 0; i < aux; i++ )
        {
            ShowTextureInformation(pMaterial, aiTextureType_HEIGHT, i);
        }
    }
    aux = pMaterial->GetTextureCount(aiTextureType_LIGHTMAP);
    if( aux > 0 )
    {
        Debug::Log( "   Number of lightmap textures: " + std::to_string(aux));
        for( unsigned int i = 0; i < aux; i++ )
        {
            ShowTextureInformation(pMaterial, aiTextureType_LIGHTMAP, i);
        }
    }
    aux = pMaterial->GetTextureCount(aiTextureType_NORMALS);
    if( aux > 0 )
    {
        Debug::Log( "   Number of normals textures: " + std::to_string(aux));
        for( unsigned int i = 0; i < aux; i++ )
        {
            ShowTextureInformation(pMaterial, aiTextureType_NORMALS, i);
        }
    }
    aux = pMaterial->GetTextureCount(aiTextureType_OPACITY);
    if( aux > 0 )
    {
        Debug::Log( "   Number of opacity textures: " + std::to_string(aux));
        for( unsigned int i = 0; i < aux; i++ )
        {
            ShowTextureInformation(pMaterial, aiTextureType_OPACITY, i);
        }
    }
    aux = pMaterial->GetTextureCount(aiTextureType_REFLECTION);
    if( aux > 0 )
    {
        Debug::Log( "   Number of reflection textures: " + std::to_string(aux));
        for( unsigned int i = 0; i < aux; i++ )
        {
            ShowTextureInformation(pMaterial, aiTextureType_REFLECTION, i);
        }
    }
    aux = pMaterial->GetTextureCount(aiTextureType_SHININESS);
    if( aux > 0 )
    {
        Debug::Log( "   Number of shininess textures: " + std::to_string(aux));
        for( unsigned int i = 0; i < aux; i++ )
        {
            ShowTextureInformation(pMaterial, aiTextureType_SHININESS, i);
        }
    }
    aux = pMaterial->GetTextureCount(aiTextureType_SPECULAR);
    if( aux > 0 )
    {
        Debug::Log( "   Number of specular textures: " + std::to_string(aux));
        for( unsigned int i = 0; i < aux; i++ )
        {
            ShowTextureInformation(pMaterial, aiTextureType_SPECULAR, i);
        }
    }
    aux = pMaterial->GetTextureCount(aiTextureType_UNKNOWN);
    if( aux > 0 )
    {
        Debug::Log( "   Number of unknown textures: " + std::to_string(aux));
        for( unsigned int i = 0; i < aux; i++ )
        {
            ShowTextureInformation(pMaterial, aiTextureType_UNKNOWN, i);
        }
    }
}

void SceneLoader::ShowTextureInformation(const aiMaterial* pMaterial, aiTextureType pType, unsigned int pTextureNumber)
{
    aiString path;
    aiTextureMapping mapping;
    unsigned int uvindex;
    float blend;
    aiTextureOp op;
    aiTextureMapMode mapmode;
    Debug::Log( "      Information of texture " + std::to_string(pTextureNumber) );
    if( AI_SUCCESS == pMaterial->GetTexture( pType, pTextureNumber, &path, &mapping, &uvindex, &blend, &op, &mapmode ) )
    {
        Debug::Log( "         Path: " + std::string{path.data} );
        Debug::Log( "         UV index: " + std::to_string(uvindex) );
        Debug::Log( "         Blend: " + std::to_string(blend) );
    }
    else
    {
        Debug::Log( "         Impossible to get the texture" );
    }
}
