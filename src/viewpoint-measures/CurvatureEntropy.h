#ifndef CURVATURE_ENTROPY_H
#define CURVATURE_ENTROPY_H

//Project includes
#include "Measure.h"

/// Class that implements the curvature entropy measure
class CurvatureEntropy : public Measure
{
public:
    explicit CurvatureEntropy(const QString &pName);

    /// Method that computes the measure
    void Compute(const SceneInformationBuilder *pSceneInformationBuilder);
};

#endif
