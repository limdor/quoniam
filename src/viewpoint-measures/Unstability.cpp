//Definition include
#include "Unstability.h"

//Dependency includes
#include "glm/exponential.hpp"

//Project includes
#include "Debug.h"
#include "Tools.h"

Unstability::Unstability(const QString &pName): Measure(pName, false)
{

}

void Unstability::Compute(const SceneInformationBuilder *pSceneInformationBuilder)
{
    const auto projectedAreasMatrix = pSceneInformationBuilder->GetProjectedAreasMatrix();
    size_t numberOfViewpoints = projectedAreasMatrix->GetNumberOfViewpoints();
    mValues.resize( numberOfViewpoints );
    std::fill(mValues.begin(), mValues.end(), 0.0f);
    std::vector< std::vector< size_t > > viewpointNeighbours = pSceneInformationBuilder->GetViewpointNeighbours();
    std::vector< size_t > viewpointsOutOfDomain;
    float minValue = FLT_MAX;
    for( size_t currentViewpoint = 0; currentViewpoint < numberOfViewpoints; currentViewpoint++ )
    {
        unsigned int numberOfNeighbours = 0;
        bool ocludedNeighbours = false;

        std::vector< size_t > neighbours = viewpointNeighbours.at(currentViewpoint);
        if(neighbours.size() == 0)
        {
            Debug::Log("Unstability::No neighbours");
        }
        for( size_t currentNeighbour = 0; currentNeighbour < neighbours.size(); currentNeighbour++ )
        {
            unsigned int a_t_i = projectedAreasMatrix->GetSumPerViewpoint(currentViewpoint);
            unsigned int a_t_j = projectedAreasMatrix->GetSumPerViewpoint(neighbours.at(currentNeighbour));

            if( a_t_i != 0 && a_t_j != 0 )
            {
                mValues[currentViewpoint] += GetDissimilarity( projectedAreasMatrix, currentViewpoint, neighbours.at(currentNeighbour) );
                numberOfNeighbours++;
            }
            else
            {
                ocludedNeighbours = true;
            }
        }
        if(numberOfNeighbours != 0)
        {
            mValues[currentViewpoint] /= numberOfNeighbours;
            if( mValues.at(currentViewpoint) < minValue )
            {
                minValue = mValues.at(currentViewpoint);
            }
        }
        else if(ocludedNeighbours)
        {
            viewpointsOutOfDomain.push_back(currentViewpoint);
        }
    }
    //The minimum value is assigned to the viewpoints out of the domain (viewpoints that don't see anything)
    for( size_t currentViewpoint = 0; currentViewpoint < viewpointsOutOfDomain.size(); currentViewpoint++ )
    {
        mValues[viewpointsOutOfDomain.at(currentViewpoint)] = minValue;
    }
    mSort = Tools::GetOrderedIndexes(mValues);
    mPositions = Tools::GetOrderedIndexes(mSort);
    mComputed = true;
}

float Unstability::GetDissimilarity(std::shared_ptr<ProjectedAreasMatrix const> pProjectedAreasMatrix, size_t pViewpointI, size_t pViewpointJ)
{
    size_t numberOfPolygons = pProjectedAreasMatrix->GetNumberOfPolygons();
    unsigned int a_t_i = pProjectedAreasMatrix->GetSumPerViewpoint(pViewpointI);
    unsigned int a_t_j = pProjectedAreasMatrix->GetSumPerViewpoint(pViewpointJ);
    unsigned int a_t_ij = a_t_i + a_t_j;
    float dissimilarity = 0.0f;
    if( a_t_ij != 0 )
    {
        for( size_t currentPolygon = 0; currentPolygon < numberOfPolygons; currentPolygon++ )
        {
            unsigned int a_z_i = pProjectedAreasMatrix->GetValue(pViewpointI, currentPolygon);
            unsigned int a_z_j = pProjectedAreasMatrix->GetValue(pViewpointJ, currentPolygon);
            unsigned int a_z_ij = a_z_i + a_z_j;
            if( a_z_ij != 0 )
            {
                dissimilarity -= a_z_ij * glm::log2( a_z_ij / (float)a_t_ij );
            }
            if( a_z_i != 0 )
            {
                dissimilarity += a_z_i * glm::log2( a_z_i / (float)a_t_i );
            }
            if( a_z_j != 0 )
            {
                dissimilarity += a_z_j * glm::log2( a_z_j / (float)a_t_j );
            }
        }
        dissimilarity /= a_t_ij;
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
