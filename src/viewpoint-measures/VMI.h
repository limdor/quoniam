/// \file VMI.h
/// \class VMI
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef _VMI_H_
#define _VMI_H_

//Project includes
#include "Measure.h"

/// Class that implements the viewpoint mutual information [Feixas et al. 2009]
class VMI : public Measure
{
public:
    /// Constructor
    explicit VMI(const QString &pName);
    /// Destructor
    ~VMI();

    /// Method that computes the measure
    void Compute(const SceneInformationBuilder *pSceneInformationBuilder);
};

#endif
