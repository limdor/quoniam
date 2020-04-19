//Definition include
#include "ViewpointEntropy.h"

//Dependency includes
#include "glm/exponential.hpp"

//Project includes
#include "Tools.h"

ViewpointEntropy::ViewpointEntropy(const QString& pName): Measure(pName, true)
{

}

void ViewpointEntropy::Compute(const SceneInformationBuilder *pSceneInformationBuilder)
{
    const auto projectedAreasMatrix = pSceneInformationBuilder->GetProjectedAreasMatrix();
    size_t numberOfViewpoints = projectedAreasMatrix->GetNumberOfViewpoints();
    size_t numberOfPolygons = projectedAreasMatrix->GetNumberOfPolygons();
    mValues.resize( numberOfViewpoints );
    std::fill(mValues.begin(), mValues.end(), 0.0f);
    for( size_t currentViewpoint = 0; currentViewpoint < numberOfViewpoints; currentViewpoint++ )
    {
        unsigned int a_t = projectedAreasMatrix->GetSumPerViewpoint(currentViewpoint);
        for( size_t currentPolygon = 0; currentPolygon < numberOfPolygons; currentPolygon++ )
        {
            unsigned int a_z = projectedAreasMatrix->GetValue( currentViewpoint, currentPolygon );
            if( a_z != 0 )
            {
                float aux = a_z / (float)a_t;
                mValues[currentViewpoint] += aux * glm::log2(aux);
            }
        }
        mValues[currentViewpoint] = -mValues.at(currentViewpoint);
    }
    mSort = Tools::GetOrderedIndexes(mValues);
    mPositions = Tools::GetOrderedIndexes(mSort);
    mComputed = true;
}
