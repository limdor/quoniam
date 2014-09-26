/// \file KullbackLeibler.h
/// \class KullbackLeibler
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef _KULLBACK_LEIBLER_H_
#define _KULLBACK_LEIBLER_H_

//Project includes
#include "Measure.h"

/// Class that implements the kullback leibler distance [Sbert et al. 2005]
class KullbackLeibler : public Measure
{
public:
    /// Constructor
    KullbackLeibler(const QString &pName);
    /// Destructor
    ~KullbackLeibler();

    /// Method that computes the measure
    void Compute(const SceneInformationBuilder *pSceneInformationBuilder);
};

#endif
