#ifndef MAXIMUM_DEPTH_H
#define MAXIMUM_DEPTH_H

//Project includes
#include "Measure.h"

/// Class that implements the maximum depth measure
class MaximumDepth : public Measure
{
public:
    explicit MaximumDepth(const std::string &pName);

    /// Method that computes the measure
    void Compute(const SceneInformationBuilder *pSceneInformationBuilder);
};

#endif
