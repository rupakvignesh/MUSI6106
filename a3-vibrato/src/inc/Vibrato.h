#if !defined(__Vibrato_hdr__)
#define __Vibrato_hdr__

#include "ErrorDef.h"

// forward declaration
class CLfo;
template <class T>
class CRingBuffer;

/*
 * brief explanation about your class-interface design
 */

class CVibrato
{
public:

    CVibrato (int iMaxDelayInFrames, int iNumChannels);
    virtual ~CVibrato ();
    
    Error_t resetInstance ();
    
    /*! list of parameters for the Vibrato */
    enum FilterParam_t
    {
        kParamWidth,                     //!< Width = amplitude for oscillation
        kParamDelay,                    //!< delay in seconds for specification of comb width
        kParamModFreq,
        
        kNumFilterParams
    };
    
    /*! initializes a comb filter instance
     \param eFilterType FIR or IIR
     \param fMaxDelayLengthInS maximum allowed delay in seconds
     \param fSampleRateInHz sample rate in Hz
     \param iNumChannels number of audio channels
     \return Error_t
     */
    Error_t init (float fMaxDelayLengthInS, float fSampleRateInHz, int iNumChannels);
    
    /*! resets the internal variables (requires new call of init)
     \return Error_t
     */
    Error_t reset ();
    
    /*! sets a comb filter parameter
     \param eParam what parameter (see ::FilterParam_t)
     \param fParamValue value of the parameter
     \return Error_t
     */
    Error_t setParam (FilterParam_t eParam, float fParamValue);
    
    /*! return the value of the specified parameter
     \param eParam
     \return float
     */
    float   getParam (FilterParam_t eParam) const;
    
    /*! processes one block of audio
     \param ppfInputBuffer input buffer [numChannels][iNumberOfFrames]
     \param ppfOutputBuffer output buffer [numChannels][iNumberOfFrames]
     \param iNumberOfFrames buffer length (per channel)
     \return Error_t
     */
    Error_t process (float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames);
    

    
private:
    CRingBuffer<float>  **m_ppCRingBuffer;
    
    float   m_afParam[CVibrato::kNumFilterParams];
    float   m_aafParamRange[CVibrato::kNumFilterParams][2];
    
    int     m_iNumChannels;
    
    bool            m_bIsInitialized;   //!< internal bool to check whether the init function has been called
    
    float           m_fSampleRate;      //!< audio sample rate in Hz

};

#endif // #if !defined(__Vibrato_hdr__)