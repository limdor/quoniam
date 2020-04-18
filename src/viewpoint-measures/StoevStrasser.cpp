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
    const size_t numberOfViewpoints = pSceneInformationBuilder->GetProjectedAreasMatrix()->GetNumberOfViewpoints();
    mValues.resize( numberOfViewpoints );
    std::fill(mValues.begin(), mValues.end(), 0.0f);
    const auto projectedAreasMatrix = pSceneInformationBuilder->GetProjectedAreasMatrix();
    unsigned int sum_a_t = projectedAreasMatrix->GetTotalSum();
    for( size_t currentViewpoint = 0; currentViewpoint < numberOfViewpoints; currentViewpoint++ )
    {
        unsigned int a_t = projectedAreasMatrix->GetSumPerViewpoint(currentViewpoint);
        float p_v = a_t / (float)sum_a_t;
        float d_v = pSceneInformationBuilder->GetMaximumDepth(currentViewpoint);
        mValues[currentViewpoint] = ( p_v + d_v + (1.0f - glm::abs(d_v - p_v)) ) / 3.0f;
    }
    mSort = Tools::GetOrderedIndexes(mValues);
    mPositions = Tools::GetOrderedIndexes(mSort);
    mComputed = true;
}
