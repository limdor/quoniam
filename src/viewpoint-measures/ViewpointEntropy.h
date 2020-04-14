#ifndef _VIEWPOINT_ENTROPY_H_
#define _VIEWPOINT_ENTROPY_H_

//Project includes
#include "Measure.h"

/// Class that implements the viewpoint entropy [Vazquez et al. 2002]
class ViewpointEntropy : public Measure
{
public:
    /// Constructor
    explicit ViewpointEntropy(const QString &pName);
    /// Destructor
    ~ViewpointEntropy();

    /// Method that computes the measure
    void Compute(const SceneInformationBuilder *pSceneInformationBuilder);
};

#endif
