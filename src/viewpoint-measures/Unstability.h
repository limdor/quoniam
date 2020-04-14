#ifndef UNSTABILITY_H
#define UNSTABILITY_H

//Project includes
#include "Measure.h"

#include <memory>

/// Class that implements the unstability measure [Feixas et al. 2009]
class Unstability : public Measure
{
public:
    /// Constructor
    explicit Unstability(const QString &pName);

    /// Method that computes the measure
    void Compute(const SceneInformationBuilder *pSceneInformationBuilder);

protected:
    /// Return the dissimilarity between two viewpoints
    float GetDissimilarity(std::shared_ptr<ProjectedAreasMatrix const> pProjectedAreasMatrix, int pViewpointI, int pViewpointJ);
};

#endif
