/// \file SilhouetteEntropy.h
/// \class SilhouetteEntropy
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef SILHOUETTE_ENTROPY_H
#define SILHOUETTE_ENTROPY_H

//Project includes
#include "Measure.h"

/// Class that implements the silhouette entropy measure
class SilhouetteEntropy : public Measure
{
public:
    /// Constructor
    explicit SilhouetteEntropy(const QString &pName);
    /// Destructor
    ~SilhouetteEntropy();

    /// Method that computes the measure
    void Compute(const SceneInformationBuilder *pSceneInformationBuilder);
};

#endif