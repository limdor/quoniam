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

        const QVector<std::shared_ptr<Material>> materials = LoadMaterials(scene, fileInfo.absolutePath());
        const QVector<std::shared_ptr<Geometry>> geometries = LoadGeometries(scene);
        const QVector<std::shared_ptr<Mesh>> meshes = LoadMeshes(scene, materials, geometries);

        SceneNode* rootNode = LoadSceneNode( meshes, scene->mRootNode );
        sceneLoaded = std::make_unique<Scene>( fileInfo.baseName(), rootNode, materials, geometries, meshes );
    }
    else
    {
        Debug::Error(QString("Impossible to load with AssimpLoader: %1").arg(importer.GetErrorString()));
        sceneLoaded = std::make_unique<Scene>("Default", new SceneNode("Default"), QVector<std::shared_ptr<Material>>(), QVector<std::shared_ptr<Geometry>>(), QVector<std::shared_ptr<Mesh>>());
    }
    return sceneLoaded;
}

SceneNode* SceneLoader::LoadSceneNode(const QVector<std::shared_ptr<Mesh>>& pSceneMeshes, const aiNode* pNode)
{
    // Store the node's name.
    SceneNode* nodeLoaded = new SceneNode( QString(pNode->mName.data) );

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

QVector<std::shared_ptr<Material>> SceneLoader::LoadMaterials(const aiScene* pAiScene, const QString& pScenePath)
{
    int numberOfMaterials = (int)pAiScene->mNumMaterials;
    QVector<std::shared_ptr<Material>> materials(numberOfMaterials);
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
        material = std::make_unique<Material>( QString("%1").arg(name.data) );
    }
    else
    {
        material = std::make_unique<Material>( QString("No name") );
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

        QImage* imageTexture = new QImage();
        if( imageTexture->load(finalTexturePath) )
        {
            material->SetKdTexture(imageTexture);
        }
        else
        {
            QFile file(finalTexturePath);
            if(!file.exists())
            {
                Debug::Warning(QString("Image %1 not found").arg(finalTexturePath));
            }
            else
            {
                Debug::Warning(QString("Impossible to load the image: %1").arg(finalTexturePath));
                QFileInfo info(finalTexturePath);
                if(info.suffix() == "tga")
                {
                    Debug::Warning(QString("RLE compression not supported in tga files"));
                }
                QList<QByteArray> supportedFormats = QImageReader::supportedImageFormats();
                QString output;
                for(int k = 0; k < supportedFormats.size(); k++)
                {
                    output += supportedFormats.at(k);
                    output += ", ";
                }
                Debug::Log(QString("Supported images: %1").arg(output));
            }
        }
        delete imageTexture;
    }
    return material;
}

QVector<std::shared_ptr<Geometry>> SceneLoader::LoadGeometries(const aiScene* pAiScene)
{
    unsigned int numberOfGeometries = pAiScene->mNumMeshes;
    QVector<std::shared_ptr<Geometry>> geometries(numberOfGeometries);
    for( int i = 0; i < numberOfGeometries; i++ )
    {
        geometries[i] = LoadGeometry(pAiScene->mMeshes[i]);
    }
    return geometries;
}

std::unique_ptr<Geometry> SceneLoader::LoadGeometry(const aiMesh* pAiMesh)
{
    unsigned int numberOfVertices = pAiMesh->mNumVertices;

    auto geometry = std::make_unique<Geometry>(QString(pAiMesh->mName.data), GeometryTopology::Triangles);

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
                    Debug::Warning(QString("Only 2 UV components supported"));
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

QVector<std::shared_ptr<Mesh>> SceneLoader::LoadMeshes(const aiScene* pAiScene, const QVector<std::shared_ptr<Material>>& pMaterials, const QVector<std::shared_ptr<Geometry>>& pGeometries)
{
    int numberOfMeshes = (int)pAiScene->mNumMeshes;
    QVector<std::shared_ptr<Mesh>> meshes(numberOfMeshes);
    for( int i = 0; i < numberOfMeshes; i++ )
    {
        meshes[i] = LoadMesh(pAiScene->mMeshes[i], i, pMaterials, pGeometries);
    }
    return meshes;
}

std::unique_ptr<Mesh> SceneLoader::LoadMesh(const aiMesh* pAiMesh, int pAiMeshIndex, const QVector<std::shared_ptr<Material>>& pMaterials, const QVector<std::shared_ptr<Geometry>>& pGeometries)
{
    auto mesh = std::make_unique<Mesh>(pGeometries.at(pAiMeshIndex), pMaterials.at(pAiMesh->mMaterialIndex));
    mesh->SetName(QString(pAiMesh->mName.data));

    return mesh;
}

void SceneLoader::ShowMaterialInformation(const aiMaterial* pMaterial)
{
    aiString name;
    if( AI_SUCCESS == pMaterial->Get(AI_MATKEY_NAME, name) )
    {
        Debug::Log( QString("Name: %1").arg(name.data) );
    }
    aiColor3D color;
    if( AI_SUCCESS == pMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color) )
    {
        Debug::Log( QString("   Ambient color: (%1, %2, %3)").arg(color.r).arg(color.g).arg(color.b) );
    }
    if( AI_SUCCESS == pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color) )
    {
        Debug::Log( QString("   Diffuse color: (%1, %2, %3)").arg(color.r).arg(color.g).arg(color.b) );
    }
    if( AI_SUCCESS == pMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, color) )
    {
        Debug::Log( QString("   Emissive color: (%1, %2, %3)").arg(color.r).arg(color.g).arg(color.b) );
    }
    if( AI_SUCCESS == pMaterial->Get(AI_MATKEY_COLOR_REFLECTIVE, color) )
    {
        Debug::Log( QString("   Reflective color: (%1, %2, %3)").arg(color.r).arg(color.g).arg(color.b) );
    }
    if( AI_SUCCESS == pMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color) )
    {
        Debug::Log( QString("   Specular color: (%1, %2, %3)").arg(color.r).arg(color.g).arg(color.b) );
    }
    float value;
    if( AI_SUCCESS == pMaterial->Get(AI_MATKEY_SHININESS, value) )
    {
        Debug::Log( QString("   Shininess: %1").arg(value) );
    }
    if( AI_SUCCESS == pMaterial->Get(AI_MATKEY_SHININESS_STRENGTH, value) )
    {
        Debug::Log( QString("   Shininess strength: %1").arg(value) );
    }
    if( AI_SUCCESS == pMaterial->Get(AI_MATKEY_COLOR_TRANSPARENT, color) )
    {
        Debug::Log( QString("   Transparent color: (%1, %2, %3)").arg(color.r).arg(color.g).arg(color.b) );
    }
    int intValue;
    if( AI_SUCCESS == pMaterial->Get(AI_MATKEY_ENABLE_WIREFRAME, intValue) )
    {
        if( intValue == 0 )
        {
            Debug::Log( QString("   Wireframe: Disabled") );
        }
        else if( intValue == 1 )
        {
            Debug::Log( QString("   Wireframe: Enabled") );
        }
        else
        {
            Debug::Warning( QString("   Wireframe: Unexpected value") );
        }
    }
    if( AI_SUCCESS == pMaterial->Get(AI_MATKEY_SHADING_MODEL, intValue) )
    {
        if( intValue == aiShadingMode_Flat )
        {
            Debug::Log( QString("   Shading model: Flat") );
        }
        else if( intValue == aiShadingMode_Gouraud )
        {
            Debug::Log( QString("   Shading model: Gouraud (simple)") );
        }
        else if( intValue == aiShadingMode_Phong )
        {
            Debug::Log( QString("   Shading model: Phong") );
        }
        else if( intValue == aiShadingMode_Blinn )
        {
            Debug::Log( QString("   Shading model: Blinn-Phong") );
        }
        else if( intValue == aiShadingMode_Toon )
        {
            Debug::Log( QString("   Shading model: Toon (comic)") );
        }
        else if( intValue == aiShadingMode_OrenNayar )
        {
            Debug::Log( QString("   Shading model: Oren-Nayar") );
        }
        else if( intValue == aiShadingMode_Minnaert )
        {
            Debug::Log( QString("   Shading model: Minnaert") );
        }
        else if( intValue == aiShadingMode_CookTorrance )
        {
            Debug::Log( QString("   Shading model: Cook-Torrance") );
        }
        else if( intValue == aiShadingMode_NoShading )
        {
            Debug::Log( QString("   Shading model: No shading") );
        }
        else if( intValue == aiShadingMode_Fresnel )
        {
            Debug::Log( QString("   Shading model: Fresnel") );
        }
        else
        {
            Debug::Warning( QString("   Shading model: Unexpected value") );
        }
    }
    unsigned int aux = pMaterial->GetTextureCount(aiTextureType_AMBIENT);
    if( aux > 0 )
    {
        Debug::Log( QString("   Number of ambient textures: %1").arg( aux ) );
        for( unsigned int i = 0; i < aux; i++ )
        {
            ShowTextureInformation(pMaterial, aiTextureType_AMBIENT, i);
        }
    }
    aux = pMaterial->GetTextureCount(aiTextureType_DIFFUSE);
    if( aux > 0 )
    {
        Debug::Log( QString("   Number of diffuse textures: %1").arg( aux ) );
        for( unsigned int i = 0; i < aux; i++ )
        {
            ShowTextureInformation(pMaterial, aiTextureType_DIFFUSE, i);
        }
    }
    aux = pMaterial->GetTextureCount(aiTextureType_DISPLACEMENT);
    if( aux > 0 )
    {
        Debug::Log( QString("   Number of displacement textures: %1").arg( aux ) );
        for( unsigned int i = 0; i < aux; i++ )
        {
            ShowTextureInformation(pMaterial, aiTextureType_DISPLACEMENT, i);
        }
    }
    aux = pMaterial->GetTextureCount(aiTextureType_EMISSIVE);
    if( aux > 0 )
    {
        Debug::Log( QString("   Number of emissive textures: %1").arg( aux ) );
        for( unsigned int i = 0; i < aux; i++ )
        {
            ShowTextureInformation(pMaterial, aiTextureType_EMISSIVE, i);
        }
    }
    aux = pMaterial->GetTextureCount(aiTextureType_HEIGHT);
    if( aux > 0 )
    {
        Debug::Log( QString("   Number of height textures: %1").arg( aux ) );
        for( unsigned int i = 0; i < aux; i++ )
        {
            ShowTextureInformation(pMaterial, aiTextureType_HEIGHT, i);
        }
    }
    aux = pMaterial->GetTextureCount(aiTextureType_LIGHTMAP);
    if( aux > 0 )
    {
        Debug::Log( QString("   Number of lightmap textures: %1").arg( aux ) );
        for( unsigned int i = 0; i < aux; i++ )
        {
            ShowTextureInformation(pMaterial, aiTextureType_LIGHTMAP, i);
        }
    }
    aux = pMaterial->GetTextureCount(aiTextureType_NORMALS);
    if( aux > 0 )
    {
        Debug::Log( QString("   Number of normals textures: %1").arg( aux ) );
        for( unsigned int i = 0; i < aux; i++ )
        {
            ShowTextureInformation(pMaterial, aiTextureType_NORMALS, i);
        }
    }
    aux = pMaterial->GetTextureCount(aiTextureType_OPACITY);
    if( aux > 0 )
    {
        Debug::Log( QString("   Number of opacity textures: %1").arg( aux ) );
        for( unsigned int i = 0; i < aux; i++ )
        {
            ShowTextureInformation(pMaterial, aiTextureType_OPACITY, i);
        }
    }
    aux = pMaterial->GetTextureCount(aiTextureType_REFLECTION);
    if( aux > 0 )
    {
        Debug::Log( QString("   Number of reflection textures: %1").arg( aux ) );
        for( unsigned int i = 0; i < aux; i++ )
        {
            ShowTextureInformation(pMaterial, aiTextureType_REFLECTION, i);
        }
    }
    aux = pMaterial->GetTextureCount(aiTextureType_SHININESS);
    if( aux > 0 )
    {
        Debug::Log( QString("   Number of shininess textures: %1").arg( aux ) );
        for( unsigned int i = 0; i < aux; i++ )
        {
            ShowTextureInformation(pMaterial, aiTextureType_SHININESS, i);
        }
    }
    aux = pMaterial->GetTextureCount(aiTextureType_SPECULAR);
    if( aux > 0 )
    {
        Debug::Log( QString("   Number of specular textures: %1").arg( aux ) );
        for( unsigned int i = 0; i < aux; i++ )
        {
            ShowTextureInformation(pMaterial, aiTextureType_SPECULAR, i);
        }
    }
    aux = pMaterial->GetTextureCount(aiTextureType_UNKNOWN);
    if( aux > 0 )
    {
        Debug::Log( QString("   Number of unknown textures: %1").arg( aux ) );
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
    Debug::Log( QString("      Information of texture %1").arg(pTextureNumber) );
    if( AI_SUCCESS == pMaterial->GetTexture( pType, pTextureNumber, &path, &mapping, &uvindex, &blend, &op, &mapmode ) )
    {
        Debug::Log( QString("         Path: %1").arg(path.data) );
        Debug::Log( QString("         UV index: %1").arg(uvindex) );
        Debug::Log( QString("         Blend: %1").arg(blend) );
    }
    else
    {
        Debug::Log( QString("         Impossible to get the texture") );
    }
}
