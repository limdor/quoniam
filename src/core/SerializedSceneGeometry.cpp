#include "SerializedSceneGeometry.h"

#include "Debug.h"
#include "Tools.h"

#include <QtCore/QTime>

#include "glm/exponential.hpp"
#include "glm/geometric.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/gtx/norm.hpp"
#include "glm/gtx/vector_angle.hpp"

#include "Miniball.h"

SerializedSceneGeometry::SerializedSceneGeometry(const Scene *pScene):
    mNumberOfVertexs(0), mNumberOfFaces(0)
{
    mVertexs.resize(pScene->GetNumberOfVertices());
    mVertexNeighbors.resize(pScene->GetNumberOfVertices());
    mVertexCurvatures.resize(pScene->GetNumberOfVertices());
    mFaces.resize(pScene->GetNumberOfPolygons());
    mFaceNeighbors.resize(pScene->GetNumberOfPolygons());
    mFaceAreas.resize(pScene->GetNumberOfPolygons());
    SerializeSceneNodes(pScene->GetRootNode());
    Q_ASSERT(mVertexs.size() == mNumberOfVertexs && mFaces.size() == mNumberOfFaces);
    ComputeBoundingSphere();
    ComputeNeighbourhood();
    ComputeFaceAreas();
    ComputeVertexCurvatures();
}

SerializedSceneGeometry::~SerializedSceneGeometry()
{
    delete mBoundingSphere;
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
    mBoundingSphere = new BoundingSphere();
    glm::vec3 sphereCenter;
    sphereCenter.x = mb.center()[0];
    sphereCenter.y = mb.center()[1];
    sphereCenter.z = mb.center()[2];
    if(!mb.is_valid(1e-6))
    {
        double slack;
        double error = mb.accuracy(slack);
        Debug::Warning(QString("Bounding sphere in SerializedSceneGeometry may be invalid. Accuracy: %1").arg(error));
    }

    mBoundingSphere->SetCenter(sphereCenter);
    mBoundingSphere->SetRadius(glm::sqrt(mb.squared_radius()));
}

void SerializedSceneGeometry::ComputeNeighbourhood()
{
    QTime t;

    t.start();

    float epsilon = mBoundingSphere->GetRadius() / 1000000.0f;
    Debug::Log( QString("SerializedSceneGeometry::ComputeNeighbourhood: Epsilon value: %1").arg(epsilon) );

    QVector< QPair< int, glm::vec3 > > points(mNumberOfFaces * 3);
    //Es posen tots els punts de l'escena a una llista de punts ("points") amb informació de a quin mesh pertanyen, l'index del vèrtex en el mesh i la posició del vèrtex
    for( int j = 0; j < mNumberOfFaces; j++ )
    {
        points[j*3 + 0] = QPair< int, glm::vec3 >(j*3 + 0, mVertexs.at(mFaces.at(j)[0]));
        points[j*3 + 1] = QPair< int, glm::vec3 >(j*3 + 1, mVertexs.at(mFaces.at(j)[1]));
        points[j*3 + 2] = QPair< int, glm::vec3 >(j*3 + 2, mVertexs.at(mFaces.at(j)[2]));
    }

    QVector< QPair< int, glm::vec3 > > pointsOrderedByX = points;
    QVector< QPair< int, glm::vec3 > > pointsOrderedByY = points;
    QVector< QPair< int, glm::vec3 > > pointsOrderedByZ = points;

    points.clear();

    //S'ordenen els punts per la coordenada X, la coordenada Y i la coordenada Z
    QVector< int > pointsX = Tools::GetOrderedIndexesByDimension(pointsOrderedByX, 0);
    pointsX = Tools::GetPositions(pointsX);
    QVector< int > pointsY = Tools::GetOrderedIndexesByDimension(pointsOrderedByY, 1);
    pointsY = Tools::GetPositions(pointsY);
    QVector< int > pointsZ = Tools::GetOrderedIndexesByDimension(pointsOrderedByZ, 2);
    pointsZ = Tools::GetPositions(pointsZ);

    int verticesWithoutNeighbours = 0;

    for( int j = 0; j < mNumberOfFaces * 3; j++ )
    {
        QVector< int > neighboursX = Tools::FindNearestThanEpsilonByDimension( pointsX.at(j), pointsOrderedByX, epsilon, 0 );
        QVector< int > neighboursY = Tools::FindNearestThanEpsilonByDimension( pointsY.at(j), pointsOrderedByY, epsilon, 1 );
        QVector< int > neighboursZ = Tools::FindNearestThanEpsilonByDimension( pointsZ.at(j), pointsOrderedByZ, epsilon, 2 );
        QVector< int > neighbours = Tools::MergeNeighbours(neighboursX, neighboursY, neighboursZ);

        mVertexNeighbors[ mFaces.at(j / 3)[j % 3] ] += neighbours;
    }
    for( int j = 0; j < mVertexNeighbors.size(); j++ )
    {
        qSort(mVertexNeighbors[j].begin(), mVertexNeighbors[j].end());
        QVector< int >::iterator it = std::unique(mVertexNeighbors[j].begin(), mVertexNeighbors[j].end());
        mVertexNeighbors[j].resize( it - mVertexNeighbors[j].begin() );
        if( mVertexNeighbors.at(j).size() == 0 )
        {
            verticesWithoutNeighbours++;
        }
    }

    if( verticesWithoutNeighbours > 1 )
    {
        Debug::Warning( QString("%1 vertices without neighbours").arg(verticesWithoutNeighbours) );
    }
    else if( verticesWithoutNeighbours == 1 )
    {
        Debug::Warning("1 vertex without neighbours");
    }

    //Using the neighbourhood of the vertex we compute the neighbourhood of the polygons
    for( int j = 0; j < mNumberOfFaces; j++ )
    {
        QVector< int > polygonNeighbours;
        for( int k = 0; k < 3; k++ )
        {
            polygonNeighbours += mVertexNeighbors.at( mFaces.at(j)[k] );
        }

        qSort(polygonNeighbours.begin(), polygonNeighbours.end());
        QVector< int >::iterator it = std::unique(polygonNeighbours.begin(), polygonNeighbours.end());
        polygonNeighbours.resize( it - polygonNeighbours.begin() );

        mFaceNeighbors[j] = polygonNeighbours;
    }

    Debug::Log(QString("Scene::ComputeNeighbourhood - Total time elapsed: %1 ms").arg(t.elapsed()));
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
    for( int i = 0; i < mNumberOfFaces; i++ )
    {
        mFaceAreas[ i ] = 0.5f * glm::length( glm::cross( mVertexs.at( mFaces.at(i)[1] ) - mVertexs.at( mFaces.at(i)[0] ), mVertexs.at( mFaces.at(i)[2] ) - mVertexs.at( mFaces.at(i)[0] ) ) );
    }
}

void SerializedSceneGeometry::ShowNeighbours() const
{
    Debug::Log( "-- Vertex neighbours --" );
    for( int i = 0; i < mVertexNeighbors.size(); i++ )
    {
        Debug::Log( QString("Neighbours of %1:").arg(i) );
        for( int j = 0; j < mVertexNeighbors.at(i).size(); j++ )
        {
            Debug::Log( QString("   %1").arg(mVertexNeighbors.at(i).at(j)) );
        }
    }
    Debug::Log( "-- Polygon neighbours --" );
    for( int i = 0; i < mFaceNeighbors.size(); i++ )
    {
        Debug::Log( QString("Neighbours of %1:").arg(i) );
        for( int j = 0; j < mFaceNeighbors.at(i).size(); j++ )
        {
            Debug::Log( QString("   %1").arg(mFaceNeighbors.at(i).at(j)) );
        }
    }
}

QVector<glm::vec3> SerializedSceneGeometry::GetVertices() const
{
    return mVertexs;
}

QVector< float > SerializedSceneGeometry::GetVerticesCurvature() const
{
    return mVertexCurvatures;
}

QVector< QVector< int > > SerializedSceneGeometry::GetFacesNeighbours() const
{
    return mFaceNeighbors;
}

QVector< float > SerializedSceneGeometry::GetFacesAreas() const
{
    return mFaceAreas;
}

void SerializedSceneGeometry::SerializeSceneNodes(const SceneNode *pSceneNode)
{
    glm::mat4 modelMatrix = pSceneNode->GetGlobalTransform();
    for( int i = 0; i < pSceneNode->GetNumMeshes(); i++ )
    {
        Geometry* geom = pSceneNode->GetMesh(i)->GetGeometry();
        if(geom->GetTopology() == Geometry::Triangles)
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
