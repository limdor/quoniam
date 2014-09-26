/// \file VisibilityRatio.h
/// \class VisibilityRatio
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef _VISIBILITY_RATIO_H_
#define _VISIBILITY_RATIO_H_

//Project includes
#include "Measure.h"

/// Class that implements the visibility ratio measures
class VisibilityRatio : public Measure
{
public:
    /// Constructor
    VisibilityRatio(const QString &pName);
    /// Destructor
    ~VisibilityRatio();

    /// Method that computes the measure
    void Compute(const SceneInformationBuilder *pSceneInformationBuilder);
};

#endif
