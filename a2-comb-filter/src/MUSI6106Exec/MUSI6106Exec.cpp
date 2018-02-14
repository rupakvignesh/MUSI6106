
#include <iostream>
#include <ctime>
#include <cassert>
#include <math.h>
#include "MUSI6106Config.h"

#include "AudioFileIf.h"
#include "CombFilterIf.h"

using std::cout;
using std::endl;

// local function declarations
void    showClInfo ();

void checkDelay(float fDelayInS, float fMaxDelayLengthInSec){
    //Delay shoud not be greater than ring buffer
    assert(fDelayInS<=fMaxDelayLengthInSec);
    cout<<"Delay Test Passed"<<endl;
}

void checkGain(float *gain){
    //Avoid Feedback blowing up
    if (*gain>1){
        *gain = 1;
    }
    else if (*gain<-1){
        *gain = -1;
    }
}

void setGetParams(CCombFilterIf *&pInstance){
    //Testing the set and get param functions of CombFilter
    pInstance->init(CCombFilterIf::kCombFIR, 1, 44100, 1);
    pInstance->setParam(CCombFilterIf::kParamGain, 0.5);
    pInstance->setParam(CCombFilterIf::kParamDelay, 0.5);
    if(pInstance->getParam(CCombFilterIf::kParamDelay) == 0.5 &&
       pInstance->getParam(CCombFilterIf::kParamGain) == 0.5){
        cout<<"Test Passed. CombFilter Parameters accessed correctly"<<endl;
    }
    else{
        cout<<"Test Failed. Check set and get functions"<<endl;
    }
}

void unitImpulseTest(CCombFilterIf *&pInstance){
    int                     kBlockSize = 1024;
    float                   samplingFreqHz = 44100;
    float                   **ppfUnitImpulse = 0;
    float                   **ppfResponse    = 0;
    ppfUnitImpulse = new float*[1];
    ppfResponse = new float*[1];
    for(int i = 0; i<1; i++){
        ppfUnitImpulse[i] = new float[kBlockSize];
        ppfResponse[i] = new float[kBlockSize];
    }
    for(int i = 0; i<kBlockSize; i++){
        ppfUnitImpulse[0][i] = 0;
        ppfResponse[0][i] = 0;
    }
    ppfUnitImpulse[0][0] = 1;
    //FIR
    pInstance->init(CCombFilterIf::kCombFIR, 1, samplingFreqHz, 1);
    pInstance->setParam(CCombFilterIf::kParamGain, 0.5);
    pInstance->setParam(CCombFilterIf::kParamDelay, 10/samplingFreqHz);
    pInstance->process(ppfUnitImpulse, ppfResponse, kBlockSize);
    assert(ppfResponse[0][10] == 0.5*ppfUnitImpulse[0][0]);
    assert(ppfResponse[0][20] == 0);
    
    cout<<"Unit impulse test case passed for FIR Comb"<<endl;
    
    //IIR
    pInstance->reset();
    for(int i = 0; i<1; i++){
        ppfUnitImpulse[i] = new float[kBlockSize];
        ppfResponse[i] = new float[kBlockSize];
    }
    for(int i = 0; i<kBlockSize; i++){
        ppfUnitImpulse[0][i] = 0;
        ppfResponse[0][i] = 0;
    }
    ppfUnitImpulse[0][0] = 1;
    pInstance->init(CCombFilterIf::kCombIIR, 1, samplingFreqHz, 1);
    pInstance->setParam(CCombFilterIf::kParamGain, 0.5);
    pInstance->setParam(CCombFilterIf::kParamDelay, 10/samplingFreqHz);
    pInstance->process(ppfUnitImpulse, ppfResponse, kBlockSize);
    assert(ppfResponse[0][10] == 0.5*ppfUnitImpulse[0][0]);
    assert(ppfResponse[0][20] == 0.5*0.5*ppfUnitImpulse[0][0]);
    assert(ppfResponse[0][30] == 0.5*0.5*0.5*ppfUnitImpulse[0][0]);
    
    cout<<"Unit impulse test case passed for IIR Comb"<<endl;
    
    //Delete
    
    delete[] ppfUnitImpulse[0];
    delete[] ppfUnitImpulse;
    delete[] ppfResponse[0];
    delete[] ppfResponse;
    ppfUnitImpulse = 0;
    ppfResponse = 0;
    
}

void testZeroInput(CCombFilterIf *&pInstance)
{
    pInstance->setParam(CCombFilterIf::kParamGain, 0.5);
    pInstance->setParam(CCombFilterIf::kParamDelay, 0.001);
    
    cout << "Testing zero input" << endl;
    
    float fError = 0;;
    float **ppfTestSignal = new float*[0];
    ppfTestSignal[0] = new float[882];
    float **ppfTestOut = new float*[0];
    ppfTestOut[0] = new float[882];
    for(int i = 0; i<882;i++)
    {
        ppfTestSignal[0][i] = 0;
    }
    
    pInstance->process(ppfTestSignal, ppfTestOut, 882);
    
    for(int i = 0;i<882;i++)
    {
        fError = ppfTestOut[0][i]-ppfTestSignal[0][i];
        if(fError != 0)
        {
            cout << "Some error"<<endl;
            cout<< i << endl;
            return;
        }
        if(i == 299)
        {
            cout<< "No Error!"<<endl;
        }
    }
    //Cleanup
    delete [] ppfTestSignal[0];
    delete [] ppfTestSignal;
    delete [] ppfTestOut[0];
    delete [] ppfTestOut;
    ppfTestOut = 0;
    ppfTestSignal = 0;
    
}

void signalLenVsDelay(CCombFilterIf *&pInstance)
{
    cout << "Testing when signal is smaller than delay" << endl;
    pInstance->setParam(CCombFilterIf::kParamGain, 0.5);
    pInstance->setParam(CCombFilterIf::kParamDelay, 0.01);
    float fError = 0;;
    float **ppfTestSignal = new float*[0];
    ppfTestSignal[0] = new float[300];
    float **ppfTestOut = new float*[0];
    ppfTestOut[0] = new float[300];
    ppfTestSignal[0][0] = 0;
    for(int i = 1; i<300;i++)
    {
        ppfTestSignal[0][i] = 0.001+ppfTestSignal[0][i-1];
    }
    
    pInstance->process(ppfTestSignal, ppfTestOut, 300);
    
    for(int i = 0;i<300;i++)
    {
        fError = ppfTestOut[0][i]-ppfTestSignal[0][i];
        if(fError != 0)
        {
            cout << "Some error at pos "<<i<<endl;
            return;
        }
        if(i == 299)
        {
            cout<< "No Error!"<<endl;
        }
        
    }
    
    
}


/////////////////////////////////////////////////////////////////////////////////
// main function
int main(int argc, char* argv[])
{
    std::string             sInputFilePath,                 //!< file paths
        sOutputFilePath;

    static const int        kBlockSize = 1024;

    clock_t                 time = 0;

    float                   **ppfAudioData = 0;
    float                   **ppfOutputData = 0;
    
    float                   fMaxDelayLengthInSec = 1;
    float                   fDelayInSec = 1;
    float                   fGain   =1;

    CAudioFileIf            *phAudioFile = 0;
    std::fstream            hOutputFile;
    CAudioFileIf::FileSpec_t stFileSpec;
    CCombFilterIf::CombFilterType_t fFilterType;
    
    CAudioFileIf            *pCOutputAudio = 0;

    CCombFilterIf   *pInstance = 0;
    CCombFilterIf::create(pInstance);
    showClInfo();

    //////////////////////////////////////////////////////////////////////////////
    // parse command line arguments
    
    switch (argc) {
            
        case 1:
            cout<< "Let the testing begin"<<endl;
            
            // 1. verify param access
            setGetParams(pInstance);
            
            // 2. Unit impulse FIR, IIR
            unitImpulseTest(pInstance);
            
            // 3. Zero Input, and delay length testing for IIR
            pInstance->reset();
            cout<< "Testing IIR"<<endl;
            pInstance->init(CCombFilterIf::kCombIIR, 0.1, 44100, 1);

            testZeroInput(pInstance);
            signalLenVsDelay(pInstance);
            
            // 4. Zero Input, and delay length testing for FIR
            pInstance->reset();
            cout<<"Testing FIR" << endl;
            pInstance->init(CCombFilterIf::kCombFIR, 0.1 , 44100, 1);

            testZeroInput(pInstance);
            signalLenVsDelay(pInstance);
            return 0;
            break;
            
        case 3:
            cout<< "Default delay and gain used" << endl;
            fGain = 0.5;
            fDelayInSec = 0.001;
            sInputFilePath = argv[1];
            sOutputFilePath = sInputFilePath + ".txt";
            if(strcmp(argv[2],"FIR"))
            {
                fFilterType = CCombFilterIf::kCombFIR;
            }
            else if (strcmp(argv[2],"IIR"))
            {
                fFilterType = CCombFilterIf::kCombIIR;
            }
            else
            {
                cout << "Invalid filter type" << endl;
            }
            break;
        case 4:
            cout<<"Default gain used" << endl;
            fGain = 0.5;
            fDelayInSec = std::stof(argv[3]);
            sInputFilePath = argv[1];
            sOutputFilePath = sInputFilePath + ".txt";
            if(strcmp(argv[2],"FIR"))
            {
                fFilterType = CCombFilterIf::kCombFIR;
            }
            else if (strcmp(argv[2],"IIR"))
            {
                fFilterType = CCombFilterIf::kCombIIR;
            }
            else
            {
                cout << "Invalid filter type" << endl;
            }
            break;
            
        case 5:
            sInputFilePath = argv[1];
            sOutputFilePath = sInputFilePath + ".txt";
            fDelayInSec = std::stof(argv[3]);
            fGain       = std::stof(argv[4]);
            if(strcmp(argv[2],"FIR"))
            {
                fFilterType = CCombFilterIf::kCombFIR;
            }
            else if (strcmp(argv[2],"IIR"))
            {
                fFilterType = CCombFilterIf::kCombIIR;
            }
            else
            {
                cout << "Invalid filter type" << endl;
            }
            break;
            
        default:
            cout << "Input arguments error" << endl;
            break;
    }
    
    if (argc < 2)
    {
        cout << "Missing audio input path!";
        return -1;
    }
    else
    {
        sInputFilePath = argv[1];
        sOutputFilePath = sInputFilePath + ".txt";
    }

    //////////////////////////////////////////////////////////////////////////////
    // open the input wave file
    CAudioFileIf::create(phAudioFile);
    phAudioFile->openFile(sInputFilePath, CAudioFileIf::kFileRead);
    if (!phAudioFile->isOpen())
    {
        cout << "Wave file open error!";
        return -1;
    }
    phAudioFile->getFileSpec(stFileSpec);

    //////////////////////////////////////////////////////////////////////////////
    // open the output wave file
    CAudioFileIf::create(pCOutputAudio);
    pCOutputAudio->openFile("output.wav", CAudioFileIf::kFileWrite, &stFileSpec);
    if (!pCOutputAudio->isOpen())
    {
        cout << "Wave file open error!";
        return -1;
    }
    
    //////////////////////////////////////////////////////////////////////////////
    // open the output text file
    hOutputFile.open(sOutputFilePath.c_str(), std::ios::out);
    if (!hOutputFile.is_open())
    {
        cout << "Text file open error!";
        return -1;
    }

    //////////////////////////////////////////////////////////////////////////////
    // allocate memory
    ppfAudioData = new float*[stFileSpec.iNumChannels];
    for (int i = 0; i < stFileSpec.iNumChannels; i++)
        ppfAudioData[i] = new float[kBlockSize];
    
    ppfOutputData = new float*[stFileSpec.iNumChannels];
    for (int i = 0; i< stFileSpec.iNumChannels; i++)
        ppfOutputData[i] = new float[kBlockSize];
    
    
    
    //////////////////////////////////////////////////////////////////////////////
    // initializations
    pInstance->init(fFilterType, fMaxDelayLengthInSec, stFileSpec.fSampleRateInHz, stFileSpec.iNumChannels);
    pInstance->setParam(CCombFilterIf::kParamGain, fGain);
    pInstance->setParam(CCombFilterIf::kParamDelay, fDelayInSec);

    time = clock();
    //////////////////////////////////////////////////////////////////////////////
    // get audio data and write it to the output file
    while (!phAudioFile->isEof())
    {
        long long iNumFrames = kBlockSize;
        phAudioFile->readData(ppfAudioData, iNumFrames);
        pInstance->process(ppfAudioData, ppfOutputData, iNumFrames);
        pCOutputAudio->writeData(ppfOutputData, iNumFrames);
        cout << "\r" << "reading and writing";

        for (int i = 0; i < iNumFrames; i++)
        {
            for (int c = 0; c < stFileSpec.iNumChannels; c++)
            {
                hOutputFile << ppfOutputData[c][i] << "\t";
            }
            hOutputFile << endl;
        }
    }

    cout << "\nreading/writing done in: \t" << (clock() - time)*1.F / CLOCKS_PER_SEC << " seconds." << endl;

    //////////////////////////////////////////////////////////////////////////////
    // clean-up
    CAudioFileIf::destroy(phAudioFile);
    CAudioFileIf::destroy(pCOutputAudio);
    hOutputFile.close();

    for (int i = 0; i < stFileSpec.iNumChannels; i++)
        delete[] ppfAudioData[i];
    delete[] ppfAudioData;
    ppfAudioData = 0;
    
    for (int i = 0; i < stFileSpec.iNumChannels; i++)
        delete[] ppfOutputData[i];
    delete[] ppfOutputData;
    ppfOutputData = 0;
    
    return 0;

}


void     showClInfo()
{
    cout << "GTCMT MUSI6106 Executable" << endl;
    cout << "(c) 2014-2018 by Alexander Lerch" << endl;
    cout  << endl;

    return;
}

