//Definition include
#include "GPUGeometry.h"

//Qt includes
#include <QGLWidget>

//Project includes
#include "Debug.h"

//STD
#include <type_traits>

GPUGeometry::GPUGeometry():
    mVerticesId(0), mVerticesStride(0),
    mColorsId(0), mColorsStride(0),
    mNormalsId(0), mTangentsId(0), mBitangentsId(0),
    mTextCoordsId(0),
    mIndexsId(0), mIndexsSize(0), mMeshTopology(Geometry::Triangles)
{
    //VAO Generation
    glGenVertexArrays(1, &mVaoId);
}

GPUGeometry::~GPUGeometry()
{
    glDeleteBuffers(1, &mVerticesId);
    glDeleteBuffers(1, &mNormalsId);
    glDeleteBuffers(1, &mColorsId);
    glDeleteBuffers(1, &mTangentsId);
    glDeleteBuffers(1, &mBitangentsId);
    glDeleteBuffers(1, &mTextCoordsId);
    glDeleteBuffers(1, &mIndexsId);

    glDeleteVertexArrays(1, &mVaoId);
}

//Methods for setting the data
void GPUGeometry::SetVerticesData(const std::vector<float>& pVertices, unsigned int pStride)
{
    if(mVerticesId == 0)
    {
        glGenBuffers(1, &mVerticesId);
    }
    mVerticesStride = pStride;
    glBindBuffer(GL_ARRAY_BUFFER, mVerticesId);
    glBufferData(GL_ARRAY_BUFFER, pVertices.size() * sizeof(std::decay_t<decltype(pVertices)>::value_type), pVertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GPUGeometry::SetNormalsData(const std::vector<float>& pNormals)
{
    if(mNormalsId == 0)
    {
        glGenBuffers(1, &mNormalsId);
    }
    glBindBuffer(GL_ARRAY_BUFFER, mNormalsId);
    glBufferData(GL_ARRAY_BUFFER, pNormals.size() * sizeof(std::decay_t<decltype(pNormals)>::value_type), pNormals.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GPUGeometry::SetColorData(const std::vector<float> &pColors, unsigned int pStride)
{
    if(mColorsId == 0)
    {
        glGenBuffers(1, &mColorsId);
    }
    mColorsStride = pStride;
    glBindBuffer(GL_ARRAY_BUFFER, mColorsId);
    glBufferData(GL_ARRAY_BUFFER, pColors.size() * sizeof(std::decay_t<decltype(pColors)>::value_type), pColors.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GPUGeometry::SetTangentData(const std::vector<float>& pTangents)
{
    if(mTangentsId == 0)
    {
        glGenBuffers(1, &mTangentsId);
    }
    glBindBuffer(GL_ARRAY_BUFFER, mTangentsId);
    glBufferData(GL_ARRAY_BUFFER, pTangents.size() * sizeof(std::decay_t<decltype(pTangents)>::value_type), pTangents.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GPUGeometry::SetBitangentData(const std::vector<float>& pBitangents)
{
    if(mBitangentsId == 0)
    {
        glGenBuffers(1, &mBitangentsId);
    }
    glBindBuffer(GL_ARRAY_BUFFER, mBitangentsId);
    glBufferData(GL_ARRAY_BUFFER, pBitangents.size() * sizeof(std::decay_t<decltype(pBitangents)>::value_type), pBitangents.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GPUGeometry::SetTextCoordsData(const std::vector<float>& pTextCoords)
{
    if(mTextCoordsId == 0)
    {
        glGenBuffers(1, &mTextCoordsId);
    }
    glBindBuffer(GL_ARRAY_BUFFER, mTextCoordsId);
    glBufferData(GL_ARRAY_BUFFER, pTextCoords.size() * sizeof(std::decay_t<decltype(pTextCoords)>::value_type), pTextCoords.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GPUGeometry::SetIndexsData(const std::vector<unsigned int>& pIndexs, Geometry::Topology pTopology)
{
    if(mIndexsId == 0)
    {
        glGenBuffers(1, &mIndexsId);
    }
    mIndexsSize = pIndexs.size();
    mMeshTopology = pTopology;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexsId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndexsSize * sizeof(std::decay_t<decltype(pIndexs)>::value_type), pIndexs.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GPUGeometry::FastDraw(unsigned int pSize) const
{
    glDrawElements(mMeshTopology, pSize, GL_UNSIGNED_INT, 0);
}

void GPUGeometry::Draw() const
{
    glBindVertexArray(mVaoId);
    FastDraw(mIndexsSize);
    glBindVertexArray(0);
}

void GPUGeometry::ConfigureVAO()
{
    const unsigned int POSITION  = 0;
    const unsigned int NORMAL    = 1;
    const unsigned int COLOR     = 2;
    const unsigned int TEXTCOORD = 3;
    const unsigned int TANGENT   = 4;
    const unsigned int BITANGENT = 5;

    glBindVertexArray(mVaoId);
    if(mVerticesId != 0)
    {
        glBindBuffer(GL_ARRAY_BUFFER, mVerticesId);
        glVertexAttribPointer(POSITION, mVerticesStride, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(POSITION);
    }
    if(mNormalsId != 0)
    {
        glBindBuffer(GL_ARRAY_BUFFER, mNormalsId);
        glVertexAttribPointer(NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(NORMAL);
    }
    if(mColorsId != 0)
    {
        glBindBuffer(GL_ARRAY_BUFFER, mColorsId);
        glVertexAttribPointer(COLOR, mColorsStride, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(COLOR);
    }
    if(mTextCoordsId != 0)
    {
        glBindBuffer(GL_ARRAY_BUFFER, mTextCoordsId);
        glVertexAttribPointer(TEXTCOORD, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(TEXTCOORD);
    }
    if(mTangentsId != 0)
    {
        glBindBuffer(GL_ARRAY_BUFFER, mTangentsId);
        glVertexAttribPointer(TANGENT, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(TANGENT);
    }
    if(mBitangentsId != 0)
    {
        glBindBuffer(GL_ARRAY_BUFFER, mBitangentsId);
        glVertexAttribPointer(BITANGENT, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(BITANGENT);
    }
    if(mIndexsId != 0)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexsId);
    }
    glBindVertexArray(0);
    CHECK_GL_ERROR();
}
