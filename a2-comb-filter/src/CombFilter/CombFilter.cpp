
// standard headers
#include <limits>

// project headers
#include "MUSI6106Config.h"

#include "ErrorDef.h"
#include "Util.h"
#include "RingBuffer.h"

#include "CombFilterIf.h"
#include "CombFilter.h"


CCombFilterBase::CCombFilterBase( int iMaxDelayInFrames, int iNumChannels ) :
    m_ppCRingBuffer(0),
    m_iNumChannels(iNumChannels)
{

    assert(iMaxDelayInFrames > 0);
    assert(iNumChannels > 0);

    m_ppCRingBuffer = new CRingBuffer<float>*[iNumChannels];
    for(int i=0; i<iNumChannels; i++){
        m_ppCRingBuffer[i] = new CRingBuffer<float>(iMaxDelayInFrames);
        m_ppCRingBuffer[i]->reset();
    }
    m_aafParamRange[CCombFilterIf::kParamDelay][0] = 0;
    m_aafParamRange[CCombFilterIf::kParamDelay][1] = iMaxDelayInFrames;
    m_aafParamRange[CCombFilterIf::kParamGain][0] = std::numeric_limits<float>::lowest();
    m_aafParamRange[CCombFilterIf::kParamGain][1] = std::numeric_limits<float>::max();
    
}

CCombFilterBase::~CCombFilterBase()
{
}

Error_t CCombFilterBase::resetInstance()
{
    m_afParam[0] = 0;
    m_afParam[1] = 0;
    return kNoError;
}

Error_t CCombFilterBase::setParam( CCombFilterIf::FilterParam_t eParam, float fParamValue )
{
    if (!isInParamRange(eParam, fParamValue))
        return kFunctionInvalidArgsError;

    switch (eParam) {
        case CCombFilterIf::kParamDelay:
            m_afParam[0] = fParamValue;
            break;
        
        default:
        case CCombFilterIf::kParamGain:
            m_afParam[1] = fParamValue;
            break;
    }
    
    return kNoError;
}

float CCombFilterBase::getParam( CCombFilterIf::FilterParam_t eParam ) const
{
 
    switch (eParam) {
        case CCombFilterIf::kParamDelay:
            return m_afParam[0];
            
        default:
        case CCombFilterIf::kParamGain:
            return m_afParam[1];
            break;
    }
    return 0;
}

bool CCombFilterBase::isInParamRange( CCombFilterIf::FilterParam_t eParam, float fValue )
{
    if (fValue < m_aafParamRange[eParam][0] || fValue > m_aafParamRange[eParam][1])
    {
        return false;
    }
    else
    {
        return true;
    }
}

Error_t CCombFilterFir::process( float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames )
{
    bool flag = false;
    for(int i=0; i<iNumberOfFrames; i++){
        for(int j=0; j<m_iNumChannels; j++){
            ppfOutputBuffer[j][i] = ppfInputBuffer[j][i] + m_afParam[1]*m_ppCRingBuffer[j]->getPostInc(-m_afParam[0]);
            m_ppCRingBuffer[j]->putPostInc(ppfInputBuffer[j][i]);
            if(ppfOutputBuffer[j][i]>=1 || ppfOutputBuffer[j][i]<-1 )
                flag = true;
        }
    }
    if(flag)
    fprintf( stderr, "The output wave file has been clipped");
    return kNoError;
}


CCombFilterIir::CCombFilterIir (int iMaxDelayInFrames, int iNumChannels) : CCombFilterBase(iMaxDelayInFrames, iNumChannels)
{
    m_aafParamRange[CCombFilterIf::kParamGain][0] = -1;
    m_aafParamRange[CCombFilterIf::kParamGain][1] = 1;
}

Error_t CCombFilterIir::process( float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames )
{
    for(int i=0; i<iNumberOfFrames; i++){
        for(int j=0; j<m_iNumChannels; j++){
            ppfOutputBuffer[j][i] = ppfInputBuffer[j][i] + m_afParam[1]*m_ppCRingBuffer[j]->getPostInc(-m_afParam[0]);
            m_ppCRingBuffer[j]->putPostInc(ppfOutputBuffer[j][i]);
        }
    }
    return kNoError;
}
