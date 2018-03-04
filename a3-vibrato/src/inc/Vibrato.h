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

    CVibrato ();
    virtual ~CVibrato ();
    
    Error_t resetInstance ();
    
    /*! list of parameters for the Vibrato */
    enum VibratoParam_t
    {
        kParamWidth,                     //!< Width = amplitude for oscillation
        kParamModFreq,
        
        kNumVibratoParams
    };
    
    /*! creates a new comb filter instance
     \param pCCombFilterIf pointer to the new class
     \return Error_t
     */
    static Error_t create (CVibrato*& pCVibrato);
    
    /*! destroys a comb filter instance
     \param pCCombFilterIf pointer to the class to be destroyed
     \return Error_t
     */
    static Error_t destroy (CVibrato*& pCVibrato);
    
    /*! initializes a comb filter instance
     \param eFilterType FIR or IIR
     \param fMaxDelayLengthInS maximum allowed delay in seconds
     \param fSampleRateInHz sample rate in Hz
     \param iNumChannels number of audio channels
     \return Error_t
     */
    Error_t init (float fMaxWidthInS, float fSampleRateInHz, int iNumChannels, float fModFreq, float fModWidth);
    
    /*! resets the internal variables (requires new call of init)
     \return Error_t
     */
    Error_t reset ();
    
    /*! sets a comb filter parameter
     \param eParam what parameter (see ::FilterParam_t)
     \param fParamValue value of the parameter
     \return Error_t
     */
    Error_t setParam (VibratoParam_t eParam, float fParamValue);
    
    /*! return the value of the specified parameter
     \param eParam
     \return float
     */
    float   getParam (VibratoParam_t eParam) const;
    
    /*! processes one block of audio
     \param ppfInputBuffer input buffer [numChannels][iNumberOfFrames]
     \param ppfOutputBuffer output buffer [numChannels][iNumberOfFrames]
     \param iNumberOfFrames buffer length (per channel)
     \return Error_t
     */
    Error_t process (float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames);
    

    
private:
    CRingBuffer<float>  **m_ppCRingBuffer;
    CLfo                *m_pCLfo;
    
    float               m_afParam[CVibrato::kNumVibratoParams];
    
    int                 m_iNumChannels;
    
    float               m_fMaxWidthInS;
    
    bool                m_bIsInitialized;   //!< internal bool to check whether the init function has been called
    
    float               m_fSampleRateInHz;      //!< audio sample rate in Hz

};

#endif // #if !defined(__Vibrato_hdr__)