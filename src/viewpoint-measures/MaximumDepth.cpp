//Definition include
#include "MaximumDepth.h"

//Project includes
#include "Tools.h"

MaximumDepth::MaximumDepth(const QString &pName): Measure(pName, true)
{

}

void MaximumDepth::Compute(const SceneInformationBuilder *pSceneInformationBuilder)
{
    const size_t numberOfViewpoints = pSceneInformationBuilder->GetProjectedAreasMatrix()->GetNumberOfViewpoints();
    mValues.resize( numberOfViewpoints );
    std::fill(mValues.begin(), mValues.end(), 0.0f);
    for( size_t currentViewpoint = 0; currentViewpoint < numberOfViewpoints; currentViewpoint++ )
    {
        mValues[currentViewpoint] = pSceneInformationBuilder->GetMaximumDepth(currentViewpoint);
    }
    mSort = Tools::GetOrderedIndexes(mValues);
    mPositions = Tools::GetOrderedIndexes(mSort);
    mComputed = true;
}
