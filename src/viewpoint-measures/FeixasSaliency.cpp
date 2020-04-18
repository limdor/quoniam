//Definition include
#include "FeixasSaliency.h"

//Dependency includes
#include "glm/exponential.hpp"

//Project includes
#include "Debug.h"
#include "Tools.h"

FeixasSaliency::FeixasSaliency(const QString &pName): Measure(pName, true)
{

}

void FeixasSaliency::Compute(const SceneInformationBuilder *pSceneInformationBuilder)
{
    const auto projectedAreasMatrix = pSceneInformationBuilder->GetProjectedAreasMatrix();
    const size_t numberOfViewpoints = projectedAreasMatrix->GetNumberOfViewpoints();
    const size_t numberOfPolygons = projectedAreasMatrix->GetNumberOfPolygons();
    std::vector< float > polygonalSaliency(numberOfPolygons, 0.0f);
    std::vector< std::vector< size_t > > serializedPolygonNeighbours = pSceneInformationBuilder->GetSerializedPolygonNeighbours();
    std::vector< int > polygonsOutOfDomain;
    float minValue = FLT_MAX;
    for( size_t currentPolygon = 0; currentPolygon < numberOfPolygons; currentPolygon++ )
    {
        unsigned int numberOfNeighbours = 0;
        bool ocludedNeighbours = false;

        std::vector< size_t > neighbours = serializedPolygonNeighbours.at(currentPolygon);
        if(neighbours.size() == 0)
        {
            Debug::Log("FeixasSaliency::No neighbours");
        }
        for( size_t currentNeighbour = 0; currentNeighbour < neighbours.size(); currentNeighbour++ )
        {
            unsigned int sum_a_z_i = projectedAreasMatrix->GetSumPerPolygon(currentPolygon);
            unsigned int sum_a_z_j = projectedAreasMatrix->GetSumPerPolygon(neighbours.at(currentNeighbour));

            if( sum_a_z_i != 0 && sum_a_z_j != 0 )
            {
                polygonalSaliency[currentPolygon] += GetDissimilarity( projectedAreasMatrix, currentPolygon, neighbours.at(currentNeighbour) );
                numberOfNeighbours++;
            }
            else
            {
                ocludedNeighbours = true;
            }
        }
        if(numberOfNeighbours != 0)
        {
            polygonalSaliency[currentPolygon] /= numberOfNeighbours;
            if( polygonalSaliency.at(currentPolygon) < minValue )
            {
                minValue = polygonalSaliency.at(currentPolygon);
            }
        }
        else if(ocludedNeighbours)
        {
            polygonsOutOfDomain.push_back(currentPolygon);
        }
    }
    //We assign the minimum value to the polygons that their saliency can not be computed
    for( int currentPolygon = 0; currentPolygon < polygonsOutOfDomain.size(); currentPolygon++ )
    {
        polygonalSaliency[polygonsOutOfDomain.at(currentPolygon)] = minValue;
    }
    mValues.resize( numberOfViewpoints );
    std::fill(mValues.begin(), mValues.end(), 0.0f);
    for( size_t currentViewpoint = 0; currentViewpoint < numberOfViewpoints; currentViewpoint++ )
    {
        for( size_t currentPolygon = 0; currentPolygon < numberOfPolygons; currentPolygon++ )
        {
            unsigned int a_z = projectedAreasMatrix->GetValue(currentViewpoint, currentPolygon);
            unsigned int sum_a_z = projectedAreasMatrix->GetSumPerPolygon(currentPolygon);
            if( a_z != 0 )
            {
                float aux = a_z / (float)sum_a_z;
                mValues[currentViewpoint] += aux * polygonalSaliency.at(currentPolygon);
            }
        }
    }
    mSort = Tools::GetOrderedIndexes(mValues);
    mPositions = Tools::GetOrderedIndexes(mSort);
    mComputed = true;
}

float FeixasSaliency::GetDissimilarity(std::shared_ptr<ProjectedAreasMatrix const> pProjectedAreasMatrix, int pPolygonI, int pPolygonJ)
{
    const size_t numberOfViewpoints = pProjectedAreasMatrix->GetNumberOfViewpoints();
    unsigned int sum_a_z_i = pProjectedAreasMatrix->GetSumPerPolygon(pPolygonI);
    unsigned int sum_a_z_j = pProjectedAreasMatrix->GetSumPerPolygon(pPolygonJ);
    unsigned int sum_a_z_ij = sum_a_z_i + sum_a_z_j;
    float dissimilarity = 0.0f;
    if( sum_a_z_ij != 0 )
    {
        for( size_t currentViewpoint = 0; currentViewpoint < numberOfViewpoints; currentViewpoint++ )
        {
            unsigned int a_z_i = pProjectedAreasMatrix->GetValue(currentViewpoint, pPolygonI);
            unsigned int a_z_j = pProjectedAreasMatrix->GetValue(currentViewpoint, pPolygonJ);
            unsigned int a_z_ij = a_z_i + a_z_j;
            if( a_z_ij != 0 )
            {
                dissimilarity -= a_z_ij * glm::log2( a_z_ij / (float)sum_a_z_ij );
            }
            if( a_z_i != 0 )
            {
                dissimilarity += a_z_i * glm::log2( a_z_i / (float)sum_a_z_i );
            }
            if( a_z_j != 0 )
            {
                dissimilarity += a_z_j * glm::log2( a_z_j / (float)sum_a_z_j );
            }
        }
        dissimilarity /= sum_a_z_ij;
    }
    //We force the range of the variable due to precision problems
    if( dissimilarity > 1.0f )
    {
        dissimilarity = 1.0f;
    }
    else if( dissimilarity < 0.0f )
    {
        dissimilarity = 0.0f;
    }
    return dissimilarity;
}
