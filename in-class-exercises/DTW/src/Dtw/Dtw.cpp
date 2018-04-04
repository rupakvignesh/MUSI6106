
#include "Vector.h"
#include "Util.h"
#include "Dtw.h"

CDtw::CDtw( void ): ppfCostMatrix(0), ppEbackTrack(0), m_iNumRows(0), m_iNumCols(0), m_bIsInitialized(false), m_iDtwPathLength(0)

{
}

CDtw::~CDtw( void )
{
    reset();
}

Error_t CDtw::init( int iNumRows, int iNumCols )
{
    if (iNumRows<=0 || iNumCols<=0){
        return kFunctionInvalidArgsError;
    }
    if(m_bIsInitialized)
        return kNoError;
    m_iNumRows = iNumRows;
    m_iNumCols = iNumCols;
    ppfCostMatrix = new float*[m_iNumRows];
    ppEbackTrack = new Directions_t*[m_iNumRows];
    for(int i=0; i<iNumRows; i++){
        ppfCostMatrix[i] = new float[m_iNumCols]();
        ppEbackTrack[i] = new Directions_t[m_iNumCols]();
    }
    ppiPathResult = new int*[kNumMatrixDimensions];
    for (int i=0; i<kNumMatrixDimensions; i++){
        ppiPathResult[i] = new int [m_iNumCols+m_iNumRows-2]();
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
        for (int i=0; i<kNumMatrixDimensions; i++){
            delete [] ppiPathResult[i];
        }
        delete ppiPathResult;
    }
    return kNoError;
}

Error_t CDtw::process(float **ppfDistanceMatrix)
{
    if(!m_bIsInitialized){
        return kNotInitializedError;
    }
    if(ppfDistanceMatrix==nullptr || ppfCostMatrix==nullptr){
        return kFunctionInvalidArgsError;
    }
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
    int iDTWCost;
    for(int i=1; i<m_iNumRows; i++){
        for(int j=1; j<m_iNumCols; j++){
            temp_min = ppfCostMatrix[i-1][j];
            temp_dir = kVert;
            if(temp_min > ppfCostMatrix[i-1][j-1]){
                temp_min = ppfCostMatrix[i-1][j-1];
                temp_dir = kDiag;
            }
            else if(temp_min > ppfCostMatrix[i][j-1]){
                temp_min = ppfCostMatrix[i][j-1];
                temp_dir = kHoriz;
            }
            ppfCostMatrix[i][j] = temp_min + ppfDistanceMatrix[i][j];
            ppEbackTrack[i][j] = temp_dir;
        }
    }
    
    
    int i=m_iNumRows-1, j=m_iNumCols-1;
    while(i>=0 && j>=0){
            auto dir = ppEbackTrack[i][j];
            if (dir == kDiag){
                i--;
                j--;
            }
            else if(dir == kHoriz){
                j--;
            }
            else{
                i--;
            }
            m_iDtwPathLength++;
        
    }
    
    
    
    
    return kNoError;
}

int CDtw::getPathLength()
{
    return m_iDtwPathLength;
}

float CDtw::getPathCost() const
{
    return ppfCostMatrix[m_iNumRows-1][m_iNumCols-1];
}

Error_t CDtw::getPath( int **ppiPathResult ) const
{
    int k=0;
    int i=m_iNumRows-1;
    int j=m_iNumCols-1;
    while(i>=0 && j>=0){
        auto dir = ppEbackTrack[i][j];
        ppiPathResult[0][m_iDtwPathLength-k-1] = i;
        ppiPathResult[1][m_iDtwPathLength-k-1] = j;
        if (dir == kDiag){
            i--;
            j--;
        }
        else if(dir == kHoriz){
            j--;
        }
        else{
            i--;
        }
        k++;
    }
    return kNoError;
}

