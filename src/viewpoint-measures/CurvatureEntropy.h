/// \file CurvatureEntropy.h
/// \class CurvatureEntropy
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef CURVATURE_ENTROPY_H
#define CURVATURE_ENTROPY_H

//Project includes
#include "Measure.h"

/// Class that implements the curvature entropy measure
class CurvatureEntropy : public Measure
{
public:
    /// Constructor
    explicit CurvatureEntropy(const QString &pName);
    /// Destructor
    ~CurvatureEntropy();

    /// Method that computes the measure
    void Compute(const SceneInformationBuilder *pSceneInformationBuilder);
};

#endif
