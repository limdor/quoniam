#include "ProjectedAreasMatrix.h"

#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include "Debug.h"

ProjectedAreasMatrix::ProjectedAreasMatrix(int pNumberOfViewpoints, int pNumberOfPolygons):
    mNumberOfViewpoints(pNumberOfViewpoints), mNumberOfPolygons(pNumberOfPolygons),
    mTotalSum(0)
{
    mValues.resize(pNumberOfViewpoints);
    for( int currentViewpoint = 0; currentViewpoint < pNumberOfViewpoints; currentViewpoint++ )
    {
        mValues[currentViewpoint].resize(pNumberOfPolygons);
        std::fill(mValues[currentViewpoint].begin(), mValues[currentViewpoint].end(), 0);
    }
    mSumPerPolygon.resize( mNumberOfPolygons );
    std::fill(mSumPerPolygon.begin(), mSumPerPolygon.end(), 0);
    mSumPerViewpoint.resize( mNumberOfViewpoints );
    std::fill(mSumPerViewpoint.begin(), mSumPerViewpoint.end(), 0);
}

ProjectedAreasMatrix::ProjectedAreasMatrix(const ProjectedAreasMatrix *pProjectedAreasMatrix):
    mValues(pProjectedAreasMatrix->mValues),
    mNumberOfViewpoints(pProjectedAreasMatrix->mNumberOfViewpoints), mNumberOfPolygons(pProjectedAreasMatrix->mNumberOfPolygons),
    mSumPerViewpoint(pProjectedAreasMatrix->mSumPerViewpoint), mSumPerPolygon(pProjectedAreasMatrix->mSumPerPolygon),
    mTotalSum(pProjectedAreasMatrix->mTotalSum)
{

}

int ProjectedAreasMatrix::GetNumberOfViewpoints() const
{
    return mNumberOfViewpoints;
}

int ProjectedAreasMatrix::GetNumberOfPolygons() const
{
    return mNumberOfPolygons;
}

unsigned int ProjectedAreasMatrix::GetSumPerViewpoint(size_t pViewpoint) const
{
    return mSumPerViewpoint.at(pViewpoint);
}

unsigned int ProjectedAreasMatrix::GetSumPerPolygon(size_t pPolygon) const
{
    return mSumPerPolygon.at(pPolygon);
}

unsigned int ProjectedAreasMatrix::GetTotalSum() const
{
    return mTotalSum;
}

void ProjectedAreasMatrix::SetValues(int pViewpoint, const std::vector< unsigned int > &pValues)
{
    mValues[pViewpoint] = pValues;
}

unsigned int ProjectedAreasMatrix::GetValue(int pViewpoint, int pPolygon) const
{
    return mValues.at(pViewpoint).at(pPolygon);
}

void ProjectedAreasMatrix::Compute()
{
    mSumPerPolygon.resize( mNumberOfPolygons );
    std::fill(mSumPerPolygon.begin(), mSumPerPolygon.end(), 0);
    mSumPerViewpoint.resize( mNumberOfViewpoints );
    std::fill(mSumPerViewpoint.begin(), mSumPerViewpoint.end(), 0);
    mTotalSum = 0;
    for( int currentViewpoint = 0; currentViewpoint < mNumberOfViewpoints; currentViewpoint++ )
    {
        for( int currentPolygon = 0; currentPolygon < mNumberOfPolygons; currentPolygon++ )
        {
            unsigned int value = mValues.at(currentViewpoint).at(currentPolygon);
            mTotalSum += value;
            mSumPerPolygon[currentPolygon] += value;
            mSumPerViewpoint[currentViewpoint] += value;
        }
    }
}

void ProjectedAreasMatrix::SaveToFile() const
{
    QFile file("InformationChannelHistogram.txt");
    if( file.open(QFile::WriteOnly) )
    {
        QTextStream out(&file);
        for( int currentViewpoint = 0; currentViewpoint < mNumberOfViewpoints; currentViewpoint++ )
        {
            for( int currentPolygon = 0; currentPolygon < mNumberOfPolygons; currentPolygon++ )
            {
                out << mValues.at(currentViewpoint).at(currentPolygon) << " ";
            }
            out << "\n";
        }
        file.close();
        Debug::Log( QString("Informacio escrita al fitxer: InformationChannelHistogram.txt") );
    }
    else
    {
        Debug::Error( QString("Impossible escriure a fitxer: InformationChannelHistogram.txt") );
    }
}
