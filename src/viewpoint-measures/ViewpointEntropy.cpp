//Definition include
#include "ViewpointEntropy.h"

//Dependency includes
#include "glm/exponential.hpp"

//Project includes
#include "Tools.h"

ViewpointEntropy::ViewpointEntropy(const QString& pName): Measure(pName, true)
{

}

ViewpointEntropy::~ViewpointEntropy()
{

}

void ViewpointEntropy::Compute(const SceneInformationBuilder *pSceneInformationBuilder)
{
    const auto projectedAreasMatrix = pSceneInformationBuilder->GetProjectedAreasMatrix();
    int numberOfViewpoints = projectedAreasMatrix->GetNumberOfViewpoints();
    int numberOfPolygons = projectedAreasMatrix->GetNumberOfPolygons();
    mValues.fill( 0.0f, numberOfViewpoints );
    for( int currentViewpoint = 0; currentViewpoint < numberOfViewpoints; currentViewpoint++ )
    {
        unsigned int a_t = projectedAreasMatrix->GetSumPerViewpoint(currentViewpoint);
        for( int currentPolygon = 0; currentPolygon < numberOfPolygons; currentPolygon++ )
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
    mPositions = Tools::GetPositions(mSort);
    mComputed = true;
}
