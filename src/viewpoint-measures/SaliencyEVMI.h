/// \file SaliencyEVMI.h
/// \class SaliencyEVMI
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef SALIENCY_EVMI_H
#define SALIENCY_EVMI_H

//Project includes
#include "Measure.h"

/// Class that implements the Saliency-based extended viewpoint mutual information [Feixas et al. 2009]
class SaliencyEVMI : public Measure
{
public:
    /// Constructor
    explicit SaliencyEVMI(const QString &pName);
    /// Destructor
    ~SaliencyEVMI();

    /// Method that computes the measure
    void Compute(const SceneInformationBuilder *pSceneInformationBuilder);

private:
    /// Return the dissimilarity between two polygons
    float GetDissimilarity(const ProjectedAreasMatrix *pProjectedAreasMatrix, int pPolygonI, int pPolygonJ);
};

#endif