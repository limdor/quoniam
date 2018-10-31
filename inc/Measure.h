/// \file Measure.h
/// \class Measure
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef _MEASURE_H_
#define _MEASURE_H_

//Qt includes
#include <QString>

//Project includes
#include "SceneInformationBuilder.h"

/// Parent class for all the measures
class Measure
{
public:
    /// Constructor
    Measure(const QString& pName, bool pMaximumBest);
    /// Destructor
    virtual ~Measure() = default;

    /// Get the name of the measure
    const QString& GetName() const;
    /// Get a list of the values of the measures for every viewpoint
    QVector<float> GetValues() const;
    /// Get the value of the measure given a viewpoint
    float GetValue(int pViewpoint) const;
    int GetNth(int pNth) const;
    int GetPosition(int pViewpoint) const;
    bool IsMaximumBest() const;
    /// Get the best viewpoint
    int GetBestViewpoint() const;
    /// Return a boolean to know if the measure is computed
    bool Computed() const;
    void SetComputed(bool pComputed);
    virtual void Compute(const SceneInformationBuilder *pSceneInformationBuilder) = 0;

protected:
    /// Name of the measure
    const QString mName;
    /// Wheter a high value is a good measure or not
    const bool mMaximumBest;

    /// Boolean to know if the measure is computed
    bool mComputed;

    QVector<float> mValues;
    QVector<int> mSort;
    QVector<int> mPositions;
};

#endif
