#ifndef HEURISTIC_MEASURE_H
#define HEURISTIC_MEASURE_H

//Project includes
#include "Measure.h"

/// Class that implements the heuristic measure [Plemenos and Benayada 1996]
class HeuristicMeasure : public Measure
{
public:
    explicit HeuristicMeasure(const std::string &pName);

    /// Method that computes the measure
    void Compute(const SceneInformationBuilder *pSceneInformationBuilder);
};

#endif
