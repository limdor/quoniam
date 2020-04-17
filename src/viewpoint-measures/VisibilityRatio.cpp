//Definition include
#include "VisibilityRatio.h"

//Project includes
#include "Tools.h"

#include <algorithm>

VisibilityRatio::VisibilityRatio(const QString& pName): Measure(pName, true)
{

}

void VisibilityRatio::Compute(const SceneInformationBuilder *pSceneInformationBuilder)
{
    const auto projectedAreasMatrix = pSceneInformationBuilder->GetProjectedAreasMatrix();
    int numberOfViewpoints = projectedAreasMatrix->GetNumberOfViewpoints();
    int numberOfPolygons = projectedAreasMatrix->GetNumberOfPolygons();
    mValues.resize( numberOfViewpoints );
    std::fill(mValues.begin(), mValues.end(), 0.0f);
    QVector< float > serializedPolygonAreas = pSceneInformationBuilder->GetSerializedPolygonAreas();
    float sumAreaPolygons = 0.0f;
    for( int currentPolygon = 0; currentPolygon < numberOfPolygons; currentPolygon++ )
    {
        sumAreaPolygons += serializedPolygonAreas.at(currentPolygon);
    }
    for( int currentViewpoint = 0; currentViewpoint < numberOfViewpoints; currentViewpoint++ )
    {
        float sumAreasSeen = 0.0f;
        for( int currentPolygon = 0; currentPolygon < numberOfPolygons; currentPolygon++ )
        {
            unsigned int a_z = projectedAreasMatrix->GetValue(currentViewpoint, currentPolygon);
            if( a_z != 0 )
            {
                sumAreasSeen += serializedPolygonAreas.at(currentPolygon);
            }
        }
        mValues[currentViewpoint] = sumAreasSeen / sumAreaPolygons;
    }
    mSort = Tools::GetOrderedIndexes(mValues);
    mPositions = Tools::GetPositions(mSort);
    mComputed = true;
}
