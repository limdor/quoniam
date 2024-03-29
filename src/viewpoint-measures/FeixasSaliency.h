#ifndef FEIXAS_SALIENCY_H
#define FEIXAS_SALIENCY_H

//Project includes
#include "Measure.h"

#include <memory>

/// Class that implements the saliency of a viewpoint [Feixas et al. 2009]
class FeixasSaliency : public Measure
{
public:
    explicit FeixasSaliency(const std::string& pName);

    /// Method that computes the measure
    void Compute(const SceneInformationBuilder *pSceneInformationBuilder);

private:
    /// Return the dissimilarity between two polygons
    float GetDissimilarity(std::shared_ptr<ProjectedAreasMatrix const> pProjectedAreasMatrix, size_t pPolygonI, size_t pPolygonJ);
};

#endif
