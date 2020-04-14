#ifndef PROJECTED_AREA_H
#define PROJECTED_AREA_H

//Project includes
#include "Measure.h"

/// Class that implements the projected area measure
class ProjectedArea : public Measure
{
public:
    explicit ProjectedArea(const QString &pName);

    /// Method that computes the measure
    void Compute(const SceneInformationBuilder *pSceneInformationBuilder);
};

#endif
