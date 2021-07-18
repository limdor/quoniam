#ifndef DEPTH_DISTRIBUTION_H
#define DEPTH_DISTRIBUTION_H

//Project includes
#include "Measure.h"

/// Class that implements the depth distribution [Secord et al. 2011]
class DepthDistribution : public Measure
{
public:
    explicit DepthDistribution(const std::string &pName);

    /// Method that computes the measure
    void Compute(const SceneInformationBuilder *pSceneInformationBuilder);
};

#endif
