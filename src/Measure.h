#ifndef MEASURE_H
#define MEASURE_H

//Qt includes
#include <QtCore/QString>

//Project includes
#include "SceneInformationBuilder.h"

#include <vector>

/// Parent class for all the measures
class Measure
{
public:
    Measure(const QString& pName, bool pMaximumBest);
    virtual ~Measure() = default;

    /// Get the name of the measure
    const QString& GetName() const;
    /// Get a list of the values of the measures for every viewpoint
    std::vector<float> GetValues() const;
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

    std::vector<float> mValues;
    std::vector<size_t> mSort;
    std::vector<size_t> mPositions;
};

#endif
