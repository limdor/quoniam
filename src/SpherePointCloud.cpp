//Definition include
#include "SpherePointCloud.h"

//Dependency includes
#include "glm/common.hpp"
#include "glm/geometric.hpp"
#include "glm/exponential.hpp"

glm::vec3 SpherePointCloud::Up(const glm::vec3 &pViewpoint)
{
    glm::vec3 front = -pViewpoint;
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    if(glm::abs(front.y) > 0.66)
    {
        if(front.x < 0.0f)
        {
            up = glm::vec3(1.0f, 0.0f, 0.0f);
        }
        else
        {
            up = glm::vec3(-1.0f, 0.0f, 0.0f);
        }
    }
    glm::vec3 left = glm::cross(up, front);
    up = glm::normalize(glm::cross(front, left));

    return up;
}

SpherePointCloud::SpherePointCloud():
    mNumberOfPoints(0)
{
    mMesh = std::make_shared<Geometry>("Sphere point cloud", GeometryTopology::Points);
}

void SpherePointCloud::SetToUniform4()
{
    const double UNIT = 1.0f / glm::sqrt(3.0f);

    mNumberOfPoints = 4;
    mVertices.resize(mNumberOfPoints);
    mVertices[0] = glm::vec3(  UNIT,  UNIT,  UNIT );  // 0
    mVertices[1] = glm::vec3( -UNIT, -UNIT,  UNIT );  // 1
    mVertices[2] = glm::vec3( -UNIT,  UNIT, -UNIT );  // 2
    mVertices[3] = glm::vec3(  UNIT, -UNIT, -UNIT );  // 3

    mNormals = mVertices;

    mFaces.resize(12);
    mFaces[0] = 0; mFaces[ 1] = 1; mFaces[ 2] = 3;
    mFaces[3] = 0; mFaces[ 4] = 2; mFaces[ 5] = 1;
    mFaces[6] = 0; mFaces[ 7] = 3; mFaces[ 8] = 2;
    mFaces[9] = 1; mFaces[10] = 2; mFaces[11] = 3;

    mNeighbours.resize(mNumberOfPoints);
    for( int i = 0; i < mNumberOfPoints; i++ )
    {
        mNeighbours[i].resize(3);
    }
    mNeighbours[0][0] = 1; mNeighbours[0][1] = 2; mNeighbours[0][2] = 3;
    mNeighbours[1][0] = 0; mNeighbours[1][1] = 2; mNeighbours[1][2] = 3;
    mNeighbours[2][0] = 0; mNeighbours[2][1] = 1; mNeighbours[2][2] = 3;
    mNeighbours[3][0] = 0; mNeighbours[3][1] = 1; mNeighbours[3][2] = 2;

    SetMeshInformation();
}

void SpherePointCloud::SetToUniform6()
{
    mNumberOfPoints = 6;
    mVertices.resize(mNumberOfPoints);
    mVertices[0] = glm::vec3(  1.0f,  0.0f,  0.0f );  // 0
    mVertices[1] = glm::vec3( -1.0f,  0.0f,  0.0f );  // 1
    mVertices[2] = glm::vec3(  0.0f,  1.0f,  0.0f );  // 2
    mVertices[3] = glm::vec3(  0.0f, -1.0f,  0.0f );  // 3
    mVertices[4] = glm::vec3(  0.0f,  0.0f,  1.0f );  // 4
    mVertices[5] = glm::vec3(  0.0f,  0.0f, -1.0f );  // 5

    mNormals = mVertices;

    mFaces.resize(24);
    mFaces[ 0] = 0; mFaces[ 1] = 4; mFaces[ 2] = 3;
    mFaces[ 3] = 1; mFaces[ 4] = 3; mFaces[ 5] = 4;
    mFaces[ 6] = 1; mFaces[ 7] = 5; mFaces[ 8] = 3;
    mFaces[ 9] = 0; mFaces[10] = 3; mFaces[11] = 5;
    mFaces[12] = 1; mFaces[13] = 2; mFaces[14] = 5;
    mFaces[15] = 1; mFaces[16] = 4; mFaces[17] = 2;
    mFaces[18] = 0; mFaces[19] = 2; mFaces[20] = 4;
    mFaces[21] = 0; mFaces[22] = 5; mFaces[23] = 2;

    mNeighbours.resize(mNumberOfPoints);
    for( int i = 0; i < mNumberOfPoints; i++ )
    {
        mNeighbours[i].resize(4);
    }
    mNeighbours[0][0] = 2; mNeighbours[0][1] = 3; mNeighbours[0][2] = 4; mNeighbours[0][3] = 5;
    mNeighbours[1][0] = 2; mNeighbours[1][1] = 3; mNeighbours[1][2] = 4; mNeighbours[1][3] = 5;
    mNeighbours[2][0] = 0; mNeighbours[2][1] = 1; mNeighbours[2][2] = 4; mNeighbours[2][3] = 5;
    mNeighbours[3][0] = 0; mNeighbours[3][1] = 1; mNeighbours[3][2] = 4; mNeighbours[3][3] = 5;
    mNeighbours[4][0] = 0; mNeighbours[4][1] = 1; mNeighbours[4][2] = 2; mNeighbours[4][3] = 3;
    mNeighbours[5][0] = 0; mNeighbours[5][1] = 1; mNeighbours[5][2] = 2; mNeighbours[5][3] = 3;

    SetMeshInformation();
}

void SpherePointCloud::SetToUniform8()
{
    const double UNIT = 1.0f / glm::sqrt(3.0f);

    mNumberOfPoints = 8;
    mVertices.resize(mNumberOfPoints);
    mVertices[0] = glm::vec3(  UNIT,  UNIT,  UNIT ); // 0
    mVertices[1] = glm::vec3(  UNIT,  UNIT, -UNIT ); // 1
    mVertices[2] = glm::vec3(  UNIT, -UNIT,  UNIT ); // 2
    mVertices[3] = glm::vec3(  UNIT, -UNIT, -UNIT ); // 3
    mVertices[4] = glm::vec3( -UNIT,  UNIT,  UNIT ); // 4
    mVertices[5] = glm::vec3( -UNIT,  UNIT, -UNIT ); // 5
    mVertices[6] = glm::vec3( -UNIT, -UNIT,  UNIT ); // 6
    mVertices[7] = glm::vec3( -UNIT, -UNIT, -UNIT ); // 7

    mNormals = mVertices;

    mFaces.resize(36);
    mFaces[ 0] = 0; mFaces[ 1] = 6; mFaces[ 2] = 2;
    mFaces[ 3] = 0; mFaces[ 4] = 4; mFaces[ 5] = 6;
    mFaces[ 6] = 0; mFaces[ 7] = 2; mFaces[ 8] = 3;
    mFaces[ 9] = 0; mFaces[10] = 3; mFaces[11] = 1;
    mFaces[12] = 0; mFaces[13] = 5; mFaces[14] = 4;
    mFaces[15] = 0; mFaces[16] = 1; mFaces[17] = 5;
    mFaces[18] = 2; mFaces[19] = 6; mFaces[20] = 3;
    mFaces[21] = 3; mFaces[22] = 6; mFaces[23] = 7;
    mFaces[24] = 4; mFaces[25] = 5; mFaces[26] = 6;
    mFaces[27] = 5; mFaces[28] = 7; mFaces[29] = 6;
    mFaces[30] = 1; mFaces[31] = 3; mFaces[32] = 5;
    mFaces[33] = 3; mFaces[34] = 7; mFaces[35] = 5;

    mNeighbours.resize(mNumberOfPoints);
    for( int i = 0; i < mNumberOfPoints; i++ )
    {
        mNeighbours[i].resize(3);
    }
    mNeighbours[0][0] = 1; mNeighbours[0][1] = 2; mNeighbours[0][2] = 4;
    mNeighbours[1][0] = 0; mNeighbours[1][1] = 3; mNeighbours[1][2] = 5;
    mNeighbours[2][0] = 0; mNeighbours[2][1] = 3; mNeighbours[2][2] = 6;
    mNeighbours[3][0] = 1; mNeighbours[3][1] = 2; mNeighbours[3][2] = 7;
    mNeighbours[4][0] = 0; mNeighbours[4][1] = 5; mNeighbours[4][2] = 6;
    mNeighbours[5][0] = 1; mNeighbours[5][1] = 4; mNeighbours[5][2] = 7;
    mNeighbours[6][0] = 2; mNeighbours[6][1] = 4; mNeighbours[6][2] = 7;
    mNeighbours[7][0] = 3; mNeighbours[7][1] = 5; mNeighbours[7][2] = 6;

    SetMeshInformation();
}

void SpherePointCloud::SetToUniform12()
{
    glm::vec3 v( 1.0f, 0.0f, ( 1.0f + glm::sqrt( 5.0f ) ) / 2.0f );
    v = glm::normalize(v);

    const double PHI_X = v.x;
    const double PHI_Z = v.z;

    mNumberOfPoints = 12;
    mVertices.resize(mNumberOfPoints);
    mVertices[ 0] = glm::vec3(   0.0f,  PHI_X,  PHI_Z ); //  0
    mVertices[ 1] = glm::vec3(   0.0f,  PHI_X, -PHI_Z ); //  1
    mVertices[ 2] = glm::vec3(   0.0f, -PHI_X,  PHI_Z ); //  2
    mVertices[ 3] = glm::vec3(   0.0f, -PHI_X, -PHI_Z ); //  3
    mVertices[ 4] = glm::vec3(  PHI_X,  PHI_Z,   0.0f ); //  4
    mVertices[ 5] = glm::vec3(  PHI_X, -PHI_Z,   0.0f ); //  5
    mVertices[ 6] = glm::vec3( -PHI_X,  PHI_Z,   0.0f ); //  6
    mVertices[ 7] = glm::vec3( -PHI_X, -PHI_Z,   0.0f ); //  7
    mVertices[ 8] = glm::vec3(  PHI_Z,   0.0f,  PHI_X ); //  8
    mVertices[ 9] = glm::vec3(  PHI_Z,   0.0f, -PHI_X ); //  9
    mVertices[10] = glm::vec3( -PHI_Z,   0.0f,  PHI_X ); // 10
    mVertices[11] = glm::vec3( -PHI_Z,   0.0f, -PHI_X ); // 11

    mNormals = mVertices;

    mFaces.resize(60);
    mFaces[ 0] =  6; mFaces[ 1] = 10; mFaces[ 2] =  0;
    mFaces[ 3] =  6; mFaces[ 4] =  0; mFaces[ 5] =  4;
    mFaces[ 6] =  6; mFaces[ 7] =  4; mFaces[ 8] =  1;
    mFaces[ 9] =  6; mFaces[10] =  1; mFaces[11] = 11;
    mFaces[12] =  6; mFaces[13] = 11; mFaces[14] = 10;
    mFaces[15] =  4; mFaces[16] =  0; mFaces[17] =  8;
    mFaces[18] =  0; mFaces[19] = 10; mFaces[20] =  2;
    mFaces[21] = 10; mFaces[22] = 11; mFaces[23] =  7;
    mFaces[24] = 11; mFaces[25] =  1; mFaces[26] =  3;
    mFaces[27] =  1; mFaces[28] =  4; mFaces[29] =  9;
    mFaces[30] =  5; mFaces[31] =  8; mFaces[32] =  2;
    mFaces[33] =  5; mFaces[34] =  2; mFaces[35] =  7;
    mFaces[36] =  5; mFaces[37] =  7; mFaces[38] =  3;
    mFaces[39] =  5; mFaces[40] =  3; mFaces[41] =  9;
    mFaces[42] =  5; mFaces[43] =  9; mFaces[44] =  8;
    mFaces[45] =  2; mFaces[46] =  8; mFaces[47] =  0;
    mFaces[48] =  7; mFaces[49] =  2; mFaces[50] = 10;
    mFaces[51] =  3; mFaces[52] =  7; mFaces[53] = 11;
    mFaces[54] =  9; mFaces[55] =  3; mFaces[56] =  1;
    mFaces[57] =  8; mFaces[58] =  9; mFaces[59] =  4;

    mNeighbours.resize(mNumberOfPoints);
    for( int i = 0; i < mNumberOfPoints; i++ )
    {
        mNeighbours[i].resize(5);
    }
    mNeighbours[ 0][0] = 2; mNeighbours[ 0][1] = 4; mNeighbours[ 0][2] = 6; mNeighbours[ 0][3] =  8; mNeighbours[ 0][4] = 10;
    mNeighbours[ 1][0] = 3; mNeighbours[ 1][1] = 4; mNeighbours[ 1][2] = 6; mNeighbours[ 1][3] =  9; mNeighbours[ 1][4] = 11;
    mNeighbours[ 2][0] = 0; mNeighbours[ 2][1] = 5; mNeighbours[ 2][2] = 7; mNeighbours[ 2][3] =  8; mNeighbours[ 2][4] = 10;
    mNeighbours[ 3][0] = 1; mNeighbours[ 3][1] = 5; mNeighbours[ 3][2] = 7; mNeighbours[ 3][3] =  9; mNeighbours[ 3][4] = 11;
    mNeighbours[ 4][0] = 0; mNeighbours[ 4][1] = 1; mNeighbours[ 4][2] = 6; mNeighbours[ 4][3] =  8; mNeighbours[ 4][4] =  9;
    mNeighbours[ 5][0] = 2; mNeighbours[ 5][1] = 3; mNeighbours[ 5][2] = 7; mNeighbours[ 5][3] =  8; mNeighbours[ 5][4] =  9;
    mNeighbours[ 6][0] = 0; mNeighbours[ 6][1] = 1; mNeighbours[ 6][2] = 4; mNeighbours[ 6][3] = 10; mNeighbours[ 6][4] = 11;
    mNeighbours[ 7][0] = 2; mNeighbours[ 7][1] = 3; mNeighbours[ 7][2] = 5; mNeighbours[ 7][3] = 10; mNeighbours[ 7][4] = 11;
    mNeighbours[ 8][0] = 0; mNeighbours[ 8][1] = 2; mNeighbours[ 8][2] = 4; mNeighbours[ 8][3] =  5; mNeighbours[ 8][4] =  9;
    mNeighbours[ 9][0] = 1; mNeighbours[ 9][1] = 3; mNeighbours[ 9][2] = 4; mNeighbours[ 9][3] =  5; mNeighbours[ 9][4] =  8;
    mNeighbours[10][0] = 0; mNeighbours[10][1] = 2; mNeighbours[10][2] = 6; mNeighbours[10][3] =  7; mNeighbours[10][4] = 11;
    mNeighbours[11][0] = 1; mNeighbours[11][1] = 3; mNeighbours[11][2] = 6; mNeighbours[11][3] =  7; mNeighbours[11][4] = 10;

    SetMeshInformation();
}

void SpherePointCloud::SetToUniform20()
{
    const float UNIT = 1.0f / glm::sqrt( 3.0f );
    const double PHI = ( 1.0f + glm::sqrt( 5.0f ) ) / 2.0f;
    const double INV_PHI = 1.0f / PHI;

    mNumberOfPoints = 20;
    mVertices.resize(mNumberOfPoints);
    mVertices[ 0] = glm::vec3(     UNIT,     UNIT,     UNIT );         //  0
    mVertices[ 1] = glm::vec3(     UNIT,     UNIT,    -UNIT );         //  1
    mVertices[ 2] = glm::vec3(     UNIT,    -UNIT,     UNIT );         //  2
    mVertices[ 3] = glm::vec3(     UNIT,    -UNIT,    -UNIT );         //  3
    mVertices[ 4] = glm::vec3(    -UNIT,     UNIT,     UNIT );         //  4
    mVertices[ 5] = glm::vec3(    -UNIT,     UNIT,    -UNIT );         //  5
    mVertices[ 6] = glm::vec3(    -UNIT,    -UNIT,     UNIT );         //  6
    mVertices[ 7] = glm::vec3(    -UNIT,    -UNIT,    -UNIT );         //  7
    mVertices[ 8] = glm::vec3(     0.0f,  INV_PHI,      PHI ) * UNIT;  //  8
    mVertices[ 9] = glm::vec3(     0.0f,  INV_PHI,     -PHI ) * UNIT;  //  9
    mVertices[10] = glm::vec3(     0.0f, -INV_PHI,      PHI ) * UNIT;  // 10
    mVertices[11] = glm::vec3(     0.0f, -INV_PHI,     -PHI ) * UNIT;  // 11
    mVertices[12] = glm::vec3(  INV_PHI,      PHI,     0.0f ) * UNIT;  // 12
    mVertices[13] = glm::vec3(  INV_PHI,     -PHI,     0.0f ) * UNIT;  // 13
    mVertices[14] = glm::vec3( -INV_PHI,      PHI,     0.0f ) * UNIT;  // 14
    mVertices[15] = glm::vec3( -INV_PHI,     -PHI,     0.0f ) * UNIT;  // 15
    mVertices[16] = glm::vec3(      PHI,     0.0f,  INV_PHI ) * UNIT;  // 16
    mVertices[17] = glm::vec3(      PHI,     0.0f, -INV_PHI ) * UNIT;  // 17
    mVertices[18] = glm::vec3(     -PHI,     0.0f,  INV_PHI ) * UNIT;  // 18
    mVertices[19] = glm::vec3(     -PHI,     0.0f, -INV_PHI ) * UNIT;  // 19

    mNormals = mVertices;

    //TODO: Add faces

    mNeighbours.resize(mNumberOfPoints);
    for( int i = 0; i < mNumberOfPoints; i++ )
    {
        mNeighbours[i].resize(3);
    }
    mNeighbours[ 0][0] =  8; mNeighbours[ 0][1] = 12; mNeighbours[ 0][2] = 16;
    mNeighbours[ 1][0] =  9; mNeighbours[ 1][1] = 12; mNeighbours[ 1][2] = 17;
    mNeighbours[ 2][0] = 10; mNeighbours[ 2][1] = 13; mNeighbours[ 2][2] = 16;
    mNeighbours[ 3][0] = 11; mNeighbours[ 3][1] = 13; mNeighbours[ 3][2] = 17;
    mNeighbours[ 4][0] =  8; mNeighbours[ 4][1] = 14; mNeighbours[ 4][2] = 18;
    mNeighbours[ 5][0] =  9; mNeighbours[ 5][1] = 14; mNeighbours[ 5][2] = 19;
    mNeighbours[ 6][0] = 10; mNeighbours[ 6][1] = 15; mNeighbours[ 6][2] = 18;
    mNeighbours[ 7][0] = 11; mNeighbours[ 7][1] = 15; mNeighbours[ 7][2] = 19;
    mNeighbours[ 8][0] =  0; mNeighbours[ 8][1] =  4; mNeighbours[ 8][2] = 10;
    mNeighbours[ 9][0] =  1; mNeighbours[ 9][1] =  5; mNeighbours[ 9][2] = 11;
    mNeighbours[10][0] =  2; mNeighbours[10][1] =  6; mNeighbours[10][2] =  8;
    mNeighbours[11][0] =  3; mNeighbours[11][1] =  7; mNeighbours[11][2] =  9;
    mNeighbours[12][0] =  0; mNeighbours[12][1] =  1; mNeighbours[12][2] = 14;
    mNeighbours[13][0] =  2; mNeighbours[13][1] =  3; mNeighbours[13][2] = 15;
    mNeighbours[14][0] =  4; mNeighbours[14][1] =  5; mNeighbours[14][2] = 12;
    mNeighbours[15][0] =  7; mNeighbours[15][1] =  6; mNeighbours[15][2] = 13;
    mNeighbours[16][0] =  0; mNeighbours[16][1] =  2; mNeighbours[16][2] = 17;
    mNeighbours[17][0] =  1; mNeighbours[17][1] =  3; mNeighbours[17][2] = 16;
    mNeighbours[18][0] =  4; mNeighbours[18][1] =  6; mNeighbours[18][2] = 19;
    mNeighbours[19][0] =  5; mNeighbours[19][1] =  7; mNeighbours[19][2] = 18;

    SetMeshInformation();
}

void SpherePointCloud::SetToQuasiUniform(unsigned char pDepth)
{
    // Create the initial vertices and faces from the sphere cloud
    CreateIcosahedron();

    QVector< unsigned int > tempFaces = mFaces;

    mFaces.clear();

    // Each triangle has to be subdivided as many times as the sphere depth
    for( int i = 0; i < tempFaces.size(); i += 3 )
    {
        int face1 = tempFaces.at(i);
        int face2 = tempFaces.at(i+1);
        int face3 = tempFaces.at(i+2);

        Subdivide( mVertices.at(face1), mVertices.at(face2), mVertices.at(face3), pDepth );
    }
    mNumberOfPoints = mVertices.size();
    ComputeQuasiUniformNeighbours();
    SetMeshInformation();
}

QVector<unsigned int> SpherePointCloud::GetFaces() const
{
    return mFaces;
}

QVector<glm::vec3> SpherePointCloud::GetVertices() const
{
    return mVertices;
}

glm::vec3 SpherePointCloud::GetVertex(unsigned int pI) const
{
    return mVertices.at(pI);
}

QVector<QVector<int> > SpherePointCloud::GetNeighbours() const
{
    return mNeighbours;
}

QVector<int> SpherePointCloud::GetNeighbours(unsigned int pI) const
{
    return mNeighbours.at(pI);
}

std::shared_ptr<Geometry> SpherePointCloud::GetMesh() const
{
    return mMesh;
}

void SpherePointCloud::ComputeQuasiUniformNeighbours()
{
    mNeighbours.clear();
    int nFaces = mFaces.size() / 3;
    Q_ASSERT( nFaces * 3 == mFaces.size() );

    for ( int i = 0; i < mNumberOfPoints; i++ )
    {
        QVector<int> vertexNeighbours;

        for ( int j = 0; j < nFaces; j++ )
        {
            if ( mFaces.at( 3 * j ) == i )
            {
                if ( vertexNeighbours.indexOf( mFaces.at( 3 * j + 1 ) ) == -1 )
                {
                    vertexNeighbours.push_back(mFaces.at( 3 * j + 1 ));
                }
                if ( vertexNeighbours.indexOf( mFaces.at( 3 * j + 2 ) ) == -1 )
                {
                    vertexNeighbours.push_back(mFaces.at( 3 * j + 2 ));
                }
            }

            if ( mFaces.at( 3 * j + 1 ) == i )
            {
                if ( vertexNeighbours.indexOf( mFaces.at( 3 * j ) ) == -1 )
                {
                    vertexNeighbours.push_back(mFaces.at( 3 * j ));
                }
                if ( vertexNeighbours.indexOf( mFaces.at( 3 * j + 2 ) ) == -1 )
                {
                    vertexNeighbours.push_back(mFaces.at( 3 * j + 2 ));
                }
            }

            if ( mFaces.at( 3 * j + 2 ) == i )
            {
                if ( vertexNeighbours.indexOf( mFaces.at( 3 * j ) ) == -1 )
                {
                    vertexNeighbours.push_back(mFaces.at( 3 * j ));
                }
                if ( vertexNeighbours.indexOf( mFaces.at( 3 * j + 1 ) ) == -1 )
                {
                    vertexNeighbours.push_back(mFaces.at( 3 * j + 1 ));
                }
            }
        }
        mNeighbours.push_back(vertexNeighbours);
    }
}

void SpherePointCloud::CreateIcosahedron()
{
    glm::vec3 v( 1.0f, 0.0f, ( 1.0f + glm::sqrt( 5.0f ) ) / 2.0f );
    v = glm::normalize(v);

    const double ICOSAHEDRON_X = v.x;
    const double ICOSAHEDRON_Z = v.z;

    mNumberOfPoints = 12;
    mVertices.resize(mNumberOfPoints);
    mVertices[ 0] = glm::vec3( -ICOSAHEDRON_X, 0.0f,  ICOSAHEDRON_Z );
    mVertices[ 1] = glm::vec3(  ICOSAHEDRON_X, 0.0f,  ICOSAHEDRON_Z );
    mVertices[ 2] = glm::vec3( -ICOSAHEDRON_X, 0.0f, -ICOSAHEDRON_Z );
    mVertices[ 3] = glm::vec3(  ICOSAHEDRON_X, 0.0f, -ICOSAHEDRON_Z );
    mVertices[ 4] = glm::vec3( 0.0f,  ICOSAHEDRON_Z,  ICOSAHEDRON_X );
    mVertices[ 5] = glm::vec3( 0.0f,  ICOSAHEDRON_Z, -ICOSAHEDRON_X );
    mVertices[ 6] = glm::vec3( 0.0f, -ICOSAHEDRON_Z,  ICOSAHEDRON_X );
    mVertices[ 7] = glm::vec3( 0.0f, -ICOSAHEDRON_Z, -ICOSAHEDRON_X );
    mVertices[ 8] = glm::vec3(  ICOSAHEDRON_Z,  ICOSAHEDRON_X, 0.0f );
    mVertices[ 9] = glm::vec3( -ICOSAHEDRON_Z,  ICOSAHEDRON_X, 0.0f );
    mVertices[10] = glm::vec3(  ICOSAHEDRON_Z, -ICOSAHEDRON_X, 0.0f );
    mVertices[11] = glm::vec3( -ICOSAHEDRON_Z, -ICOSAHEDRON_X, 0.0f );

    mNormals = mVertices;

    mFaces.resize(60);
    mFaces[ 0] =  1; mFaces[ 1] =  4; mFaces[ 2] =  0;
    mFaces[ 3] =  4; mFaces[ 4] =  9; mFaces[ 5] =  0;
    mFaces[ 6] =  4; mFaces[ 7] =  5; mFaces[ 8] =  9;
    mFaces[ 9] =  8; mFaces[10] =  5; mFaces[11] =  4;
    mFaces[12] =  1; mFaces[13] =  8; mFaces[14] =  4;
    mFaces[15] =  1; mFaces[16] = 10; mFaces[17] =  8;
    mFaces[18] = 10; mFaces[19] =  3; mFaces[20] =  8;
    mFaces[21] =  8; mFaces[22] =  3; mFaces[23] =  5;
    mFaces[24] =  3; mFaces[25] =  2; mFaces[26] =  5;
    mFaces[27] =  3; mFaces[28] =  7; mFaces[29] =  2;
    mFaces[30] =  3; mFaces[31] = 10; mFaces[32] =  7;
    mFaces[33] = 10; mFaces[34] =  6; mFaces[35] =  7;
    mFaces[36] =  6; mFaces[37] = 11; mFaces[38] =  7;
    mFaces[39] =  6; mFaces[40] =  0; mFaces[41] = 11;
    mFaces[42] =  6; mFaces[43] =  1; mFaces[44] =  0;
    mFaces[45] = 10; mFaces[46] =  1; mFaces[47] =  6;
    mFaces[48] = 11; mFaces[49] =  0; mFaces[50] =  9;
    mFaces[51] =  2; mFaces[52] = 11; mFaces[53] =  9;
    mFaces[54] =  5; mFaces[55] =  2; mFaces[56] =  9;
    mFaces[57] = 11; mFaces[58] =  2; mFaces[59] =  7;
}

inline bool equal(const glm::vec3 & v1, const glm::vec3 & v2, float error)
{
    glm::vec3 v = v1 - v2;

    return ( glm::abs( v.x ) <= error && glm::abs( v.y ) <= error && glm::abs( v.z ) <= error );
}

bool SpherePointCloud::FindSphereCloudVertex(const glm::vec3 & pV, unsigned int & pPosition) const
{
    int i = 0;
    bool trobat = false;

    while ( i < mVertices.size() && !trobat )
    {
        if ( equal( pV, mVertices.at(i), 0.0001f ) ) trobat = true;
        else i++;
    }
    pPosition = i;

    return trobat;
}

void SpherePointCloud::CreateSphereCloudTriangle(const glm::vec3 & pV1, const glm::vec3 & pV2, const glm::vec3 & pV3)
{
    unsigned int pos;

    // Add the triangle (v1, v2, v3) to the list --> Add vertexs (if needed) and faces!!!

    if( FindSphereCloudVertex( pV1, pos ) )
    {
        // Vertex finded at position pos --> Only the index of the face is added
        mFaces.push_back( pos );
    }
    else
    {
        // Add the vertex and the index of the face
        mVertices.push_back( pV1 );
        mNormals.push_back( pV1 );
        mFaces.push_back( mVertices.size() - 1 );
    }

    if( FindSphereCloudVertex( pV2, pos ) )
    {
        // Vertex finded at position pos --> Only the index of the face is added
        mFaces.push_back( pos );
    }
    else
    {
        // Add the vertex and the index of the face
        mVertices.push_back( pV2 );
        mNormals.push_back( pV2 );
        mFaces.push_back( mVertices.size() - 1 );
    }

    if( FindSphereCloudVertex( pV3, pos ) )
    {
        // Vertex finded at position pos --> Only the index of the face is added
        mFaces.push_back( pos );
    }
    else
    {
        // Add the vertex and the index of the face
        mVertices.push_back( pV3 );
        mNormals.push_back( pV3 );
        mFaces.push_back( mVertices.size() - 1 );
    }
}

void SpherePointCloud::Subdivide(glm::vec3 pV1, glm::vec3 pV2, glm::vec3 pV3, unsigned short pDepth)
{
    if( pDepth == 0 )
    {
        CreateSphereCloudTriangle( pV1, pV2, pV3 );
    }
    else
    {
        // Find the midpoint of each edge of the triangle
        glm::vec3 v12 = pV1 + pV2;
        glm::vec3 v23 = pV2 + pV3;
        glm::vec3 v31 = pV3 + pV1;

        // It's not necessary to divide by 2 the midpoints because we normalize them
        v12 = glm::normalize(v12);
        v23 = glm::normalize(v23);
        v31 = glm::normalize(v31);

        Subdivide( pV1, v12, v31, pDepth - 1 );
        Subdivide( pV2, v23, v12, pDepth - 1 );
        Subdivide( pV3, v31, v23, pDepth - 1 );
        Subdivide( v12, v23, v31, pDepth - 1 );
    }
}

void SpherePointCloud::SetMeshInformation()
{
    mMesh->SetVerticesData(mNumberOfPoints, mVertices.data());
    mMesh->SetNormalsData(mNumberOfPoints, mNormals.data());
    if(mFaces.size() > 0)
    {
        mMesh->SetIndexsData(mFaces.size(), mFaces.data());
        mMesh->SetTopology(GeometryTopology::Triangles);
    }
    mMesh->ComputeBoundingVolumes();
}
