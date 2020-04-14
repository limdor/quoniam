#ifndef FEIXAS_SALIENCY_H
#define FEIXAS_SALIENCY_H

//Project includes
#include "Measure.h"

/// Class that implements the saliency of a viewpoint [Feixas et al. 2009]
class FeixasSaliency : public Measure
{
public:
    /// Constructor
    explicit FeixasSaliency(const QString& pName);
    /// Destructor
    ~FeixasSaliency();

    /// Method that computes the measure
    void Compute(const SceneInformationBuilder *pSceneInformationBuilder);

private:
    /// Return the dissimilarity between two polygons
    float GetDissimilarity(const ProjectedAreasMatrix *pProjectedAreasMatrix, int pPolygonI, int pPolygonJ);
};

#endif
