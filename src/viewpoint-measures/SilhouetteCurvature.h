/// \file SilhouetteCurvature.h
/// \class SilhouetteCurvature
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef SILHOUETTE_CURVATURE_H
#define SILHOUETTE_CURVATURE_H

//Project includes
#include "Measure.h"

/// Class that implements the silhouette curvature measure
class SilhouetteCurvature : public Measure
{
public:
    /// Constructor
    explicit SilhouetteCurvature(const QString &pName);
    /// Destructor
    ~SilhouetteCurvature();

    /// Method that computes the measure
    void Compute(const SceneInformationBuilder *pSceneInformationBuilder);
};

#endif
