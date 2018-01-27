
#include <iostream>
#include <ctime>

#include "MUSI6106Config.h"

#include "AudioFileIf.h"
#include "RingBuffer.h"

using std::cout;
using std::endl;

// local function declarations
void    showClInfo ();

/////////////////////////////////////////////////////////////////////////////////
// main function
int main(int argc, char* argv[])
{
    // You don't need to use an audio file this time, though you can try if you want.
    // std::string             sInputFilePath,                 //!< file paths
    //                         sOutputFilePath;
     static const int        kBlockSize          = 8;
    // float                   **ppfAudioData      = 0;
    // CAudioFileIf            *phAudioFile        = 0;
    // CAudioFileIf::FileSpec_t stFileSpec;
    // long long               iInFileLength       = 0;        //!< length of input file

    clock_t                 time                = 0;

    CRingBuffer<float>      *pCRingBuffer       = 0;
    float                   *pfTestSignal       = 0;
    float                   fGain               = 1.f;

    showClInfo ();

    //////////////////////////////////////////////////////////////////////////////
    // allocate memory
    time                    = clock();
    pCRingBuffer = new CRingBuffer<float> (kBlockSize);

    // fill the test signal (e.g., a unit impulse)
    pfTestSignal = new float [kBlockSize];
    pfTestSignal[0] = 1;                            // Unit impulse
    for(int i=1;i<kBlockSize;i++){
        pfTestSignal[i] = 0.5;
    }
    //////////////////////////////////////////////////////////////////////////////
    // do processing and tests
    
    //Test case I. Fill the buffer with test signal and output values.
    cout<<"Test case I. Fill the buffer with test signal and output values."<<endl;
    for(int i=0;i<kBlockSize;i++){
        pCRingBuffer->putPostInc(pfTestSignal[i]);
    }
    
    for(int i=0;i<3*kBlockSize;i++){
        cout<<pCRingBuffer->getPostInc()<<endl;  //Should output the buffer once then output garbage twice
    }
    
    //Test case II. Function calls.
    cout<<"Test case II. Function calls."<<endl;
    cout<<pCRingBuffer->getWriteIdx()<<endl;    //Should output 1024 (or 0?).
    pCRingBuffer->put(521);
    cout<<pCRingBuffer->getReadIdx()<<endl;     //Should output 1024 (or 0?).
    pCRingBuffer->setReadIdx(pCRingBuffer->getWriteIdx());
    cout<<pCRingBuffer->getReadIdx()<<endl; // Should  output 1024 (or 0?)
    cout<<pCRingBuffer->get(0)<<endl; //Should output 123
    cout<<pCRingBuffer->get(-4098)<<endl; //Should output 123
    

    cout << "processing done in: \t"    << (clock()-time)*1.F/CLOCKS_PER_SEC << " seconds." << endl;

    //////////////////////////////////////////////////////////////////////////////
    // clean-up

    return 0;
}


void     showClInfo()
{
    cout << "GTCMT MUSI6106 Executable" << endl;
    cout << "(c) 2014-2018 by Alexander Lerch" << endl;
    cout  << endl;

    return;
}

