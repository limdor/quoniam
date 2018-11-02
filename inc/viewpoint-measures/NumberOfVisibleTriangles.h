/// \file NumberOfVisibleTriangles.h
/// \class NumberOfVisibleTriangles
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef NUMBER_OF_VISIBLE_TRIANGLES_H
#define NUMBER_OF_VISIBLE_TRIANGLES_H

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
