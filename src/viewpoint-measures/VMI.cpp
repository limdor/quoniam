//Definition include
#include "VMI.h"

//Dependency includes
#include "glm/exponential.hpp"

//Project includes
#include "Tools.h"

#include <algorithm>

VMI::VMI(const std::string &pName): Measure(pName, false)
{

}

void VMI::Compute(const SceneInformationBuilder *pSceneInformationBuilder)
{
    std::vector< size_t > elementsOutOfDomain;

    const auto projectedAreasMatrix = pSceneInformationBuilder->GetProjectedAreasMatrix();
    size_t numberOfViewpoints = projectedAreasMatrix->GetNumberOfViewpoints();
    size_t numberOfPolygons = projectedAreasMatrix->GetNumberOfPolygons();

    mValues.resize( numberOfViewpoints );
    std::fill(mValues.begin(), mValues.end(), 0.0f);

    float maxValue = -FLT_MAX;
    for( size_t currentViewpoint = 0; currentViewpoint < numberOfViewpoints; currentViewpoint++ )
    {
        unsigned int a_t = projectedAreasMatrix->GetSumPerViewpoint(currentViewpoint);
        if( a_t != 0 )
        {
            for( size_t currentPolygon = 0; currentPolygon < numberOfPolygons; currentPolygon++ )
            {
                unsigned int a_z = projectedAreasMatrix->GetValue(currentViewpoint, currentPolygon);
                unsigned int sum_a_z = projectedAreasMatrix->GetSumPerPolygon(currentPolygon);
                if( a_z != 0 )
                {
                    unsigned int sum_a_t = projectedAreasMatrix->GetTotalSum();
                    float aux = a_z / (float)a_t;
                    mValues[currentViewpoint] += a_z * glm::log2( aux * ( sum_a_t / (float) sum_a_z ) );
                }
            }
            mValues[currentViewpoint] /= a_t;
            if( mValues.at(currentViewpoint) > maxValue )
            {
                maxValue = mValues.at(currentViewpoint);
            }
        }
        else
        {
            elementsOutOfDomain.push_back(currentViewpoint);
        }
    }
    //The maximum value is assigned to the viewpoints out of the domain (viewpoints that don't see anything)
    for( size_t currentElement = 0; currentElement < elementsOutOfDomain.size(); currentElement++ )
    {
        mValues[elementsOutOfDomain.at(currentElement)] = maxValue;
    }
    mSort = Tools::GetOrderedIndexes(mValues);
    mPositions = Tools::GetOrderedIndexes(mSort);
    mComputed = true;
}
