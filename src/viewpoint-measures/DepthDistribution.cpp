//Definition include
#include "DepthDistribution.h"

//Dependency includes
#include "glm/exponential.hpp"

//Project includes
#include "Tools.h"

DepthDistribution::DepthDistribution(const QString &pName): Measure(pName, true)
{

}

void DepthDistribution::Compute(const SceneInformationBuilder *pSceneInformationBuilder)
{
    const size_t numberOfViewpoints = pSceneInformationBuilder->GetProjectedAreasMatrix()->GetNumberOfViewpoints();
    mValues.resize( numberOfViewpoints );
    std::fill(mValues.begin(), mValues.end(), 0.0f);
    for( size_t currentViewpoint = 0; currentViewpoint < numberOfViewpoints; currentViewpoint++ )
    {
        QVector<float> depthDistribution = pSceneInformationBuilder->GetNormalizedDepthHistogram(currentViewpoint);
        for( int i = 0; i < depthDistribution.size(); i++ )
        {
            mValues[currentViewpoint] += glm::pow(depthDistribution.at(i), 2.0f);
        }
        mValues[currentViewpoint] = 1.0f - mValues.at(currentViewpoint);
    }
    mSort = Tools::GetOrderedIndexes(mValues);
    mPositions = Tools::GetOrderedIndexes(mSort);
    mComputed = true;
}
