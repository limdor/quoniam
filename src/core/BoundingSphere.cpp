#include "BoundingSphere.h"

#include "Geometry.h"

#include "glm/exponential.hpp"
#include "glm/trigonometric.hpp"
#include "glm/gtx/norm.hpp"

BoundingSphere::BoundingSphere(): Gizmo(),
    mCenter(0.0f), mRadius(0.0f)
{
    CreateMesh();
}

BoundingSphere::~BoundingSphere()
{

}

glm::vec3 BoundingSphere::GetCenter() const
{
    return mCenter;
}

float BoundingSphere::GetRadius() const
{
    return mRadius;
}

void BoundingSphere::SetCenter(const glm::vec3 &pValue)
{
    mCenter = pValue;
    UpdatePositions();
}

void BoundingSphere::SetRadius(float pValue)
{
    mRadius = pValue;
    UpdatePositions();
}

std::shared_ptr<BoundingSphere> BoundingSphere::Merge(std::shared_ptr<BoundingSphere const> pBS0, std::shared_ptr<BoundingSphere const> pBS1)
{
    auto result = std::make_shared<BoundingSphere>();

    if( pBS0 != nullptr && pBS1 != nullptr )
    {
        //Bibliography: 3D Game Engine Design: A Practical Approach to Real-Time Computer Graphics (p.149)
        glm::vec3 C0 = pBS0->GetCenter();
        glm::vec3 C1 = pBS1->GetCenter();
        float r0 = pBS0->GetRadius();
        float r1 = pBS1->GetRadius();
        glm::vec3 centerDiff = C1 - C0;
        float radiusDiff = r1 - r0;
        float radiusDiffSqr = radiusDiff*radiusDiff;
        float Lsqr = glm::length2(centerDiff);
        if( radiusDiffSqr >= Lsqr )
        {
            if( radiusDiff >= 0.0f )
            {
                result->SetCenter(C1);
                result->SetRadius(r1);
            }
            else
            {
                result->SetCenter(C0);
                result->SetRadius(r0);
            }
        }
        else
        {
            float L = glm::sqrt(Lsqr);
            float t = ( L + r1 - r0 ) / ( 2.0f * L );
            result->SetCenter( C0 + t * centerDiff );
            result->SetRadius( ( L + r1 + r0 ) / 2.0f );
        }
    }
    else if( pBS0 != nullptr )
    {
        result->SetCenter(pBS0->GetCenter());
        result->SetRadius(pBS0->GetRadius());
    }
    else if( pBS1 != nullptr )
    {
        result->SetCenter(pBS1->GetCenter());
        result->SetRadius(pBS1->GetRadius());
    }

    return result;
}

void BoundingSphere::CreateMesh()
{
    /// Resolution used to render the bounding sphere
    int sphereResolution = 50;

    /// Creation of the mesh
    mGizmo = Geometry{"BoundingSphere", GeometryTopology::Lines};

    /// Set the positions
    mPositionOfVertices.resize(sphereResolution*3);
    UpdatePositions();

    /// Set the colors
    QVector<glm::vec4> colors(mPositionOfVertices.size(), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
    mGizmo.SetColorData(colors.size(), colors.data());

    /// Set the indexs
    QVector<unsigned int> indexs(sphereResolution*6);
    int offset = 0;
    for( int i = 0; i < 3; i++ )
    {
        for( int j = 0; j < sphereResolution; j++ )
        {
            indexs[(j + offset)*2] = j + offset;
            indexs[(j + offset)*2 + 1] = j + offset + 1;
        }
        indexs[(sphereResolution + offset)*2 - 1] = offset;
        offset += sphereResolution;
    }
    mGizmo.SetIndexsData(indexs.size(), indexs.data());
}

void BoundingSphere::UpdatePositions()
{
    int sphereResolution = mPositionOfVertices.size() / 3;
    int index = 0;
    const float pi2 = 3.1415926535f * 2.0f;
    float step = pi2 / (float)sphereResolution;

    for(float t = 0.0f; t <= pi2; t += step)
    {
        mPositionOfVertices[index] = glm::vec4( mCenter.x + glm::cos(t) * mRadius, mCenter.y, mCenter.z + glm::sin(t) * mRadius, 1.0f );
        index++;
    }

    for(float t = 0.0f; t <= pi2; t += step)
    {
        mPositionOfVertices[index] = glm::vec4( mCenter.x, mCenter.y + glm::cos(t) * mRadius, mCenter.z + glm::sin(t) * mRadius, 1.0f );
        index++;
    }

    for(float t = 0.0f; t <= pi2; t += step)
    {
        mPositionOfVertices[index] = glm::vec4( mCenter.x + glm::cos(t) * mRadius, mCenter.y + glm::sin(t) * mRadius, mCenter.z, 1.0f );
        index++;
    }

    mGizmo.SetVerticesData(mPositionOfVertices.size(), mPositionOfVertices.data());
}
