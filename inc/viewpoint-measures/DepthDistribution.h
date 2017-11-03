/// \file DepthDistribution.h
/// \class DepthDistribution
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef _DEPTH_DISTRIBUTION_H_
#define _DEPTH_DISTRIBUTION_H_

//Project includes
#include "Measure.h"

/// Class that implements the depth distribution [Secord et al. 2011]
class DepthDistribution : public Measure
{
public:
    /// Constructor
    explicit DepthDistribution(const QString &pName);
    /// Destructor
    ~DepthDistribution();

    /// Method that computes the measure
    void Compute(const SceneInformationBuilder *pSceneInformationBuilder);
};

#endif
