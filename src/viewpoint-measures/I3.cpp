//Definition include
#include "I3.h"

//System includes
#include <float.h>

//Dependency includes
#include "glm/exponential.hpp"

//Project includes
#include "Tools.h"

I3::I3(const QString &pName): Measure(pName, true)
{

}

void I3::Compute(const SceneInformationBuilder *pSceneInformationBuilder)
{
    std::vector< size_t > elementsOutOfDomain;

    const auto projectedAreasMatrix = pSceneInformationBuilder->GetProjectedAreasMatrix();
    size_t numberOfViewpoints = projectedAreasMatrix->GetNumberOfViewpoints();
    size_t numberOfPolygons = projectedAreasMatrix->GetNumberOfPolygons();

    mValues.resize( numberOfViewpoints );
    std::fill(mValues.begin(), mValues.end(), 0.0f);

    //The polygonal I2 is computed first
    std::vector< float > polygonalI2(numberOfPolygons, 0.0f);
    unsigned int sum_a_t = projectedAreasMatrix->GetTotalSum();
    for( size_t currentPolygon = 0; currentPolygon < numberOfPolygons; currentPolygon++ )
    {
        unsigned int sum_a_z = projectedAreasMatrix->GetSumPerPolygon(currentPolygon);
        float sumAux1 = 0.0f;
        float sumAux2 = 0.0f;
        for( size_t currentViewpoint = 0; currentViewpoint < numberOfViewpoints; currentViewpoint++ )
        {
            unsigned int a_t = projectedAreasMatrix->GetSumPerViewpoint(currentViewpoint);
            unsigned int a_z = projectedAreasMatrix->GetValue(currentViewpoint, currentPolygon);
            if( a_z != 0 )
            {
                float aux1 = a_z / (float)sum_a_z;
                sumAux1 += aux1 * glm::log2(aux1);
            }
            float aux2 = a_t / (float)sum_a_t;
            sumAux2 += aux2 * glm::log2(aux2);
        }
        polygonalI2[currentPolygon] = (sumAux1 - sumAux2);
    }

    //Now I3 is computed using polygonal I2
    float minValue = FLT_MAX;
    for( size_t currentViewpoint = 0; currentViewpoint < numberOfViewpoints; currentViewpoint++ )
    {
        unsigned int a_t = projectedAreasMatrix->GetSumPerViewpoint(currentViewpoint);
        if( a_t != 0 )
        {
            for( size_t currentPolygon = 0; currentPolygon < numberOfPolygons; currentPolygon++ )
            {
                unsigned int a_z = projectedAreasMatrix->GetValue(currentViewpoint, currentPolygon);
                if( a_z != 0)
                {
                    float aux = a_z / (float)a_t;
                    mValues[currentViewpoint] += aux * polygonalI2.at(currentPolygon);
                }
            }
            if( mValues.at(currentViewpoint) < minValue )
            {
                minValue = mValues.at(currentViewpoint);
            }
        }
        else
        {
            elementsOutOfDomain.push_back(currentViewpoint);
        }
    }
    //The minimum value is assigned to the viewpoints out of the domain (viewpoints that don't see anything)
    for( size_t currentElement = 0; currentElement < elementsOutOfDomain.size(); currentElement++ )
    {
        mValues[elementsOutOfDomain.at(currentElement)] = minValue;
    }
    mSort = Tools::GetOrderedIndexes(mValues);
    mPositions = Tools::GetOrderedIndexes(mSort);
    mComputed = true;
}
