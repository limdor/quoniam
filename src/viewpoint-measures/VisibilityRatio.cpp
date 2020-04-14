//Definition include
#include "VisibilityRatio.h"

//Project includes
#include "Tools.h"

VisibilityRatio::VisibilityRatio(const QString& pName): Measure(pName, true)
{

}

VisibilityRatio::~VisibilityRatio()
{

}

void VisibilityRatio::Compute(const SceneInformationBuilder *pSceneInformationBuilder)
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
