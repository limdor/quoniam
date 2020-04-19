#ifndef SPHERE_POINT_CLOUD_H
#define SPHERE_POINT_CLOUD_H

//Dependency includes
#include "glm/vec3.hpp"

//Project includes
#include "Geometry.h"

#include <vector>

///
/// Sphere of points uniformly or quasi-uniformly distributed
///
class SpherePointCloud {

public:
    /// Retorna un vector cap amunt per al punt de vista passat.
    static glm::vec3 Up(const glm::vec3 &pViewpoint);

    SpherePointCloud();
    /// Copy constructor
    SpherePointCloud( const SpherePointCloud& pSpherePointCloud ) = delete;
    /// Destructor
    virtual ~SpherePointCloud() = default;

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
    std::vector<size_t> GetFaces() const;
    /// Return the points in cartesian coordinates
    std::vector<glm::vec3> GetVertices() const;

    /// Return the vertex \a pI
    glm::vec3 GetVertex(size_t pI) const;
    /// Return the neighbours of every point
    std::vector< std::vector<size_t> > GetNeighbours() const;
    /// Return the neighbours of the point \a pI
    std::vector<size_t> GetNeighbours(size_t pI) const;

    /// Return the 3D mesh
    std::shared_ptr<Geometry> GetMesh() const;

private:
    /// Compute the neighbours of the quasi uniform distribution
    void ComputeQuasiUniformNeighbours();
    /// Create the vertexs, faces and normals of an icosahedron (level 0)
    void CreateIcosahedron();
    /// Find if a vertex already exists, if exists return the index
    bool FindSphereCloudVertex(const glm::vec3 &pV, size_t &pPosition) const;
    /// Create a new face with 3 vertex from the point cloud
    void CreateSphereCloudTriangle(const glm::vec3 &pV1, const glm::vec3 &pV2, const glm::vec3 &pV3);
    /// Subdivide the point cloud recursively until \a pDepth
    void Subdivide(glm::vec3 pV1, glm::vec3 pV2, glm::vec3 pV3, unsigned short pDepth);

protected:
    /// Configure the mesh
    void SetMeshInformation();
    /// Mesh to paint the sphere point cloud
    std::shared_ptr<Geometry> mMesh;
    /// Number of points
    size_t mNumberOfPoints;
    /// Faces
    std::vector< size_t > mFaces;
    /// Vertexs in cartesian coordinates
    std::vector< glm::vec3 > mVertices;
    /// Normals of the vertexs
    std::vector< glm::vec3 > mNormals;
    /// Neighbours of the vertexs
    std::vector< std::vector<size_t> > mNeighbours;
};

#endif
