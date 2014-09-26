/// \file I2.h
/// \class I2
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef _I2_H_
#define _I2_H_

//Project includes
#include "Measure.h"

/// Class that implements the I2 measure [Bonaventura et al. 2011]
class I2 : public Measure
{
public:
    /// Constructor
    I2(const QString &pName);
    /// Destructor
    ~I2();

    /// Method that computes the measure
    void Compute(const SceneInformationBuilder *pSceneInformationBuilder);
};

#endif
