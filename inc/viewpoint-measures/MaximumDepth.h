/// \file MaximumDepth.h
/// \class MaximumDepth
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef _MAXIMUM_DEPTH_H_
#define _MAXIMUM_DEPTH_H_

//Project includes
#include "Measure.h"

/// Class that implements the maximum depth measure
class MaximumDepth : public Measure
{
public:
    /// Constructor
    MaximumDepth(const QString &pName);
    /// Destructor
    ~MaximumDepth();

    /// Method that computes the measure
    void Compute(const SceneInformationBuilder *pSceneInformationBuilder);
};

#endif
