
#include <iostream>
#include <ctime>
#include <stdlib.h>
#include <fstream>

#include "MUSI6106Config.h"

#include "AudioFileIf.h"

using std::cout;
using std::endl;
using std::fstream;

// local function declarations
void    showClInfo ();

/////////////////////////////////////////////////////////////////////////////////
// main function
int main(int argc, char* argv[])
{
    std::string             sInputFilePath,                 //!< file paths
                            sOutputFilePath;

    long long               iInFileLength       = 0;        //!< length of input file
    
    long long               iNumFrames          = 1024;

    clock_t                 time                = 0;

    float                   **ppfAudioData      = 0;

    CAudioFileIf            *phAudioFile        = 0;

    showClInfo ();

    //////////////////////////////////////////////////////////////////////////////
    // parse command line arguments
    sInputFilePath = argv[1];
    sOutputFilePath = argv[2];

    //////////////////////////////////////////////////////////////////////////////
    // open the input wave file and output text file
    CAudioFileIf::create(phAudioFile);
    phAudioFile->openFile(sInputFilePath, CAudioFileIf::kFileRead);
    fstream fs;
    fs.open(sOutputFilePath);
    
    //////////////////////////////////////////////////////////////////////////////
    // allocate memory
    time                    = clock();
    
    CAudioFileIf::FileSpec_t sFileSpec;
    phAudioFile->getFileSpec(sFileSpec);
    phAudioFile->getLength(iInFileLength);
    
    ppfAudioData = (float**)malloc(sFileSpec.iNumChannels);
    for(int i=0; i<sFileSpec.iNumChannels; i++){
        ppfAudioData[i] = (float*)malloc(iNumFrames);
    }
    

    // get audio data and write it to the output file
    while(!phAudioFile->isEof()){
        phAudioFile->readData(ppfAudioData, iNumFrames);
        
        for(int i=0; i<iNumFrames; i++){
            fs<<ppfAudioData[0][i]<<", "<<ppfAudioData[1][i]<<endl;
        }
    }

    fs.close();
    cout << "reading/writing done in: \t"    << (clock()-time)*1.F/CLOCKS_PER_SEC << " seconds." << endl;
    //////////////////////////////////////////////////////////////////////////////
    // get audio info and print it to stdout
    cout<<"Sample Rate (Hz): "<<sFileSpec.fSampleRateInHz<<endl;
    cout<<"Num Channels: "<<sFileSpec.iNumChannels<<endl;

    //////////////////////////////////////////////////////////////////////////////
    // do processing
    cout << "Hello there!" << endl << endl;

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

