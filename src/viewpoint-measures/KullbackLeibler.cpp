//Definition include
#include "KullbackLeibler.h"

//Dependency includes
#include "glm/exponential.hpp"

//Project includes
#include "Tools.h"

KullbackLeibler::KullbackLeibler(const QString& pName): Measure(pName, false)
{

}

KullbackLeibler::~KullbackLeibler()
{

}

void KullbackLeibler::Compute(const SceneInformationBuilder *pSceneInformationBuilder)
{
    const auto projectedAreasMatrix = pSceneInformationBuilder->GetProjectedAreasMatrix();
    int numberOfViewpoints = projectedAreasMatrix->GetNumberOfViewpoints();
    int numberOfPolygons = projectedAreasMatrix->GetNumberOfPolygons();
    mValues.fill( 0.0f, numberOfViewpoints );
    QVector< float > serializedPolygonAreas = pSceneInformationBuilder->GetSerializedPolygonAreas();
    float sumAreaPolygons = 0.0f;
    for( int currentPolygon = 0; currentPolygon < numberOfPolygons; currentPolygon++ )
    {
        sumAreaPolygons += serializedPolygonAreas.at(currentPolygon);
    }
    for( int currentViewpoint = 0; currentViewpoint < numberOfViewpoints; currentViewpoint++ )
    {
        unsigned int a_t = projectedAreasMatrix->GetSumPerViewpoint(currentViewpoint);
        for( int currentPolygon = 0; currentPolygon < numberOfPolygons; currentPolygon++ )
        {
            unsigned int a_z = projectedAreasMatrix->GetValue(currentViewpoint, currentPolygon);
            if( a_z != 0 )
            {
                float aux = a_z / (float)a_t;
                mValues[currentViewpoint] += aux * glm::log2( aux / ( serializedPolygonAreas.at(currentPolygon) / sumAreaPolygons ) );
            }
        }
    }
    mSort = Tools::GetOrderedIndexes(mValues);
    mPositions = Tools::GetPositions(mSort);
    mComputed = true;
}
