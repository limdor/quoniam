//Definition include
#include "HeuristicMeasure.h"

//Project includes
#include "Tools.h"

HeuristicMeasure::HeuristicMeasure(const std::string& pName): Measure(pName, true)
{

}

void HeuristicMeasure::Compute(const SceneInformationBuilder *pSceneInformationBuilder)
{
    const auto projectedAreasMatrix = pSceneInformationBuilder->GetProjectedAreasMatrix();
    size_t numberOfViewpoints = projectedAreasMatrix->GetNumberOfViewpoints();
    size_t numberOfPolygons = projectedAreasMatrix->GetNumberOfPolygons();
    mValues.resize( numberOfViewpoints );
    std::fill(mValues.begin(), mValues.end(), 0.0f);
    for( size_t currentViewpoint = 0; currentViewpoint < numberOfViewpoints; currentViewpoint++ )
    {
        unsigned int differentZero = 0;
        for( size_t currentPolygon = 0; currentPolygon < numberOfPolygons; currentPolygon++ )
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
    mPositions = Tools::GetOrderedIndexes(mSort);
    mComputed = true;
}
