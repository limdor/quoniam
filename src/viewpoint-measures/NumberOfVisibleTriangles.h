#ifndef NUMBER_OF_VISIBLE_TRIANGLES_H
#define NUMBER_OF_VISIBLE_TRIANGLES_H

//Project includes
#include "Measure.h"

/// Class that implements the number of visible triangles measures
class NumberOfVisibleTriangles : public Measure
{
public:
    explicit NumberOfVisibleTriangles(const QString &pName);

    /// Method that computes the measure
    void Compute(const SceneInformationBuilder *pSceneInformationBuilder);
};

#endif
