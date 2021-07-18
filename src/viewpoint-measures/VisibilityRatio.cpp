//Definition include
#include "VisibilityRatio.h"

//Project includes
#include "Tools.h"

#include <algorithm>

VisibilityRatio::VisibilityRatio(const std::string& pName): Measure(pName, true)
{

}

void VisibilityRatio::Compute(const SceneInformationBuilder *pSceneInformationBuilder)
{
    const auto projectedAreasMatrix = pSceneInformationBuilder->GetProjectedAreasMatrix();
    size_t numberOfViewpoints = projectedAreasMatrix->GetNumberOfViewpoints();
    size_t numberOfPolygons = projectedAreasMatrix->GetNumberOfPolygons();
    mValues.resize( numberOfViewpoints );
    std::fill(mValues.begin(), mValues.end(), 0.0f);
    std::vector< float > serializedPolygonAreas = pSceneInformationBuilder->GetSerializedPolygonAreas();
    float sumAreaPolygons = 0.0f;
    for( size_t currentPolygon = 0; currentPolygon < numberOfPolygons; currentPolygon++ )
    {
        sumAreaPolygons += serializedPolygonAreas.at(currentPolygon);
    }
    for( size_t currentViewpoint = 0; currentViewpoint < numberOfViewpoints; currentViewpoint++ )
    {
        float sumAreasSeen = 0.0f;
        for( size_t currentPolygon = 0; currentPolygon < numberOfPolygons; currentPolygon++ )
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
    mPositions = Tools::GetOrderedIndexes(mSort);
    mComputed = true;
}
