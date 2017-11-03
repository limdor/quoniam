/// \file SilhouetteCurvatureExtrema.h
/// \class SilhouetteCurvatureExtrema
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef _SILHOUETTE_CURVATURE_EXTREMA_H_
#define _SILHOUETTE_CURVATURE_EXTREMA_H_

//Project includes
#include "Measure.h"

/// Class that implements the silhouette curvature extrema
class SilhouetteCurvatureExtrema : public Measure
{
public:
    /// Constructor
    explicit SilhouetteCurvatureExtrema(const QString &pName);
    /// Destructor
    ~SilhouetteCurvatureExtrema();

    /// Method that computes the measure
    void Compute(const SceneInformationBuilder *pSceneInformationBuilder);
};

#endif
