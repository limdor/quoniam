#include "Measure.h"

Measure::Measure(const QString &pName, bool pMaximumBest):
    mName{pName}, mMaximumBest{pMaximumBest}, mComputed{false}
{

}

const QString& Measure::GetName() const
{
    return mName;
}

QVector<float> Measure::GetValues() const
{
    return mValues;
}

float Measure::GetValue(int pViewpoint) const
{
    return mValues.at(pViewpoint);
}

int Measure::GetNth(int pNth) const
{
    return mSort.at(pNth);
}

int Measure::GetPosition(int pViewpoint) const
{
    return mPositions.at(pViewpoint);
}

bool Measure::IsMaximumBest() const
{
    return mMaximumBest;
}

int Measure::GetBestViewpoint() const
{
    int best;
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
