//Definition include
#include "SilhouetteLength.h"

//Project includes
#include "Tools.h"

SilhouetteLength::SilhouetteLength(const QString &pName): Measure(pName, true)
{

}

void SilhouetteLength::Compute(const SceneInformationBuilder *pSceneInformationBuilder)
{
    const size_t numberOfViewpoints = pSceneInformationBuilder->GetProjectedAreasMatrix()->GetNumberOfViewpoints();
    mValues.resize( numberOfViewpoints );
    std::fill(mValues.begin(), mValues.end(), 0.0f);
    for( size_t currentViewpoint = 0; currentViewpoint < numberOfViewpoints; currentViewpoint++ )
    {
        mValues[currentViewpoint] = pSceneInformationBuilder->GetSilhouetteLength(currentViewpoint);
    }
    mSort = Tools::GetOrderedIndexes(mValues);
    mPositions = Tools::GetOrderedIndexes(mSort);
    mComputed = true;
}
