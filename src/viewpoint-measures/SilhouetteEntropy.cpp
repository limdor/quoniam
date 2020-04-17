//Definition include
#include "SilhouetteEntropy.h"

//Dependency includes
#include "glm/exponential.hpp"

//Project includes
#include "Tools.h"

SilhouetteEntropy::SilhouetteEntropy(const QString &pName): Measure(pName, true)
{

}

void SilhouetteEntropy::Compute(const SceneInformationBuilder *pSceneInformationBuilder)
{
    int numberOfViewpoints = pSceneInformationBuilder->GetProjectedAreasMatrix()->GetNumberOfViewpoints();
    mValues.resize( numberOfViewpoints );
    std::fill(mValues.begin(), mValues.end(), 0.0f);
    for( int currentViewpoint = 0; currentViewpoint < numberOfViewpoints; currentViewpoint++ )
    {
        QVector<unsigned int> curvatureHistogram(9, 0);
        QVector<float> silhouetteCurvature = pSceneInformationBuilder->GetSilhouetteCurvature(currentViewpoint);
        int silhouetteSize = silhouetteCurvature.size();
        for( int i = 0; i < silhouetteSize; i++ )
        {
            float angle = silhouetteCurvature.at(i);
            //int bin = (int)(( (angle + 180.0f) / 360.0f ) * 8);
            int bin = (int)( (angle + 180.0f) / 45.0f );
            curvatureHistogram[bin]++;
        }
        for( int i = 0; i < curvatureHistogram.size(); i++ )
        {
            unsigned int value = curvatureHistogram.at(i);
            if( value != 0 )
            {
                float aux = value / (float)silhouetteSize;
                mValues[currentViewpoint] += aux * glm::log2(aux);
            }
        }
        mValues[currentViewpoint] = -mValues.at(currentViewpoint);
    }
    mSort = Tools::GetOrderedIndexes(mValues);
    mPositions = Tools::GetPositions(mSort);
    mComputed = true;
}
