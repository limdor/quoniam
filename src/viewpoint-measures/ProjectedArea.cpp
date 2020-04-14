//Definition include
#include "ProjectedArea.h"

//Project includes
#include "Tools.h"

ProjectedArea::ProjectedArea(const QString &pName): Measure(pName, true)
{

}

void ProjectedArea::Compute(const SceneInformationBuilder *pSceneInformationBuilder)
{
    const auto projectedAreasMatrix = pSceneInformationBuilder->GetProjectedAreasMatrix();
    int numberOfViewpoints = projectedAreasMatrix->GetNumberOfViewpoints();
    mValues.fill( 0.0f, numberOfViewpoints );
    for( int currentViewpoint = 0; currentViewpoint < numberOfViewpoints; currentViewpoint++ )
    {
        unsigned int a_t = projectedAreasMatrix->GetSumPerViewpoint(currentViewpoint);
        mValues[currentViewpoint] = a_t;
    }
    mSort = Tools::GetOrderedIndexes(mValues);
    mPositions = Tools::GetPositions(mSort);
    mComputed = true;
}
