#if !defined(__Ppm_hdr__)
#define __Ppm_hdr__

#include "ErrorDef.h"

class CPpm
{
public:
    static Error_t createInstance (CPpm*& pCPpm);
    static Error_t destroyInstance (CPpm*& pCPpm);
    Error_t init(float fSampleRate, int iNumberOfChannels);
    Error_t process (const float **ppfInputBuffer, int iNumberOfFrames);
    float getMaxPpm();


protected:
    CPpm ();
    virtual ~CPpm ();

private:
    int m_iNumberOfChannels;
    
    float m_fAlphaAT;
    float m_fAlphaRT;
    
    float m_fMaxPpm;
    float* m_pfLastPpm;
    
    bool m_bIsInitialized;
};

#endif // #if !defined(__Ppm_hdr__)
