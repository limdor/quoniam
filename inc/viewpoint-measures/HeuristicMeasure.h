/// \file HeuristicMeasure.h
/// \class HeuristicMeasure
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef _HEURISTIC_MEASURE_H_
#define _HEURISTIC_MEASURE_H_

//Project includes
#include "Measure.h"

/// Class that implements the heuristic measure [Plemenos and Benayada 1996]
class HeuristicMeasure : public Measure
{
public:
    /// Constructor
    HeuristicMeasure(const QString &pName);
    /// Destructor
    ~HeuristicMeasure();

    /// Method that computes the measure
    void Compute(const SceneInformationBuilder *pSceneInformationBuilder);
};

#endif
