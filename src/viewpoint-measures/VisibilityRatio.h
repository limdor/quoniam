/// \file VisibilityRatio.h
/// \class VisibilityRatio
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef VISIBILITY_RATIO_H
#define VISIBILITY_RATIO_H

//Project includes
#include "Measure.h"

/// Class that implements the visibility ratio measures
class VisibilityRatio : public Measure
{
public:
    /// Constructor
    explicit VisibilityRatio(const QString &pName);
    /// Destructor
    ~VisibilityRatio();

    /// Method that computes the measure
    void Compute(const SceneInformationBuilder *pSceneInformationBuilder);
};

#endif