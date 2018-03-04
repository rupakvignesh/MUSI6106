
// standard headers

// project headers
#include "MUSI6106Config.h"

#include "ErrorDef.h"

#include "RingBuffer.h"

#include "Lfo.h"
#include "Vibrato.h"


CVibrato::CVibrato():
m_pCLfo(0),
m_ppCRingBuffer(0),
m_iNumChannels(0),
m_fSampleRateInHz(0),
m_bIsInitialized(false)
{
    
}

CVibrato::~CVibrato(){
    if(m_ppCRingBuffer){
        for (int i=0; i<m_iNumChannels; i++){
            delete m_ppCRingBuffer[i];
        }
        delete m_ppCRingBuffer;
    }
}

Error_t CVibrato::create(CVibrato *&pCVibrato){
    pCVibrato = new CVibrato();
    return kNoError;
}

Error_t CVibrato::destroy(CVibrato *&pCVibrato){
     delete pCVibrato;
    return kNoError;
}

Error_t CVibrato::init(float fMaxWidthInS, float fSampleRateInHz, int iNumChannels, float fModFreq, float fModWidth){
    
    m_ppCRingBuffer = new CRingBuffer<float>*[iNumChannels];
    m_fMaxWidthInS = fMaxWidthInS;
    int iMaxWidthInSamples = fMaxWidthInS*fSampleRateInHz;
    m_afParam[VibratoParam_t::kParamModFreq] = fModFreq;
    m_afParam[VibratoParam_t::kParamWidth] = fModWidth;
    m_fSampleRateInHz = fSampleRateInHz;
    m_iNumChannels = iNumChannels;
    m_pCLfo = new CLfo(fModFreq, fModFreq);
    for(int i=0; i<iNumChannels; i++){
        m_ppCRingBuffer[i] = new CRingBuffer<float>(iMaxWidthInSamples*2 +2);
        m_ppCRingBuffer[i]->setReadIdx(0);
        m_ppCRingBuffer[i]->setWriteIdx(m_afParam[VibratoParam_t::kParamWidth]*m_fSampleRateInHz);
    }
    
    m_bIsInitialized = true;
    
    return kNoError;
    
}

Error_t CVibrato::reset(){
    for(int i=0;i<m_iNumChannels; i++){
        m_ppCRingBuffer[i]->reset();
    }
    m_pCLfo->reset();
    m_afParam[kParamModFreq] = 0;
    m_afParam[kParamWidth] = 0;
    return kNoError;
}

Error_t CVibrato::setParam(VibratoParam_t eParam, float fParamValue){
    
    switch (eParam) {
        case VibratoParam_t::kParamModFreq:
            if(fParamValue>m_fMaxWidthInS){
                return kFunctionInvalidArgsError;
            }
            m_afParam[eParam] = fParamValue;
            m_pCLfo->setLfoRate(fParamValue);
            break;
            
        default:
        case VibratoParam_t::kParamWidth:
            m_afParam[VibratoParam_t::kParamWidth] = fParamValue;
            break;
    }
    
    return kNoError;
    
}

float CVibrato::getParam(VibratoParam_t eParam) const{
    return m_afParam[eParam];
}

Error_t CVibrato :: process (float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames){

    for(int i=0; i<iNumberOfFrames; i++){
        for(int j=0; j<m_iNumChannels; j++){
            m_ppCRingBuffer[j]->putPostInc(ppfInputBuffer[j][i]);
            ppfOutputBuffer[j][i] =  m_ppCRingBuffer[j]->get(m_pCLfo->getLfoValue()*m_afParam[VibratoParam_t::kParamWidth]*m_fSampleRateInHz);
            m_ppCRingBuffer[j]->getPostInc();
        }
    }
    return kNoError;
}