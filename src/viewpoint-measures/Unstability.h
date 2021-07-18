#ifndef UNSTABILITY_H
#define UNSTABILITY_H

//Project includes
#include "Measure.h"

#include <memory>

/// Class that implements the unstability measure [Feixas et al. 2009]
class Unstability : public Measure
{
public:
    explicit Unstability(const std::string &pName);

    /// Method that computes the measure
    void Compute(const SceneInformationBuilder *pSceneInformationBuilder);

protected:
    /// Return the dissimilarity between two viewpoints
    float GetDissimilarity(std::shared_ptr<ProjectedAreasMatrix const> pProjectedAreasMatrix, size_t pViewpointI, size_t pViewpointJ);
};

#endif
