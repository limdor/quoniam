/// \file NumberOfVisibleTriangles.h
/// \class NumberOfVisibleTriangles
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef _NUMBER_OF_VISIBLE_TRIANGLES_H_
#define _NUMBER_OF_VISIBLE_TRIANGLES_H_

//Project includes
#include "Measure.h"

/// Class that implements the number of visible triangles measures
class NumberOfVisibleTriangles : public Measure
{
public:
    /// Constructor
    explicit NumberOfVisibleTriangles(const QString &pName);
    /// Destructor
    ~NumberOfVisibleTriangles();

    /// Method that computes the measure
    void Compute(const SceneInformationBuilder *pSceneInformationBuilder);
};

#endif
