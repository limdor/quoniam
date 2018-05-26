/// \file Geometry.h
/// \class Geometry
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

//Project includes
#include "AxisAlignedBoundingBox.h"
#include "BoundingSphere.h"

//Dependency includes
#include "glew.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

//Qt includes
#include <QString>

//STD
#include <vector>

class GPUGeometry;

/// Class to wrap the geometry of a 3d mesh that is not stored into the GPU until the GetGPUGeometry method is called.
class Geometry
{
public:
    /// Enumeration to define the topology of the geometry
    enum Topology
    {
        Points = GL_POINTS,
        Lines = GL_LINES,
        Line_Strip = GL_LINE_STRIP,
        Line_Loop = GL_LINE_LOOP,
        Triangles = GL_TRIANGLES
    };
    /// Constructor
    Geometry(const QString &pName, Topology pT);
    /// Copy constructor (vertex neighbours have to be set again)
    Geometry(const Geometry& pGeometry);
    /// Destructor
    ~Geometry();

    /// Set the vertices of the geometry
    void SetVerticesData(unsigned int pSize, const glm::vec4 *pData);
    /// Set the vertices of the geometry
    void SetVerticesData(unsigned int pSize, const glm::vec3 *pData);
    /// Set the vertices of the geometry
    void SetVerticesData(unsigned int pSize, const glm::vec2 *pData);
    /// Get the vertices of the geometry
    const std::vector<float>& GetVerticesData() const;
    /// Get vertex stride
    unsigned int GetVertexStride() const;
    /// Set the normals of the geometry
    void SetNormalsData(unsigned int pSize, const glm::vec3 *pData);
    /// Set the colors of the geometry
    void SetColorData(unsigned int pSize, const glm::vec4 *pData);
    /// Set the colors of the geometry
    void SetColorData(unsigned int pSize, const glm::vec3 *pData);
    /// Set the texture coordinates of the geometry
    void SetTextCoordsData(unsigned int pSize, const glm::vec2 *pData);
    /// Set the tangents of the geometry
    void SetTangentData(unsigned int pSize, const glm::vec3 *pData);
    /// Set the bitangents of the geometry
    void SetBitangentData(unsigned int pSize, const glm::vec3 *pData);

    /// Set the information of connectivities between vertices of the geometry
    void SetIndexsData(unsigned int pSize, const unsigned int *pData);
    /// Get the information of connectivities between vertices of the geometry
    const std::vector<unsigned int>& GetIndexsData() const;

    /// Set the name of the geometry
    void SetName(const QString &pName);
    /// Set the topology
    void SetTopology( Topology pTopology );

    /// Compute the bounding volumes
    void ComputeBoundingVolumes();
    /// Show information of the geometry like faces, vertices and diameter of the bounding sphere
    void ShowInformation() const;

    /// Get the bounding box
    AxisAlignedBoundingBox* GetBoundingBox() const;
    /// Get the bounding sphere
    const BoundingSphere *GetBoundingSphere() const;

    /// Draw the geometry
    void Draw();

    /// Get the number of indexes
    size_t GetNumIndices() const;
    /// Get the number of faces
    size_t GetNumFaces() const;
    /// Get the number of vertices
    size_t GetNumVertices() const;
    /// Get the topology
    Topology GetTopology() const;

    /// Get the GPUGeometry creating it if it's necessary
    const GPUGeometry* GetGPUGeometry();

private:
    /// Data of the positions of the vertices
    std::vector<float> mVertexData;
    /// Stride of the vertices
    unsigned int mVertexStride;
    /// Data of the normals of the vertices
    std::vector<float> mNormalData;
    /// Data of the colors of the vertices
    std::vector<float> mColorData;
    /// Stride of the colors
    unsigned int mColorStride;
    /// Data of the texture coordinates of the vertices
    std::vector<float> mTextCoordsData;
    /// Data of the tangents of the vertices
    std::vector<float> mTangentData;
    /// Data of the bitangents of the vertices
    std::vector<float> mBitangentData;

    /// Data of the connectivity between vertices
    std::vector<unsigned int> mIndexData;

    /// Name of the geometry
    QString mName;
    /// Topology of the geometry
    Topology mTopology;

    /// Axis-aligned bounding box
    AxisAlignedBoundingBox* mBoundingBox;
    /// Bounding sphere
    BoundingSphere* mBoundingSphere;
    /// GPU version of the geometry
    GPUGeometry* mGPUGeometry;
    /// Boolean to know if the GPUGeometry needs to be updated
    bool mNeedGPUGeometryUpdate;
};
#endif
