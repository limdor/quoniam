#ifndef DEPTH_BASED_VISUAL_STABILITY_H
#define DEPTH_BASED_VISUAL_STABILITY_H

//Project includes
#include "Measure.h"

class DepthBasedVisualStability : public Measure
{
public:
    /// Constructor
    explicit DepthBasedVisualStability(const QString &pName);
    /// Destructor
    ~DepthBasedVisualStability();

    /// Method that computes the measure
    void Compute(const SceneInformationBuilder *pSceneInformationBuilder);
};

#endif
