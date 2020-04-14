#ifndef DEPTH_BASED_VISUAL_STABILITY_H
#define DEPTH_BASED_VISUAL_STABILITY_H

//Project includes
#include "Measure.h"

class DepthBasedVisualStability : public Measure
{
public:
    explicit DepthBasedVisualStability(const QString &pName);

    /// Method that computes the measure
    void Compute(const SceneInformationBuilder *pSceneInformationBuilder);
};

#endif
