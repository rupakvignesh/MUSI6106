#if !defined(__Lfo_hdr__)
#define __Lfo_hdr__

#define _USE_MATH_DEFINES
#include <math.h>

#include "ErrorDef.h"
#include "Synthesis.h"
#include "Ringbuffer.h"

class CLfo
{
public:
    CLfo(float fModFreq, float fSampleRateinHz) :
    m_fModFreq(fModFreq),
    m_fFixedSampleRateInHz(fSampleRateinHz),
    m_fCurPos(0)
    {
        m_iFixedBufferLength = m_fFixedSampleRateInHz/m_fFixedModFreq;
        m_fIncrement = m_iFixedBufferLength/(fSampleRateinHz/fModFreq);
        float pfOutBuf[m_iFixedBufferLength];
        
        CSynthesis::generateSine (pfOutBuf, m_fModFreq, m_fSampleFreqInHz, m_iFixedBufferLength);
        for (int i=0; i<m_iFixedBufferLength; i++){
            m_pCRingBuffer->putPostInc(pfOutBuf[i]);
        }
    }
    
    ~CLfo(){
        delete m_pCRingBuffer;
    }
    
    Error_t reset(){
        m_fCurPos = 0;
        return kNoError;
    }
    
    Error_t setLfoRate(float fLfoRate){
        m_fModFreq = fLfoRate;
        m_fIncrement = m_iFixedBufferLength / (m_fSampleFreqInHz/m_fModFreq);
        return kNoError;
    }
    
    float getLfoValue(){
        float result = m_pCRingBuffer->get(m_fCurPos);
        m_fCurPos += m_fIncrement;
        if(m_fCurPos>=m_iFixedBufferLength){
            m_fCurPos = m_fCurPos - m_iFixedBufferLength;
        }
        return result;
    }
    

private:
    float               m_fModFreq;
    float               m_fWidth;
    CRingBuffer<float>  *m_pCRingBuffer;
    const float         m_fFixedModFreq = 10;
    const float         m_fFixedSampleRateInHz = 44100;
    int                 m_iFixedBufferLength;
    float               m_fSampleFreqInHz;
    float               m_fIncrement;
    float               m_fCurPos;
    
};

#endif // __Lfo_hdr__