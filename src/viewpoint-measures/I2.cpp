//Definition include
#include "I2.h"

//System includes
#include <float.h>

//Dependency includes
#include "glm/exponential.hpp"

//Project includes
#include "Tools.h"

I2::I2(const std::string &pName): Measure(pName, false)
{

}

void I2::Compute(const SceneInformationBuilder *pSceneInformationBuilder)
{
    std::vector< size_t > elementsOutOfDomain;

    const auto projectedAreasMatrix = pSceneInformationBuilder->GetProjectedAreasMatrix();
    size_t numberOfViewpoints = projectedAreasMatrix->GetNumberOfViewpoints();
    size_t numberOfPolygons = projectedAreasMatrix->GetNumberOfPolygons();

    mValues.resize( numberOfViewpoints );
    std::fill(mValues.begin(), mValues.end(), 0.0f);

    float maxValue = -FLT_MAX;
    unsigned int sum_a_t = projectedAreasMatrix->GetTotalSum();
    for( size_t currentViewpoint = 0; currentViewpoint < numberOfViewpoints; currentViewpoint++ )
    {
        unsigned int a_t = projectedAreasMatrix->GetSumPerViewpoint(currentViewpoint);
        if( a_t != 0 )
        {
            float sumAux1 = 0.0f;
            float sumAux2 = 0.0f;
            for( size_t currentPolygon = 0; currentPolygon < numberOfPolygons; currentPolygon++ )
            {
                unsigned int sum_a_z = projectedAreasMatrix->GetSumPerPolygon(currentPolygon);
                unsigned int a_z = projectedAreasMatrix->GetValue(currentViewpoint, currentPolygon);
                if( sum_a_z != 0 )
                {
                    float aux1 = sum_a_z / (float)sum_a_t;
                    sumAux1 += aux1 * glm::log2(aux1);
                    if( a_z != 0 )
                    {
                        float aux2 = a_z / (float)a_t;
                        sumAux2 += aux2 * glm::log2(aux2);
                    }
                }
            }
            mValues[currentViewpoint] = - sumAux1 + sumAux2;
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
    for( int currentElement = 0; currentElement < elementsOutOfDomain.size(); currentElement++ )
    {
        mValues[elementsOutOfDomain.at(currentElement)] = maxValue;
    }
    mSort = Tools::GetOrderedIndexes(mValues);
    mPositions = Tools::GetOrderedIndexes(mSort);
    mComputed = true;
}
