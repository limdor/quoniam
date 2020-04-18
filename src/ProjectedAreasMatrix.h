#ifndef PROJECTED_AREAS_MATRIX_H
#define PROJECTED_AREAS_MATRIX_H

#include <vector>

class ProjectedAreasMatrix
{
public:
    ProjectedAreasMatrix(size_t pNumberOfViewpoints, size_t pNumberOfPolygons);
    explicit ProjectedAreasMatrix(const ProjectedAreasMatrix *pProjectedAreasMatrix);
    size_t GetNumberOfViewpoints() const;
    size_t GetNumberOfPolygons() const;
    unsigned int GetSumPerViewpoint(size_t pViewpoint) const;
    unsigned int GetSumPerPolygon(size_t pPolygon) const;
    unsigned int GetTotalSum() const;
    void SetValues(size_t pViewpoint, const std::vector< unsigned int > &pValues);
    unsigned int GetValue(size_t pViewpoint, int pPolygon) const;
    void Compute();
    void SaveToFile() const;

private:
    std::vector< std::vector< unsigned int > > mValues;
    size_t mNumberOfViewpoints;
    size_t mNumberOfPolygons;
    std::vector< unsigned int > mSumPerViewpoint;
    std::vector< unsigned int > mSumPerPolygon;
    unsigned int mTotalSum;
};

#endif
