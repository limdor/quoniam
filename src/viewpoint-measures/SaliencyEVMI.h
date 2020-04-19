#ifndef SALIENCY_EVMI_H
#define SALIENCY_EVMI_H

//Project includes
#include "Measure.h"

#include <memory>

/// Class that implements the Saliency-based extended viewpoint mutual information [Feixas et al. 2009]
class SaliencyEVMI : public Measure
{
public:
    explicit SaliencyEVMI(const QString &pName);

    /// Method that computes the measure
    void Compute(const SceneInformationBuilder *pSceneInformationBuilder);

private:
    /// Return the dissimilarity between two polygons
    float GetDissimilarity(std::shared_ptr<ProjectedAreasMatrix const> pProjectedAreasMatrix, size_t pPolygonI, size_t pPolygonJ);
};

#endif
