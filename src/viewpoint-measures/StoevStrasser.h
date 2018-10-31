/// \file StoevStrasser.h
/// \class StoevStrasser
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef STOEV_STRASSER_H
#define STOEV_STRASSER_H

//Project includes
#include "Measure.h"

/// Class that implements the Stoev and Strasser measure  [Stoev and Strasser 2002]
class StoevStrasser : public Measure
{
public:
    /// Constructor
    explicit StoevStrasser(const QString &pName);
    /// Destructor
    ~StoevStrasser();

    /// Method that computes the measure
    void Compute(const SceneInformationBuilder *pSceneInformationBuilder);
};

#endif
