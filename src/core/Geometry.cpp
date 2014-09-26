//Definition include
#include "Geometry.h"

//Dependency includes
#include "glm/vec2.hpp"
#include "glm/exponential.hpp"
#include "Miniball.h"

//Project includes
#include "Debug.h"
#include "GPUGeometry.h"
#include "Scene.h"

Geometry::Geometry(const QString &pName, Topology pT):
    mTopology(pT), mBoundingBox(NULL), mBoundingSphere(NULL), mGPUGeometry(NULL), mName(pName), mNeedGPUGeometryUpdate(false)
{

}

Geometry::Geometry(const Geometry& pGeometry):
    mVertexData(pGeometry.mVertexData), mVertexStride(pGeometry.mVertexStride), mNormalData(pGeometry.mNormalData),
    mColorData(pGeometry.mColorData), mColorStride(pGeometry.mColorStride), mTextCoordsData(pGeometry.mTextCoordsData),
    mTangentData(pGeometry.mTangentData), mBitangentData(pGeometry.mBitangentData), mIndexData(pGeometry.mIndexData),
    mName(pGeometry.mName), mTopology(pGeometry.mTopology), mNeedGPUGeometryUpdate(true)
{
    if(pGeometry.mBoundingBox != NULL)
    {
        mBoundingBox = new AxisAlignedBoundingBox(*pGeometry.mBoundingBox);
    }
    else
    {
        mBoundingBox = NULL;
    }

    if(pGeometry.mBoundingSphere != NULL)
    {
        mBoundingSphere = new BoundingSphere(*pGeometry.mBoundingSphere);
    }
    else
    {
        mBoundingSphere = NULL;
    }
    mGPUGeometry = NULL;
}

Geometry::~Geometry()
{
    delete mBoundingBox;
    delete mBoundingSphere;
    delete mGPUGeometry;
}

void Geometry::SetVerticesData(unsigned int pSize, const glm::vec4 *pData)
{
    mVertexData.resize(pSize*4);
    for( unsigned int i = 0; i < pSize; i++ )
    {
        mVertexData[i*4] = pData[i].x;
        mVertexData[i*4+1] = pData[i].y;
        mVertexData[i*4+2] = pData[i].z;
        mVertexData[i*4+3] = pData[i].w;
    }
    mVertexStride = 4;
    mNeedGPUGeometryUpdate = true;
}

void Geometry::SetVerticesData(unsigned int pSize, const glm::vec3 *pData)
{
    mVertexData.resize(pSize*3);
    for( unsigned int i = 0; i < pSize; i++ )
    {
        mVertexData[i*3] = pData[i].x;
        mVertexData[i*3+1] = pData[i].y;
        mVertexData[i*3+2] = pData[i].z;
    }
    mVertexStride = 3;
    mNeedGPUGeometryUpdate = true;
}

void Geometry::SetVerticesData(unsigned int pSize, const glm::vec2 *pData)
{
    mVertexData.resize(pSize*2);
    for( unsigned int i = 0; i < pSize; i++ )
    {
        mVertexData[i*2] = pData[i].x;
        mVertexData[i*2+1] = pData[i].y;
    }
    mVertexStride = 2;
    mNeedGPUGeometryUpdate = true;
}

QVector<float> Geometry::GetVerticesData() const
{
    return mVertexData;
}

unsigned int Geometry::GetVertexStride() const
{
    return mVertexStride;
}

void Geometry::SetNormalsData(unsigned int pSize, const glm::vec3 *pData)
{
    mNormalData.resize(pSize*3);
    for( unsigned int i = 0; i < pSize; i++ )
    {
        mNormalData[i*3] = pData[i].x;
        mNormalData[i*3+1] = pData[i].y;
        mNormalData[i*3+2] = pData[i].z;
    }
    mNeedGPUGeometryUpdate = true;
}

void Geometry::SetColorData(unsigned int pSize, const glm::vec4 *pData)
{
    mColorData.resize(pSize*4);
    for( unsigned int i = 0; i < pSize; i++ )
    {
        mColorData[i*4] = pData[i].r;
        mColorData[i*4+1] = pData[i].g;
        mColorData[i*4+2] = pData[i].b;
        mColorData[i*4+3] = pData[i].a;
    }
    mColorStride = 4;
    mNeedGPUGeometryUpdate = true;
}

void Geometry::SetColorData(unsigned int pSize, const glm::vec3 *pData)
{
    mColorData.resize(pSize*3);
    for( unsigned int i = 0; i < pSize; i++ )
    {
        mColorData[i*3] = pData[i].x;
        mColorData[i*3+1] = pData[i].y;
        mColorData[i*3+2] = pData[i].z;
    }
    mColorStride = 3;
    mNeedGPUGeometryUpdate = true;
}

void Geometry::SetTextCoordsData(unsigned int pSize, const glm::vec2 *pData)
{
    mTextCoordsData.resize(pSize*2);
    for( unsigned int i = 0; i < pSize; i++ )
    {
        mTextCoordsData[i*2] = pData[i].x;
        mTextCoordsData[i*2+1] = pData[i].y;
    }
    mNeedGPUGeometryUpdate = true;
}

void Geometry::SetTangentData(unsigned int pSize, const glm::vec3 *pData)
{
    mTangentData.resize(pSize*3);
    for( unsigned int i = 0; i < pSize; i++ )
    {
        mTangentData[i*3] = pData[i].x;
        mTangentData[i*3+1] = pData[i].y;
        mTangentData[i*3+2] = pData[i].z;
    }
    mNeedGPUGeometryUpdate = true;
}

void Geometry::SetBitangentData(unsigned int pSize, const glm::vec3 *pData)
{
    mBitangentData.resize(pSize*3);
    for( unsigned int i = 0; i < pSize; i++ )
    {
        mBitangentData[i*3] = pData[i].x;
        mBitangentData[i*3+1] = pData[i].y;
        mBitangentData[i*3+1] = pData[i].z;
    }
    mNeedGPUGeometryUpdate = true;
}

void Geometry::SetIndexsData(unsigned int pSize, const unsigned int *pData)
{
    mIndexData.resize(pSize);
    for( unsigned int i = 0; i < pSize; i++ )
    {
        mIndexData[i] = pData[i];
    }
    mNeedGPUGeometryUpdate = true;
}

QVector<unsigned int> Geometry::GetIndexsData() const
{
    return mIndexData;
}

void Geometry::SetName(const QString &pName)
{
    mName = pName;
}

void Geometry::SetTopology( Topology pTopology )
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
    if(mBoundingBox == NULL)
    {
        mBoundingBox = new AxisAlignedBoundingBox();
    }
    if(mBoundingSphere == NULL)
    {
        mBoundingSphere = new BoundingSphere();
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
        Debug::Warning(QString("Bounding sphere in geometry %1 may be invalid. Accuracy: %2").arg(mName).arg(error));
    }

    mBoundingSphere->SetCenter(sphereCenter);
    mBoundingSphere->SetRadius(glm::sqrt(mb.squared_radius()));
}

void Geometry::ShowInformation() const
{
    Debug::Log(QString("Information of the mesh %1:").arg(mName));
    Debug::Log(QString("   Number of faces: %1").arg(GetNumFaces()));
    Debug::Log(QString("   Number of vertices: %1").arg(GetNumVertices()));
    Debug::Log(QString("   Diameter: %1").arg(mBoundingSphere->GetRadius()*2));
}

AxisAlignedBoundingBox * Geometry::GetBoundingBox() const
{
    return mBoundingBox;
}

const BoundingSphere * Geometry::GetBoundingSphere() const
{
    return mBoundingSphere;
}

void Geometry::Draw()
{
    GetGPUGeometry()->Draw();
}

int Geometry::GetNumIndices() const
{
    return mIndexData.size();
}

int Geometry::GetNumFaces() const
{
    if( mTopology == Triangles )
    {
        return mIndexData.size() / 3;
    }
    else if( mTopology == Lines )
    {
        return mIndexData.size() / 2;
    }
    else if( mTopology == Points )
    {
        return mIndexData.size();
    }
    else if( mTopology == Line_Strip )
    {
        return mIndexData.size() - 1;
    }
    else if( mTopology == Line_Loop )
    {
        return mIndexData.size();
    }
    else
    {
        Debug::Error("Unexpected topology!");
        return 0;
    }
}

int Geometry::GetNumVertices() const
{
    return mVertexData.size() / mVertexStride;
}

Geometry::Topology Geometry::GetTopology() const
{
    return mTopology;
}

const GPUGeometry *Geometry::GetGPUGeometry()
{
    if(mGPUGeometry == NULL || mNeedGPUGeometryUpdate)
    {
        if( mGPUGeometry != NULL )
        {
            delete mGPUGeometry;
        }
        CHECK_GL_ERROR();
        mGPUGeometry = new GPUGeometry();
        mGPUGeometry->SetVerticesData( mVertexData.size(), mVertexStride, mVertexData.data() );
        if(mNormalData.size() > 0)
            mGPUGeometry->SetNormalsData(mNormalData.size(), mNormalData.data() );
        mGPUGeometry->SetIndexsData(mIndexData.size(), mTopology, mIndexData.data() );
        if(mColorData.size() > 0)
            mGPUGeometry->SetColorData(mColorData.size(), mColorStride, mColorData.data() );
        if(mTextCoordsData.size() > 0)
            mGPUGeometry->SetTextCoordsData(mTextCoordsData.size(), mTextCoordsData.data() );
        if(mTangentData.size() > 0)
            mGPUGeometry->SetTangentData(mTangentData.size(), mTangentData.data() );
        if(mBitangentData.size() > 0)
            mGPUGeometry->SetBitangentData(mBitangentData.size(), mBitangentData.data() );
        mGPUGeometry->ConfigureVAO();
        mNeedGPUGeometryUpdate = false;
    }
    return mGPUGeometry;
}
