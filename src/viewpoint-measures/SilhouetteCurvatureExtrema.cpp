//Definition include
#include "SilhouetteCurvatureExtrema.h"

//Dependency includes
#include "glm/common.hpp"
#include "glm/exponential.hpp"

//Project includes
#include "Tools.h"

SilhouetteCurvatureExtrema::SilhouetteCurvatureExtrema(const std::string &pName): Measure(pName, true)
{

}

void SilhouetteCurvatureExtrema::Compute(const SceneInformationBuilder *pSceneInformationBuilder)
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
            mValues[currentViewpoint] += glm::pow(glm::abs(angle) / 90.0f, 2.0f);
        }
        mValues[currentViewpoint] /= silhouetteSize;
    }
    mSort = Tools::GetOrderedIndexes(mValues);
    mPositions = Tools::GetOrderedIndexes(mSort);
    mComputed = true;
}
