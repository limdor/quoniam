#ifndef SILHOUETTE_CURVATURE_H
#define SILHOUETTE_CURVATURE_H

//Project includes
#include "Measure.h"

/// Class that implements the silhouette curvature measure
class SilhouetteCurvature : public Measure
{
public:
    explicit SilhouetteCurvature(const QString &pName);

    /// Method that computes the measure
    void Compute(const SceneInformationBuilder *pSceneInformationBuilder);
};

#endif
