#include "Gizmo.h"

#include "GPUGeometry.h"
#include "Geometry.h"
#include "Debug.h"

Gizmo::Gizmo( const Gizmo& pGizmo ):
    mPositionOfVertices(pGizmo.mPositionOfVertices)
{
    if(pGizmo.mGizmo != nullptr)
    {
        mGizmo = new Geometry(*pGizmo.mGizmo);
    }
    else
    {
        mGizmo = nullptr;
    }
}

Gizmo::~Gizmo()
{
    delete mGizmo;
}

void Gizmo::Draw()
{
    if(mGizmo != nullptr)
    {
        mGizmo->Draw();
    }
}

Geometry* Gizmo::GetMesh() const
{
    return mGizmo;
}
