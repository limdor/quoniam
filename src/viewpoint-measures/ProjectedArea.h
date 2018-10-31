/// \file ProjectedArea.h
/// \class ProjectedArea
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef _PROJECTED_AREA_H_
#define _PROJECTED_AREA_H_

//Project includes
#include "Measure.h"

/// Class that implements the projected area measure
class ProjectedArea : public Measure
{
public:
    /// Constructor
    explicit ProjectedArea(const QString &pName);
    /// Destructor
    ~ProjectedArea();

    /// Method that computes the measure
    void Compute(const SceneInformationBuilder *pSceneInformationBuilder);
};

#endif
