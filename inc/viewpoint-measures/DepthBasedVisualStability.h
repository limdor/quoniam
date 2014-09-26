/// \file DepthBasedVisualStability.h
/// \class DepthBasedVisualStability
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef _DEPTH_BASED_VISUAL_STABILITY_H_
#define _DEPTH_BASED_VISUAL_STABILITY_H_

//Project includes
#include "Measure.h"

class DepthBasedVisualStability : public Measure
{
public:
    /// Constructor
    DepthBasedVisualStability(const QString &pName);
    /// Destructor
    ~DepthBasedVisualStability();

    /// Method that computes the measure
    void Compute(const SceneInformationBuilder *pSceneInformationBuilder);
};

#endif
