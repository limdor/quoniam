/// \file SilhouetteLength.h
/// \class SilhouetteLength
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

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
    /// Destructor
    ~SilhouetteLength();

    /// Method that computes the measure
    void Compute(const SceneInformationBuilder *pSceneInformationBuilder);
};

#endif