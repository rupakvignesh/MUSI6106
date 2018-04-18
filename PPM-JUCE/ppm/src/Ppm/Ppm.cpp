#include <cmath>
#include "ErrorDef.h"
#include "Ppm.h"

Error_t CPpm::createInstance(CPpm *&pCPpm)
{
    if (pCPpm) {
        return kUnknownError;
    }
    pCPpm = new CPpm ();

    if (!pCPpm)
        return kUnknownError;

    return kNoError;
}

Error_t CPpm::destroyInstance(CPpm *&pCPpm)
{
    if (!pCPpm)
        return kUnknownError;

    delete pCPpm;
    pCPpm = 0;

    return kNoError;
}

CPpm::CPpm():m_iNumberOfChannels(0),
             m_fAlphaAT(0.0f),
             m_fAlphaRT(0.0f),
             m_fMaxPpm(0.0f),
             m_pfLastPpm(nullptr),
             m_bIsInitialized(false)
{
    
}

CPpm::~CPpm()
{
    delete[] m_pfLastPpm;
    m_pfLastPpm = nullptr;
}

Error_t CPpm::init(float sampleRate, int iNumberOfChannels, float fAttackTime, float fReleaseTime) {
    
    if (sampleRate<=0 || iNumberOfChannels <=0 || fAttackTime <= 0 || fReleaseTime <=0){
        return kFunctionInvalidArgsError;
    }
    m_bIsInitialized = true;
    m_iNumberOfChannels = iNumberOfChannels;
    m_fAlphaAT = 1.0f - exp(-2.2f / (sampleRate*fAttackTime));
    m_fAlphaRT = 1.0f - exp(-2.2f / (sampleRate*fReleaseTime));
    delete [] m_pfLastPpm;
    m_pfLastPpm = new float[iNumberOfChannels]();
    return kNoError;
}

Error_t CPpm::reset(){
    
    
    m_iNumberOfChannels = 0;
    m_fAlphaAT = 0;
    m_fAlphaRT = 0;
    delete [] m_pfLastPpm;
    m_fMaxPpm = 0;
    m_bIsInitialized = false;
    return kNoError;
    
}

Error_t CPpm::process(const float **ppfInputBuffer, int iNumberOfFrames)
{
    if (!m_bIsInitialized) {
        return kNotInitializedError;
    }
    
    if(ppfInputBuffer == nullptr || iNumberOfFrames<=0){
        return kFunctionInvalidArgsError;
    }
    m_pfLastPpm[0] = ppfInputBuffer[0][0];
    m_pfLastPpm[1] = ppfInputBuffer[1][0];
    
    m_fMaxPpm = 0;
    for (int f = 0; f < iNumberOfFrames; f++) {
         for (int c = 0; c < m_iNumberOfChannels; c++){
            if (ppfInputBuffer[c][f] < m_pfLastPpm[c]) {
                m_pfLastPpm[c] *= (1 - m_fAlphaRT);
            } else {
                m_pfLastPpm[c] = ppfInputBuffer[c][f] * m_fAlphaAT + (1 - m_fAlphaAT) * m_pfLastPpm[c];
            }
            if(m_fMaxPpm < m_pfLastPpm[c])
                m_fMaxPpm = m_pfLastPpm[c];
        }
    }
    return kNoError;
}

float CPpm::getMaxPpm() {
    if (!m_bIsInitialized) {
        return kNotInitializedError;
    }
    return m_fMaxPpm;
}









