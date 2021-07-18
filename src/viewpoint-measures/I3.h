#ifndef I3_H
#define I3_H

//Project includes
#include "Measure.h"

/// Class that implements the I3 measure [Bonaventura et al. 2011]
class I3 : public Measure
{
public:
    explicit I3(const std::string &pName);

    /// Method that computes the measure
    void Compute(const SceneInformationBuilder *pSceneInformationBuilder);
};

#endif
