#ifndef PROJECTED_AREAS_MATRIX_H
#define PROJECTED_AREAS_MATRIX_H

#include <vector>

class ProjectedAreasMatrix
{
public:
    ProjectedAreasMatrix(int pNumberOfViewpoints, int pNumberOfPolygons);
    explicit ProjectedAreasMatrix(const ProjectedAreasMatrix *pProjectedAreasMatrix);
    int GetNumberOfViewpoints() const;
    int GetNumberOfPolygons() const;
    unsigned int GetSumPerViewpoint(size_t pViewpoint) const;
    unsigned int GetSumPerPolygon(size_t pPolygon) const;
    unsigned int GetTotalSum() const;
    void SetValues(int pViewpoint, const std::vector< unsigned int > &pValues);
    unsigned int GetValue(int pViewpoint, int pPolygon) const;
    void Compute();
    void SaveToFile() const;

private:
    std::vector< std::vector< unsigned int > > mValues;
    int mNumberOfViewpoints;
    int mNumberOfPolygons;
    std::vector< unsigned int > mSumPerViewpoint;
    std::vector< unsigned int > mSumPerPolygon;
    unsigned int mTotalSum;
};

#endif
