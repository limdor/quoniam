/// \file ViewpointEntropy.h
/// \class ViewpointEntropy
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef _VIEWPOINT_ENTROPY_H_
#define _VIEWPOINT_ENTROPY_H_

//Project includes
#include "Measure.h"

/// Class that implements the viewpoint entropy [V�zquez et al. 2002]
class ViewpointEntropy : public Measure
{
public:
    /// Constructor
    explicit ViewpointEntropy(const QString &pName);
    /// Destructor
    ~ViewpointEntropy();

    /// Method that computes the measure
    void Compute(const SceneInformationBuilder *pSceneInformationBuilder);
};

#endif