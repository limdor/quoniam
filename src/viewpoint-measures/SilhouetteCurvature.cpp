//Definition include
#include "SilhouetteCurvature.h"

//Dependency includes
#include "glm/common.hpp"

//Project includes
#include "Tools.h"

SilhouetteCurvature::SilhouetteCurvature(const std::string &pName): Measure(pName, true)
{

}

void SilhouetteCurvature::Compute(const SceneInformationBuilder *pSceneInformationBuilder)
{
    const size_t numberOfViewpoints = pSceneInformationBuilder->GetProjectedAreasMatrix()->GetNumberOfViewpoints();
    mValues.resize( numberOfViewpoints );
    std::fill(mValues.begin(), mValues.end(), 0.0f);
    for( size_t currentViewpoint = 0; currentViewpoint < numberOfViewpoints; currentViewpoint++ )
    {
        std::vector<float> silhouetteCurvature = pSceneInformationBuilder->GetSilhouetteCurvature(currentViewpoint);
        size_t silhouetteSize = silhouetteCurvature.size();
        for( size_t i = 0; i < silhouetteSize; i++ )
        {
            float angle = silhouetteCurvature.at(i);
            mValues[currentViewpoint] += glm::abs(angle) / 90.0f;
        }
        mValues[currentViewpoint] /= silhouetteSize;
    }
    mSort = Tools::GetOrderedIndexes(mValues);
    mPositions = Tools::GetOrderedIndexes(mSort);
    mComputed = true;
}
