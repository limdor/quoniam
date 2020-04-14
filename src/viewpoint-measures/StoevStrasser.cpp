//Definition include
#include "StoevStrasser.h"

//Dependency includes
#include "glm/common.hpp"

//Project includes
#include "Tools.h"

StoevStrasser::StoevStrasser(const QString &pName): Measure(pName, true)
{

}

void StoevStrasser::Compute(const SceneInformationBuilder *pSceneInformationBuilder)
{
    int numberOfViewpoints = pSceneInformationBuilder->GetProjectedAreasMatrix()->GetNumberOfViewpoints();
    mValues.fill( 0.0f, numberOfViewpoints );
    const auto projectedAreasMatrix = pSceneInformationBuilder->GetProjectedAreasMatrix();
    unsigned int sum_a_t = projectedAreasMatrix->GetTotalSum();
    for( int currentViewpoint = 0; currentViewpoint < numberOfViewpoints; currentViewpoint++ )
    {
        unsigned int a_t = projectedAreasMatrix->GetSumPerViewpoint(currentViewpoint);
        float p_v = a_t / (float)sum_a_t;
        float d_v = pSceneInformationBuilder->GetMaximumDepth(currentViewpoint);
        mValues[currentViewpoint] = ( p_v + d_v + (1.0f - glm::abs(d_v - p_v)) ) / 3.0f;
    }
    mSort = Tools::GetOrderedIndexes(mValues);
    mPositions = Tools::GetPositions(mSort);
    mComputed = true;
}
