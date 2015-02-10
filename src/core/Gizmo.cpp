//Definition include
#include "Gizmo.h"

//Project includes
#include "GPUGeometry.h"
#include "Geometry.h"

Gizmo::Gizmo():
    mGizmo(NULL), mPositionOfVertices()
{

}

Gizmo::Gizmo( const Gizmo& pGizmo ):
    mPositionOfVertices(pGizmo.mPositionOfVertices)
{
    if(pGizmo.mGizmo != NULL)
    {
        mGizmo = new Geometry(*pGizmo.mGizmo);
    }
    else
    {
        mGizmo = NULL;
    }
}

Gizmo::~Gizmo()
{
    delete mGizmo;
}

void Gizmo::Draw()
{
    if(mGizmo != NULL)
    {
        mGizmo->Draw();
    }
}

Geometry* Gizmo::GetMesh() const
{
    return mGizmo;
}
