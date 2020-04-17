#ifndef GEOMETRY_H
#define GEOMETRY_H

//GLEW has to be included before any OpenGL include
#include "glew.h"

#include "GeometryTopology.h"

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

#include <QtCore/QString>

#include <memory>
#include <vector>

class AxisAlignedBoundingBox;
class BoundingSphere;
class GPUGeometry;

/// Class to wrap the geometry of a 3d mesh that is not stored into the GPU until the GetGPUGeometry method is called.
class Geometry
{
public:
    Geometry(const QString &pName, GeometryTopology pT);
    Geometry() = default;
    Geometry(const Geometry& pGeometry);
    Geometry& operator=(const Geometry& pGeometry) = delete;
    Geometry(Geometry&& pGeometry) = delete;
    Geometry& operator=(Geometry&& pGeometry);
    ~Geometry();

    /// Set the vertices of the geometry
    void SetVerticesData(size_t pSize, const glm::vec4 *pData);
    /// Set the vertices of the geometry
    void SetVerticesData(size_t pSize, const glm::vec3 *pData);
    /// Set the vertices of the geometry
    void SetVerticesData(size_t pSize, const glm::vec2 *pData);
    /// Get the vertices of the geometry
    const std::vector<float>& GetVerticesData() const;
    /// Get vertex stride
    unsigned int GetVertexStride() const;
    /// Set the normals of the geometry
    void SetNormalsData(size_t pSize, const glm::vec3 *pData);
    /// Set the colors of the geometry
    void SetColorData(size_t pSize, const glm::vec4 *pData);
    /// Set the colors of the geometry
    void SetColorData(size_t pSize, const glm::vec3 *pData);
    /// Set the texture coordinates of the geometry
    void SetTextCoordsData(size_t pSize, const glm::vec2 *pData);
    /// Set the tangents of the geometry
    void SetTangentData(size_t pSize, const glm::vec3 *pData);
    /// Set the bitangents of the geometry
    void SetBitangentData(size_t pSize, const glm::vec3 *pData);

    /// Set the information of connectivities between vertices of the geometry
    void SetIndexsData(size_t pSize, const unsigned int *pData);
    /// Get the information of connectivities between vertices of the geometry
    const std::vector<unsigned int>& GetIndexsData() const;

    /// Set the name of the geometry
    void SetName(const QString &pName);
    /// Set the topology
    void SetTopology( GeometryTopology pTopology );

    /// Compute the bounding volumes
    void ComputeBoundingVolumes();
    /// Show information of the geometry like faces, vertices and diameter of the bounding sphere
    void ShowInformation() const;

    std::shared_ptr<AxisAlignedBoundingBox> GetBoundingBox() const;
    std::shared_ptr<BoundingSphere> GetBoundingSphere() const;

    /// Draw the geometry
    void Draw();

    /// Get the number of indexes
    size_t GetNumIndices() const;
    /// Get the number of faces
    size_t GetNumFaces() const;
    /// Get the number of vertices
    size_t GetNumVertices() const;
    /// Get the topology
    GeometryTopology GetTopology() const;

    /// Get the GPUGeometry creating it if it's necessary
    std::shared_ptr<GPUGeometry const> GetGPUGeometry();

private:
    /// Data of the positions of the vertices
    std::vector<float> mVertexData;
    /// Stride of the vertices
    unsigned int mVertexStride{3};
    /// Data of the normals of the vertices
    std::vector<float> mNormalData;
    /// Data of the colors of the vertices
    std::vector<float> mColorData;
    /// Stride of the colors
    unsigned int mColorStride{3};
    /// Data of the texture coordinates of the vertices
    std::vector<float> mTextCoordsData;
    /// Data of the tangents of the vertices
    std::vector<float> mTangentData;
    /// Data of the bitangents of the vertices
    std::vector<float> mBitangentData;

    /// Data of the connectivity between vertices
    std::vector<unsigned int> mIndexData;

    /// Name of the geometry
    QString mName{"Default"};
    /// Topology of the geometry
    GeometryTopology mTopology{GeometryTopology::Triangles};

    /// Axis-aligned bounding box
    std::shared_ptr<AxisAlignedBoundingBox> mBoundingBox;
    /// Bounding sphere
    std::shared_ptr<BoundingSphere> mBoundingSphere;
    /// GPU version of the geometry
    std::shared_ptr<GPUGeometry> mGPUGeometry;
    /// Boolean to know if the GPUGeometry needs to be updated
    bool mNeedGPUGeometryUpdate{true};
};
#endif
