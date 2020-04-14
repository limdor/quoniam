//Definition include
#include "SilhouetteLength.h"

//Project includes
#include "Tools.h"

SilhouetteLength::SilhouetteLength(const QString &pName): Measure(pName, true)
{

}

void SilhouetteLength::Compute(const SceneInformationBuilder *pSceneInformationBuilder)
{
    int numberOfViewpoints = pSceneInformationBuilder->GetProjectedAreasMatrix()->GetNumberOfViewpoints();
    mValues.fill( 0.0f, numberOfViewpoints );
    for( int currentViewpoint = 0; currentViewpoint < numberOfViewpoints; currentViewpoint++ )
    {
        mValues[currentViewpoint] = pSceneInformationBuilder->GetSilhouetteLength(currentViewpoint);
    }
    mSort = Tools::GetOrderedIndexes(mValues);
    mPositions = Tools::GetPositions(mSort);
    mComputed = true;
}
