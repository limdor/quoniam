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
    mValues.resize( numberOfViewpoints );
    std::fill(mValues.begin(), mValues.end(), 0.0f);
    for( size_t currentViewpoint = 0; currentViewpoint < numberOfViewpoints; currentViewpoint++ )
    {
        unsigned int a_t = projectedAreasMatrix->GetSumPerViewpoint(currentViewpoint);
        mValues[currentViewpoint] = a_t;
    }
    mSort = Tools::GetOrderedIndexes(mValues);
    mPositions = Tools::GetOrderedIndexes(mSort);
    mComputed = true;
}
