//Definition include
#include "SaliencyEVMI.h"

//Dependency includes
#include "glm/exponential.hpp"

//Project includes
#include "Debug.h"
#include "Tools.h"

SaliencyEVMI::SaliencyEVMI(const QString& pName): Measure(pName, false)
{

}

SaliencyEVMI::~SaliencyEVMI()
{

}

void SaliencyEVMI::Compute(const SceneInformationBuilder *pSceneInformationBuilder)
{
    const ProjectedAreasMatrix* projectedAreasMatrix = pSceneInformationBuilder->GetProjectedAreasMatrix();
    int numberOfViewpoints = projectedAreasMatrix->GetNumberOfViewpoints();
    int numberOfPolygons = projectedAreasMatrix->GetNumberOfPolygons();
    QVector< float > polygonalSaliency(numberOfPolygons, 0.0f);
    QVector< QVector< int > > serializedPolygonNeighbours = pSceneInformationBuilder->GetSerializedPolygonNeighbours();
    QVector< int > polygonsOutOfDomain;
    float maxValue = -FLT_MAX;
    for( int currentPolygon = 0; currentPolygon < numberOfPolygons; currentPolygon++ )
    {
        unsigned int numberOfNeighbours = 0;
        bool ocludedNeighbours = false;

        QVector< int > neighbours = serializedPolygonNeighbours.at(currentPolygon);
        if(neighbours.size() == 0)
        {
            Debug::Log("SaliencyEVMI::No neighbours");
        }
        for( int currentNeighbour = 0; currentNeighbour < neighbours.size(); currentNeighbour++ )
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
            if( polygonalSaliency.at(currentPolygon) > maxValue )
            {
                maxValue = polygonalSaliency.at(currentPolygon);
            }
        }
        else if(ocludedNeighbours)
        {
            polygonsOutOfDomain.push_back(currentPolygon);
        }
    }
    //We assign the maximum value to the polygons that their saliency can not be computed
    for( int currentPolygon = 0; currentPolygon < polygonsOutOfDomain.size(); currentPolygon++ )
    {
        polygonalSaliency[polygonsOutOfDomain.at(currentPolygon)] = maxValue;
    }
    QVector< int > viewpointsOutOfDomain;
    mValues.fill( 0.0f, numberOfViewpoints );
    maxValue = -FLT_MAX;
    for( int currentViewpoint = 0; currentViewpoint < numberOfViewpoints; currentViewpoint++ )
    {
        unsigned int a_t = projectedAreasMatrix->GetSumPerViewpoint(currentViewpoint);
        if( a_t != 0 )
        {
            float auxDependency = 0.0f;
            for( int currentPolygon = 0; currentPolygon < numberOfPolygons; currentPolygon++ )
            {
                unsigned int sumDependency = projectedAreasMatrix->GetSumPerPolygon(currentPolygon);
                if(sumDependency != 0)
                {
                    auxDependency += sumDependency * polygonalSaliency.at(currentPolygon);
                }
            }
            for( int currentPolygon = 0; currentPolygon < numberOfPolygons; currentPolygon++ )
            {
                unsigned int a_z = projectedAreasMatrix->GetValue(currentViewpoint, currentPolygon);
                unsigned int sum_a_z = projectedAreasMatrix->GetSumPerPolygon(currentPolygon);
                if( a_z != 0 )
                {
                    float aux = a_z / (float)a_t;
                    if(polygonalSaliency.at(currentPolygon) != 0.0f)
                    {
                        mValues[currentViewpoint] += a_z * glm::log2( aux * ( auxDependency / ( polygonalSaliency.at(currentPolygon) * sum_a_z ) ) );
                    }
                }
            }
            mValues[currentViewpoint] /= a_t;
            if( mValues.at(currentViewpoint) > maxValue )
            {
                maxValue = mValues.at(currentViewpoint);
            }
        }
        else
        {
            viewpointsOutOfDomain.push_back(currentViewpoint);
        }
    }
    //The maximum value is assigned to the viewpoints out of the domain (viewpoints that don't see anything)
    for( int currentViewpoint = 0; currentViewpoint < viewpointsOutOfDomain.size(); currentViewpoint++ )
    {
        mValues[viewpointsOutOfDomain.at(currentViewpoint)] = maxValue;
    }
    mSort = Tools::GetOrderedIndexes(mValues);
    mPositions = Tools::GetPositions(mSort);
    mComputed = true;
}

float SaliencyEVMI::GetDissimilarity(const ProjectedAreasMatrix *pProjectedAreasMatrix, int pPolygonI, int pPolygonJ)
{
    int numberOfViewpoints = pProjectedAreasMatrix->GetNumberOfViewpoints();
    unsigned int sum_a_z_i = pProjectedAreasMatrix->GetSumPerPolygon(pPolygonI);
    unsigned int sum_a_z_j = pProjectedAreasMatrix->GetSumPerPolygon(pPolygonJ);
    unsigned int sum_a_z_ij = sum_a_z_i + sum_a_z_j;
    float dissimilarity = 0.0f;
    if( sum_a_z_ij != 0 )
    {
        for( int currentViewpoint = 0; currentViewpoint < numberOfViewpoints; currentViewpoint++ )
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
