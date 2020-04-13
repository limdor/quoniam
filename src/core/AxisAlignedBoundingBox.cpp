#include "AxisAlignedBoundingBox.h"

#include "Geometry.h"

AxisAlignedBoundingBox::AxisAlignedBoundingBox(): Gizmo(),
    mMin(FLT_MAX), mMax(-FLT_MAX)
{
    CreateMesh();
}

AxisAlignedBoundingBox::~AxisAlignedBoundingBox()
{

}

void AxisAlignedBoundingBox::SetMin(const glm::vec3 &pValue)
{
    mMin = pValue;
    UpdatePositions();
}

void AxisAlignedBoundingBox::SetMax(const glm::vec3 &pValue)
{
    mMax = pValue;
    UpdatePositions();
}

glm::vec3 AxisAlignedBoundingBox::GetMin() const
{
    return mMin;
}

glm::vec3 AxisAlignedBoundingBox::GetMax() const
{
    return mMax;
}

std::shared_ptr<AxisAlignedBoundingBox> AxisAlignedBoundingBox::Merge(std::shared_ptr<AxisAlignedBoundingBox const> pAABB0, std::shared_ptr<AxisAlignedBoundingBox const> pAABB1)
{
    glm::vec3 newMin, newMax;

    auto result = std::make_shared<AxisAlignedBoundingBox>();

    if( pAABB0 != nullptr && pAABB1 != nullptr )
    {
        glm::vec3 minAABB0 = pAABB0->GetMin();
        glm::vec3 minAABB1 = pAABB1->GetMin();
        newMin = minAABB1;
        if( minAABB0.x < minAABB1.x )
        {
            newMin.x = minAABB0.x;
        }
        if( minAABB0.y < minAABB1.y )
        {
            newMin.y = minAABB0.y;
        }
        if( minAABB0.z < minAABB1.z )
        {
            newMin.z = minAABB0.z;
        }

        glm::vec3 maxAABB0 = pAABB0->GetMax();
        glm::vec3 maxAABB1 = pAABB1->GetMax();
        newMax = maxAABB1;
        if( maxAABB0.x > maxAABB1.x )
        {
            newMax.x = maxAABB0.x;
        }
        if( maxAABB0.y > maxAABB1.y )
        {
            newMax.y = maxAABB0.y;
        }
        if( maxAABB0.z > maxAABB1.z )
        {
            newMax.z = maxAABB0.z;
        }
    }
    else if( pAABB0 != nullptr )
    {
        newMax = pAABB0->GetMax();
        newMin = pAABB0->GetMin();
    }
    else if( pAABB1 != nullptr )
    {
        newMax = pAABB1->GetMax();
        newMin = pAABB1->GetMin();
    }
    result->SetMax(newMax);
    result->SetMin(newMin);

    return result;
}

void AxisAlignedBoundingBox::CreateMesh()
{
    /// Creation of the mesh
    mGizmo = new Geometry("AxisAlignedBoundingBox", GeometryTopology::Lines);

    /// Set the positions
    mPositionOfVertices.resize(8);
    UpdatePositions();

    /// Set the colors
    QVector<glm::vec4> colors(mPositionOfVertices.size(), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
    mGizmo->SetColorData(colors.size(), colors.data());

    /// Set the indexs
    QVector<unsigned int> indexs(24);
    indexs[0]  = 0; indexs[1]  = 1;
    indexs[2]  = 0; indexs[3]  = 2;
    indexs[4]  = 0; indexs[5]  = 4;
    indexs[6]  = 1; indexs[7]  = 3;
    indexs[8]  = 1; indexs[9]  = 5;
    indexs[10] = 2; indexs[11] = 6;
    indexs[12] = 2; indexs[13] = 3;
    indexs[14] = 4; indexs[15] = 6;
    indexs[16] = 4; indexs[17] = 5;
    indexs[18] = 3; indexs[19] = 7;
    indexs[20] = 5; indexs[21] = 7;
    indexs[22] = 6; indexs[23] = 7;
    mGizmo->SetIndexsData(indexs.size(), indexs.data());
}

void AxisAlignedBoundingBox::UpdatePositions()
{
    mPositionOfVertices[0] = glm::vec4(mMin.x, mMin.y, mMin.z, 1.0f);
    mPositionOfVertices[1] = glm::vec4(mMin.x, mMin.y, mMax.z, 1.0f);
    mPositionOfVertices[2] = glm::vec4(mMin.x, mMax.y, mMin.z, 1.0f);
    mPositionOfVertices[3] = glm::vec4(mMin.x, mMax.y, mMax.z, 1.0f);
    mPositionOfVertices[4] = glm::vec4(mMax.x, mMin.y, mMin.z, 1.0f);
    mPositionOfVertices[5] = glm::vec4(mMax.x, mMin.y, mMax.z, 1.0f);
    mPositionOfVertices[6] = glm::vec4(mMax.x, mMax.y, mMin.z, 1.0f);
    mPositionOfVertices[7] = glm::vec4(mMax.x, mMax.y, mMax.z, 1.0f);

    mGizmo->SetVerticesData(mPositionOfVertices.size(), mPositionOfVertices.data());
}
