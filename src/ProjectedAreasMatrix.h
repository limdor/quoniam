#ifndef PROJECTED_AREAS_MATRIX_H
#define PROJECTED_AREAS_MATRIX_H

//Qt includes
#include <QtCore/QVector>

class ProjectedAreasMatrix
{
public:
    ProjectedAreasMatrix(int pNumberOfViewpoints, int pNumberOfPolygons);
    explicit ProjectedAreasMatrix(const ProjectedAreasMatrix *pProjectedAreasMatrix);
    int GetNumberOfViewpoints() const;
    int GetNumberOfPolygons() const;
    unsigned int GetSumPerViewpoint(int pViewpoint) const;
    unsigned int GetSumPerPolygon(int pPolygon) const;
    unsigned int GetTotalSum() const;
    void SetValues(int pViewpoint, const QVector< unsigned int > &pValues);
    unsigned int GetValue(int pViewpoint, int pPolygon) const;
    void Compute();
    void SaveToFile() const;

private:
    QVector< QVector< unsigned int > > mValues;
    int mNumberOfViewpoints;
    int mNumberOfPolygons;
    QVector< unsigned int > mSumPerViewpoint;
    QVector< unsigned int > mSumPerPolygon;
    unsigned int mTotalSum;
};

#endif
