#include "MUSI6106Config.h"

#ifdef WITH_TESTS
#include <cassert>

#include "UnitTest++.h"
#include "Ppm.h"
#include "Vector.h"
#include "Synthesis.h"

SUITE(Ppm)
{
    struct PpmData
    {
        PpmData() : m_iNumberOfChannels(2),
                    m_iNumFrames(100),
                    m_bIsInitialized(false),
                    m_fAlphaAT(0.10),
                    m_fAlphaRT(1.5),
                    m_ppfInputBuffer(0),
                    m_iSampleRateHz(16000)
        
        {
            m_pCPpm     = new CPpm ();
            m_ppfInputBuffer   = new float*[m_iNumberOfChannels];
            for (int c=0;c<m_iNumberOfChannels; c++){
                m_ppfInputBuffer[c] = new float[m_iNumFrames]();
            }
            m_pfLastPpm = new float[m_iNumberOfChannels]();
            m_bIsInitialized = true;
        }
        
        ~PpmData()
        {
            delete m_pCPpm;
            for (int c=0;c < m_iNumberOfChannels; c++){
                delete m_ppfInputBuffer[c];
            }
            
            delete [] m_pfLastPpm;
            delete [] m_ppfInputBuffer;
            m_bIsInitialized = false;
        }
        
        
        CPpm *m_pCPpm;
        int m_iSampleRateHz;
        int m_iNumberOfChannels;
        int m_iNumFrames;
        
        float m_fAlphaAT;
        float m_fAlphaRT;
        
        float m_fMaxPpm;
        float* m_pfLastPpm;
        
        float **m_ppfInputBuffer;
        
        bool m_bIsInitialized;
        
    };
    
    TEST_FIXTURE(PpmData, Api)
    {
        CHECK_EQUAL(kFunctionInvalidArgsError, m_pCPpm->init(-1,4, -8, 0));
        CHECK_EQUAL(kNotInitializedError, m_pCPpm->process((const float**)m_ppfInputBuffer, m_iNumFrames));
        CHECK_EQUAL(kFunctionInvalidArgsError, m_pCPpm->init(3,0, 0,0));
        
    }
    
    TEST_FIXTURE(PpmData, ModulatedSineWave)
    {
        
    }
    
    TEST_FIXTURE(PpmData, ZeroInput)
    {
        m_pCPpm->init(m_iSampleRateHz, m_iNumberOfChannels, m_fAlphaAT, m_fAlphaRT);
        m_pCPpm->process((const float**)m_ppfInputBuffer, m_iNumFrames);
        CHECK_EQUAL(0.F, m_pCPpm->getMaxPpm());
    }
    
    TEST_FIXTURE(PpmData, DCInput)
    {
       
    
        for (int c = 0; c < m_iNumberOfChannels; c++)
        {
           CSynthesis::generateDc(m_ppfInputBuffer[c], m_iNumFrames, 1.F);
        }
        
        
        m_pCPpm->init(m_iSampleRateHz, m_iNumberOfChannels, m_fAlphaAT, m_fAlphaRT);
        m_pCPpm->process((const float**)m_ppfInputBuffer, m_iNumFrames);
        
        CHECK_EQUAL(1.F, m_pCPpm->getMaxPpm());
        
    }
    
    TEST_FIXTURE(PpmData, test5)
    {
        
    }
    
    TEST_FIXTURE(PpmData, test6)
    {
    
    }

}

#endif //WITH_TESTS