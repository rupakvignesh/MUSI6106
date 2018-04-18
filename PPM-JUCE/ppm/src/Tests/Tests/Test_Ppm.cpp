#include "MUSI6106Config.h"

#ifdef WITH_TESTS
#include <cassert>

#include "UnitTest++.h"
#include "Ppm.h"
#include "Vector.h"
#include "Dtw.h"

SUITE(Ppm)
{
    struct PpmData
    {
        PpmData()
        {
            m_pCPpm     = new CPpm ();
        }
        
        ~PpmData()
        {
            delete m_pCPpm;
            
            
        }
        
        CPpm *m_pCPpm;
        
        int m_iNumberOfChannels;
        
        float m_fAlphaAT;
        float m_fAlphaRT;
        
        float m_fMaxPpm;
        float* m_pfLastPpm;
        
        bool m_bIsInitialized;
        
    };
    
    TEST_FIXTURE(PpmData, Api)
    {
        
    }
    
    TEST_FIXTURE(PpmData, ModulatedSineWave)
    {
        
    }
    
    TEST_FIXTURE(PpmData, ZeroInput)
    {
        
    }
    
    TEST_FIXTURE(PpmData, DCInput)
    {
        
    }
    
    TEST_FIXTURE(PpmData, test5)
    {
        
    }
    
    TEST_FIXTURE(PpmData, test6)
    {
    
    }

}

#endif //WITH_TESTS