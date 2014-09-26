//Definition include
#include "SilhouetteCurvature.h"

//Dependency includes
#include "glm/common.hpp"

//Project includes
#include "Tools.h"

SilhouetteCurvature::SilhouetteCurvature(const QString &pName): Measure(pName, true)
{

}

SilhouetteCurvature::~SilhouetteCurvature()
{

}

void SilhouetteCurvature::Compute(const SceneInformationBuilder *pSceneInformationBuilder)
{
    int numberOfViewpoints = pSceneInformationBuilder->GetProjectedAreasMatrix()->GetNumberOfViewpoints();
    mValues.fill( 0.0f, numberOfViewpoints );
    for( int currentViewpoint = 0; currentViewpoint < numberOfViewpoints; currentViewpoint++ )
    {
        QVector<float> silhouetteCurvature = pSceneInformationBuilder->GetSilhouetteCurvature(currentViewpoint);
        int silhouetteSize = silhouetteCurvature.size();
        for( int i = 0; i < silhouetteSize; i++ )
        {
            float angle = silhouetteCurvature.at(i);
            mValues[currentViewpoint] += glm::abs(angle) / 90.0f;
        }
        mValues[currentViewpoint] /= silhouetteSize;
    }
    mSort = Tools::GetOrderedIndexes(mValues);
    mPositions = Tools::GetPositions(mSort);
    mComputed = true;
}
