#include "SerializedSceneGeometry.h"

#include "Debug.h"
#include "Tools.h"

#include "glm/exponential.hpp"
#include "glm/geometric.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/gtx/norm.hpp"
#include "glm/gtx/vector_angle.hpp"

#include "Miniball.h"

SerializedSceneGeometry::SerializedSceneGeometry(std::shared_ptr<Scene const> pScene):
    mNumberOfVertexs(0), mNumberOfFaces(0)
{
    int numberOfVertices = static_cast<int>(pScene->GetNumberOfVertices());
    mVertexs.resize(numberOfVertices);
    mVertexNeighbors.resize(numberOfVertices);
    mVertexCurvatures.resize(numberOfVertices);
    int numberOfPolygons = static_cast<int>(pScene->GetNumberOfPolygons());
    mFaces.resize(numberOfPolygons);
    mFaceNeighbors.resize(numberOfPolygons);
    mFaceAreas.resize(numberOfPolygons);
    SerializeSceneNodes(pScene->GetRootNode());
    Q_ASSERT(mVertexs.size() == mNumberOfVertexs && mFaces.size() == mNumberOfFaces);
    ComputeBoundingSphere();
    ComputeNeighbourhood();
    ComputeFaceAreas();
    ComputeVertexCurvatures();
}

void SerializedSceneGeometry::ComputeBoundingSphere()
{
    Miniball<3> mb;
    Point<3> p;
    for( int i = 0; i < mNumberOfVertexs; i++ )
    {
        p[0] = mVertexs.at(i).x;
        p[1] = mVertexs.at(i).y;
        p[2] = mVertexs.at(i).z;
        mb.check_in(p);
    }
    mb.build();
    mBoundingSphere = std::make_unique<BoundingSphere>();
    glm::vec3 sphereCenter;
    sphereCenter.x = mb.center()[0];
    sphereCenter.y = mb.center()[1];
    sphereCenter.z = mb.center()[2];
    if(!mb.is_valid(1e-6))
    {
        double slack;
        double error = mb.accuracy(slack);
        Debug::Warning("Bounding sphere in SerializedSceneGeometry may be invalid. Accuracy: " + std::to_string(error));
    }

    mBoundingSphere->SetCenter(sphereCenter);
    mBoundingSphere->SetRadius(glm::sqrt(mb.squared_radius()));
}

void SerializedSceneGeometry::ComputeNeighbourhood()
{
    auto const start = std::chrono::steady_clock::now();

    float epsilon = mBoundingSphere->GetRadius() / 1000000.0f;
    Debug::Log( "SerializedSceneGeometry::ComputeNeighbourhood: Epsilon value: " + std::to_string(epsilon) );

    std::vector< std::pair< size_t, glm::vec3 > > points(mNumberOfFaces * 3);
    // All vertices of the scene are added to a list of points with information about what mesh they belong,
    // the vertex index inside the mesh and the vertex position
    for( size_t j = 0; j < mNumberOfFaces; j++ )
    {
        points[j*3 + 0] = std::pair< size_t, glm::vec3 >(j*3 + 0, mVertexs.at(mFaces.at(j)[0]));
        points[j*3 + 1] = std::pair< size_t, glm::vec3 >(j*3 + 1, mVertexs.at(mFaces.at(j)[1]));
        points[j*3 + 2] = std::pair< size_t, glm::vec3 >(j*3 + 2, mVertexs.at(mFaces.at(j)[2]));
    }

    std::vector< std::pair< size_t, glm::vec3 > > pointsOrderedByX = points;
    std::vector< std::pair< size_t, glm::vec3 > > pointsOrderedByY = points;
    std::vector< std::pair< size_t, glm::vec3 > > pointsOrderedByZ = points;

    points.clear();

    //S'ordenen els punts per la coordenada X, la coordenada Y i la coordenada Z
    std::vector< size_t > pointsX = Tools::GetOrderedIndexesByDimension(pointsOrderedByX, 0);
    pointsX = Tools::GetOrderedIndexes(pointsX);
    std::vector< size_t > pointsY = Tools::GetOrderedIndexesByDimension(pointsOrderedByY, 1);
    pointsY = Tools::GetOrderedIndexes(pointsY);
    std::vector< size_t > pointsZ = Tools::GetOrderedIndexesByDimension(pointsOrderedByZ, 2);
    pointsZ = Tools::GetOrderedIndexes(pointsZ);

    int verticesWithoutNeighbours = 0;

    for( size_t j = 0; j < mNumberOfFaces * 3; j++ )
    {
        std::vector< size_t > neighboursX = Tools::FindNearestThanEpsilonByDimension( pointsX.at(j), pointsOrderedByX, epsilon, 0 );
        std::vector< size_t > neighboursY = Tools::FindNearestThanEpsilonByDimension( pointsY.at(j), pointsOrderedByY, epsilon, 1 );
        std::vector< size_t > neighboursZ = Tools::FindNearestThanEpsilonByDimension( pointsZ.at(j), pointsOrderedByZ, epsilon, 2 );
        std::vector< size_t > neighbours = Tools::MergeNeighbours(neighboursX, neighboursY, neighboursZ);

        auto& vertexNeighbors = mVertexNeighbors[ mFaces.at(j / 3)[j % 3] ];
        vertexNeighbors.insert(vertexNeighbors.end(), neighbours.cbegin(), neighbours.cend());
    }
    for( int j = 0; j < mVertexNeighbors.size(); j++ )
    {
        std::sort(mVertexNeighbors[j].begin(), mVertexNeighbors[j].end());
        std::vector< size_t >::iterator it = std::unique(mVertexNeighbors[j].begin(), mVertexNeighbors[j].end());
        mVertexNeighbors[j].resize( it - mVertexNeighbors[j].begin() );
        if( mVertexNeighbors.at(j).size() == 0 )
        {
            verticesWithoutNeighbours++;
        }
    }

    if( verticesWithoutNeighbours > 1 )
    {
        Debug::Warning( std::to_string(verticesWithoutNeighbours) + " vertices without neighbours");
    }
    else if( verticesWithoutNeighbours == 1 )
    {
        Debug::Warning("1 vertex without neighbours");
    }

    //Using the neighbourhood of the vertex we compute the neighbourhood of the polygons
    for( size_t j = 0; j < mNumberOfFaces; j++ )
    {
        std::vector< size_t > polygonNeighbours;
        for( int k = 0; k < 3; k++ )
        {
            const auto& vertexNeighbors = mVertexNeighbors.at( mFaces.at(j)[k] );
            polygonNeighbours.insert(polygonNeighbours.end(), vertexNeighbors.cbegin(), vertexNeighbors.cend());
        }

        std::sort(polygonNeighbours.begin(), polygonNeighbours.end());
        std::vector< size_t >::iterator it = std::unique(polygonNeighbours.begin(), polygonNeighbours.end());
        polygonNeighbours.resize( it - polygonNeighbours.begin() );

        mFaceNeighbors[j] = polygonNeighbours;
    }
    auto const end = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> const timeElapsed = end - start;
    Debug::Log("Scene::ComputeNeighbourhood - Total time elapsed: " + std::to_string(timeElapsed.count()) + " ms");
}

void SerializedSceneGeometry::ComputeVertexCurvatures()
{
    for( int i = 0; i < mNumberOfVertexs; i++ )
    {
        float sumAngles = 0.0f;
        for( int j = 0; j < mVertexNeighbors.at(i).size(); j++ )
        {
            glm::ivec3 face = mFaces.at(mVertexNeighbors.at(i).at(j));
            float minDistance = FLT_MAX;
            int index = 0;
            for( int k = 0; k < 3; k++ )
            {
                float distance = glm::distance2(mVertexs.at(i), mVertexs.at(face[k]));
                if(distance < minDistance)
                {
                    index = k;
                    minDistance = distance;
                }
            }
            glm::vec3 v1 = mVertexs.at(face[(index + 1) % 3]) - mVertexs.at(face[index]);
            glm::vec3 v2 = mVertexs.at(face[(index + 2) % 3]) - mVertexs.at(face[index]);
            v1 = glm::normalize(v1);
            v2 = glm::normalize(v2);
            sumAngles += glm::angle(v1, v2);
        }
        mVertexCurvatures[i] = 2.0f * glm::pi<float>() - sumAngles;
    }
}

void SerializedSceneGeometry::ComputeFaceAreas()
{
    for( size_t i = 0; i < mNumberOfFaces; i++ )
    {
        mFaceAreas[ i ] = 0.5f * glm::length( glm::cross( mVertexs.at( mFaces.at(i)[1] ) - mVertexs.at( mFaces.at(i)[0] ), mVertexs.at( mFaces.at(i)[2] ) - mVertexs.at( mFaces.at(i)[0] ) ) );
    }
}

void SerializedSceneGeometry::ShowNeighbours() const
{
    Debug::Log( "-- Vertex neighbours --" );
    for( int i = 0; i < mVertexNeighbors.size(); i++ )
    {
        Debug::Log( "Neighbours of " + std::to_string(i) + ":");
        for( int j = 0; j < mVertexNeighbors.at(i).size(); j++ )
        {
            Debug::Log( "   " + std::to_string(mVertexNeighbors.at(i).at(j)) );
        }
    }
    Debug::Log( "-- Polygon neighbours --" );
    for( int i = 0; i < mFaceNeighbors.size(); i++ )
    {
        Debug::Log( "Neighbours of " + std::to_string(i) + ":");
        for( int j = 0; j < mFaceNeighbors.at(i).size(); j++ )
        {
            Debug::Log( "   " + std::to_string(mFaceNeighbors.at(i).at(j)) );
        }
    }
}

std::vector<glm::vec3> SerializedSceneGeometry::GetVertices() const
{
    return mVertexs;
}

std::vector< float > SerializedSceneGeometry::GetVerticesCurvature() const
{
    return mVertexCurvatures;
}

std::vector< std::vector< size_t > > SerializedSceneGeometry::GetFacesNeighbours() const
{
    return mFaceNeighbors;
}

std::vector< float > SerializedSceneGeometry::GetFacesAreas() const
{
    return mFaceAreas;
}

void SerializedSceneGeometry::SerializeSceneNodes(std::shared_ptr<SceneNode const> pSceneNode)
{
    glm::mat4 modelMatrix = pSceneNode->GetGlobalTransform();
    for( int i = 0; i < pSceneNode->GetNumMeshes(); i++ )
    {
        auto geom = pSceneNode->GetMesh(i)->GetGeometry();
        if(geom->GetTopology() == GeometryTopology::Triangles)
        {
            int facesOffset = mNumberOfVertexs;
            const std::vector<float>& vertices = geom->GetVerticesData();
            for( int j = 0; j < vertices.size(); j += geom->GetVertexStride() )
            {
                glm::vec4 vertex(vertices.at(j), vertices.at(j+1), vertices.at(j+2), 1.0f);
                vertex = modelMatrix * vertex;
                mVertexs[mNumberOfVertexs] = glm::vec3(vertex[0], vertex[1], vertex[2]);
                mNumberOfVertexs++;
            }
            const std::vector<unsigned int> indexs = geom->GetIndexsData();
            for( int j = 0; j < indexs.size(); j += 3 )
            {
                glm::ivec3 face( indexs.at(j) + facesOffset, indexs.at(j+1) + facesOffset, indexs.at(j+2) + facesOffset );
                mFaces[mNumberOfFaces] = face;
                mNumberOfFaces++;
            }
        }
        else
        {
            Debug::Error("The serialization only works when the scene is only compunded by triangles");
        }
    }
    for( int i = 0; i < pSceneNode->GetNumChilds(); i++ )
    {
        SerializeSceneNodes(pSceneNode->GetChild(i));
    }
}
