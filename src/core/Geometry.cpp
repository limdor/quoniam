#include "Geometry.h"

#include "AxisAlignedBoundingBox.h"
#include "Debug.h"
#include "GPUGeometry.h"
#include "Scene.h"

#include "Miniball.h"

#include "glm/exponential.hpp"

Geometry::Geometry(const QString &pName, GeometryTopology pT):
    mTopology{pT}, mName{pName}
{

}

Geometry::Geometry(const Geometry& pGeometry):
    mVertexData(pGeometry.mVertexData), mVertexStride(pGeometry.mVertexStride), mNormalData(pGeometry.mNormalData),
    mColorData(pGeometry.mColorData), mColorStride(pGeometry.mColorStride), mTextCoordsData(pGeometry.mTextCoordsData),
    mTangentData(pGeometry.mTangentData), mBitangentData(pGeometry.mBitangentData), mIndexData(pGeometry.mIndexData),
    mName(pGeometry.mName), mTopology(pGeometry.mTopology), mNeedGPUGeometryUpdate(true)
{
    mBoundingBox = nullptr;
    mBoundingSphere = nullptr;
    if(pGeometry.mBoundingBox != nullptr)
    {
        mBoundingBox = std::make_shared<AxisAlignedBoundingBox>(*pGeometry.mBoundingBox);
    }
    if(pGeometry.mBoundingSphere != nullptr)
    {
        mBoundingSphere = std::make_shared<BoundingSphere>(*pGeometry.mBoundingSphere);
    }
    mGPUGeometry = nullptr;
}

Geometry& Geometry::operator=(Geometry&& pGeometry)
{
    mVertexData = std::move(pGeometry.mVertexData);
    mVertexStride = pGeometry.mVertexStride;
    mNormalData = std::move(pGeometry.mNormalData);
    mColorData = std::move(pGeometry.mColorData);
    mColorStride = pGeometry.mColorStride;
    mTextCoordsData = std::move(pGeometry.mTextCoordsData);
    mTangentData = std::move(pGeometry.mTangentData);
    mBitangentData = std::move(pGeometry.mBitangentData);
    mIndexData = std::move(pGeometry.mIndexData);
    mName = pGeometry.mName;
    mTopology = pGeometry.mTopology;
    mNeedGPUGeometryUpdate = true;
    mBoundingBox = nullptr;
    mBoundingSphere = nullptr;
    if(pGeometry.mBoundingBox != nullptr)
    {
        mBoundingBox = std::make_shared<AxisAlignedBoundingBox>(*pGeometry.mBoundingBox);
    }
    if(pGeometry.mBoundingSphere != nullptr)
    {
        mBoundingSphere = std::make_shared<BoundingSphere>(*pGeometry.mBoundingSphere);
    }
    mGPUGeometry = nullptr;

    return *this;
}

Geometry::~Geometry() = default;

void Geometry::SetVerticesData(size_t pSize, const glm::vec4 *pData)
{
    mVertexData.resize(pSize*4);
    for( size_t i = 0; i < pSize; i++ )
    {
        mVertexData[i*4] = pData[i].x;
        mVertexData[i*4+1] = pData[i].y;
        mVertexData[i*4+2] = pData[i].z;
        mVertexData[i*4+3] = pData[i].w;
    }
    mVertexStride = 4;
    mNeedGPUGeometryUpdate = true;
}

void Geometry::SetVerticesData(size_t pSize, const glm::vec3 *pData)
{
    mVertexData.resize(pSize*3);
    for( size_t i = 0; i < pSize; i++ )
    {
        mVertexData[i*3] = pData[i].x;
        mVertexData[i*3+1] = pData[i].y;
        mVertexData[i*3+2] = pData[i].z;
    }
    mVertexStride = 3;
    mNeedGPUGeometryUpdate = true;
}

void Geometry::SetVerticesData(size_t pSize, const glm::vec2 *pData)
{
    mVertexData.resize(pSize*2);
    for( size_t i = 0; i < pSize; i++ )
    {
        mVertexData[i*2] = pData[i].x;
        mVertexData[i*2+1] = pData[i].y;
    }
    mVertexStride = 2;
    mNeedGPUGeometryUpdate = true;
}

const std::vector<float>& Geometry::GetVerticesData() const
{
    return mVertexData;
}

unsigned int Geometry::GetVertexStride() const
{
    return mVertexStride;
}

void Geometry::SetNormalsData(size_t pSize, const glm::vec3 *pData)
{
    mNormalData.resize(pSize*3);
    for( size_t i = 0; i < pSize; i++ )
    {
        mNormalData[i*3] = pData[i].x;
        mNormalData[i*3+1] = pData[i].y;
        mNormalData[i*3+2] = pData[i].z;
    }
    mNeedGPUGeometryUpdate = true;
}

void Geometry::SetColorData(size_t pSize, const glm::vec4 *pData)
{
    mColorData.resize(pSize*4);
    for( size_t i = 0; i < pSize; i++ )
    {
        mColorData[i*4] = pData[i].r;
        mColorData[i*4+1] = pData[i].g;
        mColorData[i*4+2] = pData[i].b;
        mColorData[i*4+3] = pData[i].a;
    }
    mColorStride = 4;
    mNeedGPUGeometryUpdate = true;
}

void Geometry::SetColorData(size_t pSize, const glm::vec3 *pData)
{
    mColorData.resize(pSize*3);
    for( size_t i = 0; i < pSize; i++ )
    {
        mColorData[i*3] = pData[i].x;
        mColorData[i*3+1] = pData[i].y;
        mColorData[i*3+2] = pData[i].z;
    }
    mColorStride = 3;
    mNeedGPUGeometryUpdate = true;
}

void Geometry::SetTextCoordsData(size_t pSize, const glm::vec2 *pData)
{
    mTextCoordsData.resize(pSize*2);
    for( size_t i = 0; i < pSize; i++ )
    {
        mTextCoordsData[i*2] = pData[i].x;
        mTextCoordsData[i*2+1] = pData[i].y;
    }
    mNeedGPUGeometryUpdate = true;
}

void Geometry::SetTangentData(size_t pSize, const glm::vec3 *pData)
{
    mTangentData.resize(pSize*3);
    for( size_t i = 0; i < pSize; i++ )
    {
        mTangentData[i*3] = pData[i].x;
        mTangentData[i*3+1] = pData[i].y;
        mTangentData[i*3+2] = pData[i].z;
    }
    mNeedGPUGeometryUpdate = true;
}

void Geometry::SetBitangentData(size_t pSize, const glm::vec3 *pData)
{
    mBitangentData.resize(pSize*3);
    for( size_t i = 0; i < pSize; i++ )
    {
        mBitangentData[i*3] = pData[i].x;
        mBitangentData[i*3+1] = pData[i].y;
        mBitangentData[i*3+1] = pData[i].z;
    }
    mNeedGPUGeometryUpdate = true;
}

void Geometry::SetIndexsData(size_t pSize, const unsigned int *pData)
{
    mIndexData.resize(pSize);
    for( size_t i = 0; i < pSize; i++ )
    {
        mIndexData[i] = pData[i];
    }
    mNeedGPUGeometryUpdate = true;
}

void Geometry::SetIndexsData(size_t pSize, const size_t *pData)
{
    mIndexData.resize(pSize);
    for( size_t i = 0; i < pSize; i++ )
    {
        mIndexData[i] = static_cast<unsigned int>(pData[i]);
    }
    mNeedGPUGeometryUpdate = true;
}

const std::vector<unsigned int>& Geometry::GetIndexsData() const
{
    return mIndexData;
}

void Geometry::SetName(const QString &pName)
{
    mName = pName;
}

void Geometry::SetTopology( GeometryTopology pTopology )
{
    mTopology = pTopology;
    mNeedGPUGeometryUpdate = true;
}

void Geometry::ComputeBoundingVolumes()
{
    glm::vec3 min(FLT_MAX);
    glm::vec3 max(-FLT_MAX);

    Miniball<3> mb;
    Point<3> p;
    for( int i = 0; i < mVertexData.size(); i += mVertexStride )
    {
        p[0] = mVertexData.at(i);
        p[1] = mVertexData.at(i+1);
        if( mVertexStride > 2 )
        {
            p[2] = mVertexData.at(i+2);
        }
        else
        {
            p[2] = 0.0f;
        }
        mb.check_in(p);

        if(mVertexData.at(i) <  min.x)
        {
            min.x = mVertexData.at(i);
        }
        if(mVertexData.at(i) > max.x)
        {
            max.x = mVertexData.at(i);
        }
        if(mVertexData.at(i+1) <  min.y)
        {
            min.y = mVertexData.at(i+1);
        }
        if(mVertexData.at(i+1) > max.y)
        {
            max.y = mVertexData.at(i+1);
        }
        if( mVertexStride > 2 )
        {
            if(mVertexData.at(i+2) <  min.z)
            {
                min.z = mVertexData.at(i+2);
            }
            if(mVertexData.at(i+2) > max.z)
            {
                max.z = mVertexData.at(i+2);
            }
        }
    }
    mb.build();
    if(mBoundingBox == nullptr)
    {
        mBoundingBox = std::make_shared<AxisAlignedBoundingBox>();
    }
    if(mBoundingSphere == nullptr)
    {
        mBoundingSphere = std::make_shared<BoundingSphere>();
    }
    mBoundingBox->SetMax(max);
    mBoundingBox->SetMin(min);
    glm::vec3 sphereCenter;
    sphereCenter.x = mb.center()[0];
    sphereCenter.y = mb.center()[1];
    sphereCenter.z = mb.center()[2];
    if(!mb.is_valid(1e-6))
    {
        double slack;
        double error = mb.accuracy(slack);
        Debug::Warning("Bounding sphere in geometry " + mName.toStdString() + " may be invalid. Accuracy: " + std::to_string(error));
    }

    mBoundingSphere->SetCenter(sphereCenter);
    mBoundingSphere->SetRadius(glm::sqrt(mb.squared_radius()));
}

void Geometry::ShowInformation() const
{
    Debug::Log("Information of the mesh " + mName.toStdString() + ":");
    Debug::Log("   Number of faces: " + std::to_string(GetNumFaces()));
    Debug::Log("   Number of vertices: " + std::to_string(GetNumVertices()));
    Debug::Log("   Diameter: " + std::to_string(mBoundingSphere->GetRadius()*2));
}

std::shared_ptr<AxisAlignedBoundingBox> Geometry::GetBoundingBox() const
{
    return mBoundingBox;
}

std::shared_ptr<BoundingSphere> Geometry::GetBoundingSphere() const
{
    return mBoundingSphere;
}

void Geometry::Draw()
{
    GetGPUGeometry()->Draw();
}

size_t Geometry::GetNumIndices() const
{
    return mIndexData.size();
}

size_t Geometry::GetNumFaces() const
{
    if( mTopology == GeometryTopology::Triangles )
    {
        return mIndexData.size() / 3;
    }
    else if( mTopology == GeometryTopology::Lines )
    {
        return mIndexData.size() / 2;
    }
    else if( mTopology == GeometryTopology::Points )
    {
        return mIndexData.size();
    }
    else if( mTopology == GeometryTopology::Line_Strip )
    {
        return mIndexData.size() - 1;
    }
    else if( mTopology == GeometryTopology::Line_Loop )
    {
        return mIndexData.size();
    }
    else
    {
        Debug::Error("Unexpected topology!");
        return 0;
    }
}

size_t Geometry::GetNumVertices() const
{
    return mVertexData.size() / mVertexStride;
}

GeometryTopology Geometry::GetTopology() const
{
    return mTopology;
}

std::shared_ptr<GPUGeometry const> Geometry::GetGPUGeometry()
{
    if(mGPUGeometry == nullptr || mNeedGPUGeometryUpdate)
    {
        CHECK_GL_ERROR();
        mGPUGeometry = std::make_shared<GPUGeometry>();
        mGPUGeometry->SetVerticesData( mVertexData, mVertexStride );
        if(mNormalData.size() > 0)
            mGPUGeometry->SetNormalsData(mNormalData);
        mGPUGeometry->SetIndexsData(mIndexData, mTopology);
        if(mColorData.size() > 0)
            mGPUGeometry->SetColorData(mColorData, mColorStride );
        if(mTextCoordsData.size() > 0)
            mGPUGeometry->SetTextCoordsData(mTextCoordsData);
        if(mTangentData.size() > 0)
            mGPUGeometry->SetTangentData(mTangentData);
        if(mBitangentData.size() > 0)
            mGPUGeometry->SetBitangentData(mBitangentData);
        mGPUGeometry->ConfigureVAO();
        mNeedGPUGeometryUpdate = false;
    }
    return mGPUGeometry;
}
