/// \file GPUGeometry.h
/// \class GPUGeometry
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef _GPU_GEOMETRY_H_
#define _GPU_GEOMETRY_H_

//GLEW has to be included before any OpenGL include
#include "glew.h"

//Project includes
#include "Geometry.h"

/// Class to wrap a Mesh that is stored into the GPU
class GPUGeometry
{
public:
    /// Constructor
    GPUGeometry();
    /// Destructor
    ~GPUGeometry();

    /// Set the vertices of the mesh
    void SetVerticesData(unsigned int pSize, unsigned int pStride, const float *pData);
    /// Set the normals of the mesh
    void SetNormalsData(unsigned int pSize, const float *pData);
    /// Set the colors of the mesh
    void SetColorData(unsigned int pSize, unsigned int pStride, const float *pData);
    /// Set the tangents of the mesh
    void SetTangentData(unsigned int pSize, const float *pData);
    /// Set the bitangents of the mesh
    void SetBitangentData(unsigned int pSize, const float *pData);
    /// Set the texture coordinates of the mesh
    void SetTextCoordsData(unsigned int pSize, const float *pData);
    /// Set the information of connectivities between vertices of the mesh
    void SetIndexsData(unsigned int pSize, Geometry::Topology pTopology, const unsigned int *pData);

    /// Draw the mesh
    void Draw() const;

    /// Configure the vertex array object
    void ConfigureVAO();

private:
    /// We don't want a default copy constructor because it will not work due to it will not duplicate the memory in the gpu
    GPUGeometry(GPUGeometry const&){}

    /// Draw the mesh given the number of elements to be drawn when the vertex array object is already binded
    void FastDraw(unsigned int pSize) const;

    /// Id of the vertex array object
    unsigned int mVaoId;
    /// Id of the vertex buffer object that stores the position of the vertices
    unsigned int mVerticesId;
    /// Stride of the vertices
    unsigned int mVerticesStride;
    /// Id of the vertex buffer object that stores the color of the vertices
    unsigned int mColorsId;
    /// Stride of the colors
    unsigned int mColorsStride;
    /// Id of the vertex buffer object that stores the normal of the vertices
    unsigned int mNormalsId;
    /// Id of the vertex buffer object that stores the tangent of the vertices
    unsigned int mTangentsId;
    /// Id of the vertex buffer object that stores the bitangent of the vertices
    unsigned int mBitangentsId;
    /// Id of the vertex buffer object that stores the texture coordinates of the vertices
    unsigned int mTextCoordsId;
    /// Id of the buffer that stores the information of connectivities between vertices
    unsigned int mIndexsId;
    /// Number of elements of the mesh
    unsigned int mIndexsSize;
    /// Topology of the mesh
    Geometry::Topology mMeshTopology;
};
#endif
