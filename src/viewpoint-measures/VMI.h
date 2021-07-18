#ifndef VMI_H
#define VMI_H

//Project includes
#include "Measure.h"

/// Class that implements the viewpoint mutual information [Feixas et al. 2009]
class VMI : public Measure
{
public:
    explicit VMI(const std::string &pName);

    /// Method that computes the measure
    void Compute(const SceneInformationBuilder *pSceneInformationBuilder);
};

#endif
