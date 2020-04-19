//Definition include
#include "CurvatureEntropy.h"

//Dependency includes
#include "glm/exponential.hpp"
#include "glm/gtc/constants.hpp"

//Project includes
#include "Tools.h"

CurvatureEntropy::CurvatureEntropy(const QString& pName): Measure(pName, true)
{

}

void CurvatureEntropy::Compute(const SceneInformationBuilder *pSceneInformationBuilder)
{
    const int histogramSize = 256;
    const size_t numberOfViewpoints = pSceneInformationBuilder->GetProjectedAreasMatrix()->GetNumberOfViewpoints();
    std::vector<float> vertexCurvatures = pSceneInformationBuilder->GetSerializedVertexCurvature();
    mValues.resize( numberOfViewpoints );
    std::fill(mValues.begin(), mValues.end(), 0.0f);
    float min = -2.0f * glm::pi<float>();
    float range = 4.0f * glm::pi<float>();
    for( size_t currentViewpoint = 0; currentViewpoint < numberOfViewpoints; currentViewpoint++ )
    {
        std::vector<unsigned int> curvatureHistogram(histogramSize, 0);
        std::set<int> visibleVertices = pSceneInformationBuilder->GetVisibleVertices(currentViewpoint);
        foreach(int value, visibleVertices)
        {
            float curvature = vertexCurvatures.at(value);
            int bin = (int)( (histogramSize - 1) * ( (curvature - min) / range ) );
            if(bin < 0)
            {
                bin = 0;
            }
            curvatureHistogram[bin]++;
        }
        for( int i = 0; i < histogramSize; i++ )
        {
            unsigned int value = curvatureHistogram.at(i);
            if( value != 0 )
            {
                float aux = value / (float)vertexCurvatures.size();
                mValues[currentViewpoint] += aux * glm::log2(aux);
            }
        }
        mValues[currentViewpoint] = -mValues.at(currentViewpoint);
    }
    mSort = Tools::GetOrderedIndexes(mValues);
    mPositions = Tools::GetOrderedIndexes(mSort);
    mComputed = true;
}
