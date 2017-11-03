/// \file I3.h
/// \class I3
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef _I3_H_
#define _I3_H_

//Project includes
#include "Measure.h"

/// Class that implements the I3 measure [Bonaventura et al. 2011]
class I3 : public Measure
{
public:
    /// Constructor
    explicit I3(const QString &pName);
    /// Destructor
    ~I3();

    /// Method that computes the measure
    void Compute(const SceneInformationBuilder *pSceneInformationBuilder);
};

#endif
