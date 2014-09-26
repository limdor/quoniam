/// \file SilhouetteCurvature.h
/// \class SilhouetteCurvature
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef _SILHOUETTE_CURVATURE_H_
#define _SILHOUETTE_CURVATURE_H_

//Project includes
#include "Measure.h"

/// Class that implements the silhouette curvature measure
class SilhouetteCurvature : public Measure
{
public:
    /// Constructor
    SilhouetteCurvature(const QString &pName);
    /// Destructor
    ~SilhouetteCurvature();

    /// Method that computes the measure
    void Compute(const SceneInformationBuilder *pSceneInformationBuilder);
};

#endif
