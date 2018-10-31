/// \file Unstability.h
/// \class Unstability
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef _UNSTABILITY_H_
#define _UNSTABILITY_H_

//Project includes
#include "Measure.h"

/// Class that implements the unstability measure [Feixas et al. 2009]
class Unstability : public Measure
{
public:
    /// Constructor
    explicit Unstability(const QString &pName);
    /// Destructor
    ~Unstability();

    /// Method that computes the measure
    void Compute(const SceneInformationBuilder *pSceneInformationBuilder);

protected:
    /// Return the dissimilarity between two viewpoints
    float GetDissimilarity(const ProjectedAreasMatrix *pProjectedAreasMatrix, int pViewpointI, int pViewpointJ);
};

#endif
