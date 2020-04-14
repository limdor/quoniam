#ifndef SILHOUETTE_ENTROPY_H
#define SILHOUETTE_ENTROPY_H

//Project includes
#include "Measure.h"

/// Class that implements the silhouette entropy measure
class SilhouetteEntropy : public Measure
{
public:
    explicit SilhouetteEntropy(const QString &pName);

    /// Method that computes the measure
    void Compute(const SceneInformationBuilder *pSceneInformationBuilder);
};

#endif
