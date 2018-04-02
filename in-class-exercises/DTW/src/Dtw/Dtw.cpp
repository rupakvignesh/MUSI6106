
#include "Vector.h"
#include "Util.h"
#include "Algorithm.h"
#include "Dtw.h"

CDtw::CDtw( void ): ppfCostMatrix(0), ppEbackTrack(0), m_iNumRows(0), m_iNumCols(0), m_bIsInitialized(false)
{
}

CDtw::~CDtw( void )
{
    reset();
}

Error_t CDtw::init( int iNumRows, int iNumCols )
{
    if(m_bIsInitialized)
        return kNoError;
    m_iNumRows = iNumRows;
    m_iNumCols = iNumCols;
    ppfCostMatrix = new float*[m_iNumRows];
    for(int i=0; i<iNumRows; i++){
        ppfCostMatrix[i] = new float[m_iNumCols]();
        ppEbackTrack[i] = new Directions_t[m_iNumCols]();
    }
    m_bIsInitialized = true;
    return kNoError;
}

Error_t CDtw::reset()
{
    if(m_bIsInitialized){
        for (int i=0; i<m_iNumRows; i++){
            delete [] ppfCostMatrix[i];
            delete [] ppEbackTrack[i];
        }
        delete ppfCostMatrix;
        delete ppEbackTrack;
    }
    return kNoError;
}

Error_t CDtw::process(float **ppfDistanceMatrix)
{
    ppfCostMatrix[0][0] = ppfDistanceMatrix[0][0];
    for(int i=1; i<m_iNumRows; i++){
        ppfCostMatrix[i][0] = ppfCostMatrix[i-1][0] + ppfDistanceMatrix[i][0];
        ppEbackTrack[i][0] = kVert;
    }
    for(int j=1; j<m_iNumCols; j++){
        ppfCostMatrix[0][j] = ppfCostMatrix[0][j-1] + ppfDistanceMatrix[0][j];
        ppEbackTrack[0][j] = kHoriz;
    }
    int temp_min;
    Directions_t temp_dir;
    for(int i=1; i<m_iNumRows; i++){
        for(int j=1; j<m_iNumCols; j++){
            temp_min = ppfDistanceMatrix[i-1][j];
            temp_dir = kVert;
            if(temp_min > ppfDistanceMatrix[i-1][j-1]){
                temp_min = ppfDistanceMatrix[i-1][j-1];
                temp_dir = kDiag;
            }
            else if(temp_min > ppfDistanceMatrix[i][j-1]){
                temp_min = ppfDistanceMatrix[i][j-1];
                temp_dir = kHoriz;
            }
            ppfCostMatrix[i][j] = temp_min;
            ppEbackTrack[i][j] = temp_dir;
        }
    }
    return kNoError;
}

int CDtw::getPathLength()
{    
    return 0;
}

float CDtw::getPathCost() const
{
    return 0.f;
}

Error_t CDtw::getPath( int **ppiPathResult ) const
{
    return kNoError;
}

