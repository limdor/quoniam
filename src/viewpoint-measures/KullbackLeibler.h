#ifndef KULLBACK_LEIBLER_H
#define KULLBACK_LEIBLER_H

//Project includes
#include "Measure.h"

/// Class that implements the kullback leibler distance [Sbert et al. 2005]
class KullbackLeibler : public Measure
{
public:
    explicit KullbackLeibler(const std::string &pName);

    /// Method that computes the measure
    void Compute(const SceneInformationBuilder *pSceneInformationBuilder);
};

#endif
