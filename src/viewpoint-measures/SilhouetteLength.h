#ifndef SILHOUETTE_LENGTH_H
#define SILHOUETTE_LENGTH_H

//Project includes
#include "Measure.h"

/// Class that implements the silhouette length measure
class SilhouetteLength : public Measure
{
public:
    /// Constructor
    explicit SilhouetteLength(const QString &pName);

    /// Method that computes the measure
    void Compute(const SceneInformationBuilder *pSceneInformationBuilder);
};

#endif
