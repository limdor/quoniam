/// \file SilhouetteEntropy.h
/// \class SilhouetteEntropy
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef _SILHOUETTE_ENTROPY_H_
#define _SILHOUETTE_ENTROPY_H_

//Project includes
#include "Measure.h"

/// Class that implements the silhouette entropy measure
class SilhouetteEntropy : public Measure
{
public:
    /// Constructor
    SilhouetteEntropy(const QString &pName);
    /// Destructor
    ~SilhouetteEntropy();

    /// Method that computes the measure
    void Compute(const SceneInformationBuilder *pSceneInformationBuilder);
};

#endif
