//Definition include
#include "NumberOfVisibleTriangles.h"

//Project includes
#include "Tools.h"

NumberOfVisibleTriangles::NumberOfVisibleTriangles(const QString& pName): Measure(pName, true)
{

}

void NumberOfVisibleTriangles::Compute(const SceneInformationBuilder *pSceneInformationBuilder)
{
    const auto projectedAreasMatrix = pSceneInformationBuilder->GetProjectedAreasMatrix();
    int numberOfViewpoints = projectedAreasMatrix->GetNumberOfViewpoints();
    int numberOfPolygons = projectedAreasMatrix->GetNumberOfPolygons();
    mValues.resize( numberOfViewpoints );
    std::fill(mValues.begin(), mValues.end(), 0.0f);
    for( int currentViewpoint = 0; currentViewpoint < numberOfViewpoints; currentViewpoint++ )
    {
        int polygonsSeen = 0;
        for( int currentPolygon = 0; currentPolygon < numberOfPolygons; currentPolygon++ )
        {
            unsigned int a_z = projectedAreasMatrix->GetValue(currentViewpoint, currentPolygon);
            if( a_z != 0 )
            {
                polygonsSeen++;
            }
        }
        mValues[currentViewpoint] = polygonsSeen;
    }
    mSort = Tools::GetOrderedIndexes(mValues);
    mPositions = Tools::GetPositions(mSort);
    mComputed = true;
}
