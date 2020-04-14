#ifndef SILHOUETTE_CURVATURE_EXTREMA_H
#define SILHOUETTE_CURVATURE_EXTREMA_H

//Project includes
#include "Measure.h"

/// Class that implements the silhouette curvature extrema
class SilhouetteCurvatureExtrema : public Measure
{
public:
    explicit SilhouetteCurvatureExtrema(const QString &pName);

    /// Method that computes the measure
    void Compute(const SceneInformationBuilder *pSceneInformationBuilder);
};

#endif
