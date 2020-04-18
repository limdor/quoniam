#include "Measure.h"

Measure::Measure(const QString &pName, bool pMaximumBest):
    mName{pName}, mMaximumBest{pMaximumBest}, mComputed{false}
{

}

const QString& Measure::GetName() const
{
    return mName;
}

std::vector<float> Measure::GetValues() const
{
    return mValues;
}

float Measure::GetValue(size_t pViewpoint) const
{
    return mValues.at(pViewpoint);
}

size_t Measure::GetNth(size_t pNth) const
{
    return mSort.at(pNth);
}

size_t Measure::GetPosition(size_t pViewpoint) const
{
    return mPositions.at(pViewpoint);
}

bool Measure::IsMaximumBest() const
{
    return mMaximumBest;
}

size_t Measure::GetBestViewpoint() const
{
    size_t best;
    if(mMaximumBest)
    {
        best = mSort.at(mSort.size()-1);
    }
    else
    {
        best = mSort.at(0);
    }
    return best;
}

bool Measure::Computed() const
{
    return mComputed;
}

void Measure::SetComputed(bool pComputed)
{
    mComputed = pComputed;
}
