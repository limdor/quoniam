#ifndef VISIBILITY_RATIO_H
#define VISIBILITY_RATIO_H

//Project includes
#include "Measure.h"

/// Class that implements the visibility ratio measures
class VisibilityRatio : public Measure
{
public:
    explicit VisibilityRatio(const QString &pName);

    /// Method that computes the measure
    void Compute(const SceneInformationBuilder *pSceneInformationBuilder);
};

#endif
