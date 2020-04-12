/// \file GPUGeometry.h
/// \class GPUGeometry
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef GPU_GEOMETRY_H
#define GPU_GEOMETRY_H

//GLEW has to be included before any OpenGL include
#include "glew.h"

#include "GeometryTopology.h"

#include <vector>

/// Class to wrap a Mesh that is stored into the GPU
class GPUGeometry
{
public:
    /// Constructor
    GPUGeometry() = default;
    /// Destructor
    ~GPUGeometry();
    /// Remove copy constructor
    GPUGeometry(GPUGeometry const&) = delete;
    /// Allow move constructor
    GPUGeometry(GPUGeometry&&) = default;

    GPUGeometry& operator =(const GPUGeometry&) = delete;
    GPUGeometry& operator =(GPUGeometry&&) = default;

    /// Set the vertices of the mesh
    void SetVerticesData(const std::vector<float> &pVertices, unsigned int pStride);
    /// Set the normals of the mesh
    void SetNormalsData(const std::vector<float> &pNormals);
    /// Set the colors of the mesh
    void SetColorData(const std::vector<float> &pColors, unsigned int pStride);
    /// Set the tangents of the mesh
    void SetTangentData(const std::vector<float> &pTangents);
    /// Set the bitangents of the mesh
    void SetBitangentData(const std::vector<float> &pBitangents);
    /// Set the texture coordinates of the mesh
    void SetTextCoordsData(const std::vector<float> &pTextCoords);
    /// Set the information of connectivities between vertices of the mesh
    void SetIndexsData(const std::vector<unsigned int> &pIndexs, GeometryTopology pTopology);

    /// Draw the mesh
    void Draw() const;

    /// Configure the vertex array object
    void ConfigureVAO();

private:
    /// We don't want a default copy constructor because it will not work due to it will not duplicate the memory in the gpu


    /// Draw the mesh given the number of elements to be drawn when the vertex array object is already binded
    void FastDraw(unsigned int pSize) const;

    /// Id of the vertex array object
    unsigned int mVaoId{0};
    /// Id of the vertex buffer object that stores the position of the vertices
    unsigned int mVerticesId{0};
    /// Stride of the vertices
    unsigned int mVerticesStride{0};
    /// Id of the vertex buffer object that stores the color of the vertices
    unsigned int mColorsId{0};
    /// Stride of the colors
    unsigned int mColorsStride{0};
    /// Id of the vertex buffer object that stores the normal of the vertices
    unsigned int mNormalsId{0};
    /// Id of the vertex buffer object that stores the tangent of the vertices
    unsigned int mTangentsId{0};
    /// Id of the vertex buffer object that stores the bitangent of the vertices
    unsigned int mBitangentsId{0};
    /// Id of the vertex buffer object that stores the texture coordinates of the vertices
    unsigned int mTextCoordsId{0};
    /// Id of the buffer that stores the information of connectivities between vertices
    unsigned int mIndexsId{0};
    /// Number of elements of the mesh
    size_t mIndexsSize{0};
    /// Topology of the mesh
    GeometryTopology mMeshTopology{GeometryTopology::Triangles};
};
#endif
