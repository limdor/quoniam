//Definition include
#include "MaximumDepth.h"

//Project includes
#include "Tools.h"

MaximumDepth::MaximumDepth(const QString &pName): Measure(pName, true)
{

}

MaximumDepth::~MaximumDepth()
{

}

void MaximumDepth::Compute(const SceneInformationBuilder *pSceneInformationBuilder)
{
    int numberOfViewpoints = pSceneInformationBuilder->GetProjectedAreasMatrix()->GetNumberOfViewpoints();
    mValues.fill( 0.0f, numberOfViewpoints );
    for( int currentViewpoint = 0; currentViewpoint < numberOfViewpoints; currentViewpoint++ )
    {
        mValues[currentViewpoint] = pSceneInformationBuilder->GetMaximumDepth(currentViewpoint);
    }
    mSort = Tools::GetOrderedIndexes(mValues);
    mPositions = Tools::GetPositions(mSort);
    mComputed = true;
}
