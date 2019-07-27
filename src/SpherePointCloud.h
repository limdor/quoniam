/// \file SpherePointCloud.h
/// \class SpherePointCloud
/// \author Francisco González
/// \author Marc Ruiz
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef SPHERE_POINT_CLOUD_H
#define SPHERE_POINT_CLOUD_H

//Qt includes
#include <QVector>

//Dependency includes
#include "glm/vec3.hpp"

//Project includes
#include "Geometry.h"

///
/// Sphere of points uniformly or quasi-uniformly distributed
///
class SpherePointCloud {

public:
    /// Retorna un vector cap amunt per al punt de vista passat.
    static glm::vec3 Up(const glm::vec3 &pViewpoint);

    /// Constructor
    SpherePointCloud();
    /// Copy constructor
    SpherePointCloud( const SpherePointCloud& pSpherePointCloud );
    /// Destructor
    ~SpherePointCloud();

    /// Generates 4 points uniformly distributed
    void SetToUniform4();
    /// Generates 6 points uniformly distributed
    void SetToUniform6();
    /// Generates 8 points uniformly distributed
    void SetToUniform8();
    /// Generates 12 points uniformly distributed
    void SetToUniform12();
    /// Generates 20 points uniformly distributed
    void SetToUniform20();
    /// Generate 10*4^depth+2 points quasi-uniformly distributed using the recursive
    /// subdivision of faces, starting from an icosahedron
    void SetToQuasiUniform(unsigned char pDepth);

    /// Return the faces of the triangles
    QVector<unsigned int> GetFaces() const;
    /// Return the points in cartesian coordinates
    QVector<glm::vec3> GetVertices() const;

    /// Return the vertex \a pI
    glm::vec3 GetVertex(unsigned int pI) const;
    /// Return the neighbours of every point
    QVector< QVector<int> > GetNeighbours() const;
    /// Return the neighbours of the point \a pI
    QVector<int> GetNeighbours(unsigned int pI) const;

    /// Return the 3D mesh
    Geometry* GetMesh() const;

private:
    /// Compute the neighbours of the quasi uniform distribution
    void ComputeQuasiUniformNeighbours();
    /// Create the vertexs, faces and normals of an icosahedron (level 0)
    void CreateIcosahedron();
    /// Find if a vertex already exists, if exists return the index
    bool FindSphereCloudVertex(const glm::vec3 &pV, unsigned int &pPosition) const;
    /// Create a new face with 3 vertex from the point cloud
    void CreateSphereCloudTriangle(const glm::vec3 &pV1, const glm::vec3 &pV2, const glm::vec3 &pV3);
    /// Subdivide the point cloud recursively until \a pDepth
    void Subdivide(glm::vec3 pV1, glm::vec3 pV2, glm::vec3 pV3, unsigned short pDepth);

protected:
    /// Configure the mesh
    void SetMeshInformation();
    /// Mesh to paint the sphere point cloud
    Geometry* mMesh;
    /// Number of points
    int mNumberOfPoints;
    /// Faces
    QVector< unsigned int > mFaces;
    /// Vertexs in cartesian coordinates
    QVector< glm::vec3 > mVertices;
    /// Normals of the vertexs
    QVector< glm::vec3 > mNormals;
    /// Neighbours of the vertexs
    QVector< QVector<int> > mNeighbours;
};

#endif
