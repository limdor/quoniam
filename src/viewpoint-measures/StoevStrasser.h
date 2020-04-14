#ifndef STOEV_STRASSER_H
#define STOEV_STRASSER_H

//Project includes
#include "Measure.h"

/// Class that implements the Stoev and Strasser measure  [Stoev and Strasser 2002]
class StoevStrasser : public Measure
{
public:
    explicit StoevStrasser(const QString &pName);

    /// Method that computes the measure
    void Compute(const SceneInformationBuilder *pSceneInformationBuilder);
};

#endif
