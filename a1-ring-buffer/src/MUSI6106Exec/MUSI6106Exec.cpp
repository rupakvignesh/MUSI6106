
#include <iostream>
#include <ctime>

#include "MUSI6106Config.h"

#include "AudioFileIf.h"
#include "RingBuffer.h"

using std::cout;
using std::endl;

// local function declarations
void    showClInfo();

void testFillBuffer(CRingBuffer<float> *pCRingBuffer, float* pfTestSignal, int kBlockSize) {
	for (int i = 0;i<kBlockSize;i++) {
		pCRingBuffer->putPostInc(pfTestSignal[i]);
	}
	int sum = 0;
	for (int i = 0;i<3 * kBlockSize;i++) {
		if (pCRingBuffer->getPostInc() == pfTestSignal[i%kBlockSize]) sum += 1;
	}
	if (sum == 3 * kBlockSize) {
		cout << "Fill buffer test case Passed" << endl;
	}
	else {
		cout << "Test case failed" << endl;
	}
}

void testMethods(CRingBuffer<float> *pCRingBuffer, float* pfTestSignal, int kBlockSize) {
	int prod = 1;
	pCRingBuffer->reset();                                      //w=0, r=0
	if (pCRingBuffer->getWriteIdx() != 0) prod = 0;              //w=0
	pCRingBuffer->put(521);
	if (pCRingBuffer->getReadIdx() != 0) prod = 0;               //r=0
	pCRingBuffer->setReadIdx(pCRingBuffer->getWriteIdx());      //r=0
	if (pCRingBuffer->get(0) != 521) prod = 0;                           //521
	pCRingBuffer->setWriteIdx(kBlockSize - 1);
	pCRingBuffer->setReadIdx(kBlockSize - 1);
	if (pCRingBuffer->getReadIdx() != kBlockSize - 1 && pCRingBuffer->getWriteIdx() != kBlockSize - 1) prod = 0;      //r=kBlockSize-1
	if (prod == 1) {
		cout << "Test cases for methods passed" << endl;
	}
	else {
		cout << "Test case failed" << endl;
	}

}

void testEmptyFull(CRingBuffer<float> *pCRingBuffer, float* pfTestSignal, int kBlockSize) {
	pCRingBuffer->reset();
	int prod = 1;
	if (pCRingBuffer->getNumValuesInBuffer() != 0) {            //Empty Buffer
		prod = 0;
	}
	for (int i = 0;i<kBlockSize;i++) {                          //Fill in values
		pCRingBuffer->putPostInc(pfTestSignal[i]);
	}
	if (pCRingBuffer->getNumValuesInBuffer() != kBlockSize) {   //Buffer full
		prod = 0;
	}

	if (prod == 1) {
		cout << "Test case Empty/Full buffer Passed" << endl;
	}
	else {
		cout << "Test case Empty/Full buffer failed" << endl;
	}

}

void testOffsetNegative(CRingBuffer<float> *pCRingBuffer, float* pfTestSignal, int kBlockSize) {
	pCRingBuffer->reset();
	for (int i = 0; i<kBlockSize; i++) {                          //Fill in values
		pCRingBuffer->putPostInc(pfTestSignal[i]);
	}
	int flag = 0;
	for (int i = 0; i<kBlockSize; i++) {
		if (pCRingBuffer->get(-i - 1) != pfTestSignal[kBlockSize - i - 1]) {
			flag = 1;
			break;
		}
	}

	if (flag == 0) {
		cout << "Test case offset negative passed" << endl;
	}
	else {
		cout << "Test case offset negative failed" << endl;
	}

}

void testIndexCrossing(CRingBuffer<float> *pCRingBuffer, float* pfTestSignal, int kBlockSize) {
	pCRingBuffer->reset();
	for (int i = 0; i<kBlockSize; i++) {                          //Fill in values
		pCRingBuffer->putPostInc(pfTestSignal[i]);
	}


	pCRingBuffer->putPostInc(0);                                 //Should give a warning
}



/////////////////////////////////////////////////////////////////////////////////
// main function
int main(int argc, char* argv[])
{
	// You don't need to use an audio file this time, though you can try if you want.
	// std::string             sInputFilePath,                 //!< file paths
	//                         sOutputFilePath;
	static const int        kBlockSize = 8;
	// float                   **ppfAudioData      = 0;
	// CAudioFileIf            *phAudioFile        = 0;
	// CAudioFileIf::FileSpec_t stFileSpec;
	// long long               iInFileLength       = 0;        //!< length of input file

	clock_t                 time = 0;

	CRingBuffer<float>      *pCRingBuffer = 0;
	float                   *pfTestSignal = 0;
	//float                   fGain               = 1.f;

	showClInfo();

	//////////////////////////////////////////////////////////////////////////////
	// allocate memory
	time = clock();
	pCRingBuffer = new CRingBuffer<float>(kBlockSize);

	// fill the test signal (e.g., a unit impulse)
	pfTestSignal = new float[kBlockSize];
	pfTestSignal[0] = 1;                            // Unit impulse
	for (int i = 1;i<kBlockSize;i++) {
		pfTestSignal[i] = 0;
	}
	//////////////////////////////////////////////////////////////////////////////
	// do processing and tests

	//Test case I. Fill the buffer with test signal and output values.
	cout << "Test case I. Fill the buffer with test signal and check circular functionality " << endl;
	testFillBuffer(pCRingBuffer, pfTestSignal, kBlockSize);

	//Test case II. Function calls.
	cout << "Test case II. Function calls." << endl;
	testMethods(pCRingBuffer, pfTestSignal, kBlockSize);

	//Test case III. Empty and full buffers
	cout << "Test case III. Empty and Full buffers." << endl;
	testEmptyFull(pCRingBuffer, pfTestSignal, kBlockSize);

	//Test case IV. Negative offsets.
	cout << "Test case IV. Negative offsets" << endl;
	testOffsetNegative(pCRingBuffer, pfTestSignal, kBlockSize);

	//Test case V. Read write index crossing.
	cout << "Test case V. Read write index crossing" << endl;
	testIndexCrossing(pCRingBuffer, pfTestSignal, kBlockSize);

	cout << "processing done in: \t" << (clock() - time)*1.F / CLOCKS_PER_SEC << " seconds." << endl;

	//////////////////////////////////////////////////////////////////////////////
	// clean-up
	delete pCRingBuffer;

	return 0;
}


void     showClInfo()
{
	cout << "GTCMT MUSI6106 Executable" << endl;
	cout << "(c) 2014-2018 by Alexander Lerch" << endl;
	cout << endl;

	return;
}

