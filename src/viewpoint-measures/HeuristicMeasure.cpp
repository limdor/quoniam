//Definition include
#include "HeuristicMeasure.h"

//Project includes
#include "Tools.h"

HeuristicMeasure::HeuristicMeasure(const QString& pName): Measure(pName, true)
{

}

HeuristicMeasure::~HeuristicMeasure()
{

}

void HeuristicMeasure::Compute(const SceneInformationBuilder *pSceneInformationBuilder)
{
    const ProjectedAreasMatrix* projectedAreasMatrix = pSceneInformationBuilder->GetProjectedAreasMatrix();
    int numberOfViewpoints = projectedAreasMatrix->GetNumberOfViewpoints();
    int numberOfPolygons = projectedAreasMatrix->GetNumberOfPolygons();
    mValues.fill( 0.0f, numberOfViewpoints );
    for( int currentViewpoint = 0; currentViewpoint < numberOfViewpoints; currentViewpoint++ )
    {
        unsigned int differentZero = 0;
        for( int currentPolygon = 0; currentPolygon < numberOfPolygons; currentPolygon++ )
        {
            unsigned int a_z = projectedAreasMatrix->GetValue(currentViewpoint, currentPolygon);
            if( a_z != 0 )
            {
                differentZero++;
            }
        }
        mValues[currentViewpoint] = ( differentZero / (float)numberOfPolygons ) + ( projectedAreasMatrix->GetSumPerViewpoint(currentViewpoint) / ( pSceneInformationBuilder->GetWidthResolution() * ( pSceneInformationBuilder->GetWidthResolution() / pSceneInformationBuilder->GetAspectRatio() ) ) );
    }
    mSort = Tools::GetOrderedIndexes(mValues);
    mPositions = Tools::GetPositions(mSort);
    mComputed = true;
}
