#ifndef I2_H
#define I2_H

//Project includes
#include "Measure.h"

/// Class that implements the I2 measure [Bonaventura et al. 2011]
class I2 : public Measure
{
public:
    explicit I2(const QString &pName);

    /// Method that computes the measure
    void Compute(const SceneInformationBuilder *pSceneInformationBuilder);
};

#endif
