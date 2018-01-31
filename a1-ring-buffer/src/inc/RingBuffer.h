#if !defined(__RingBuffer_hdr__)
#define __RingBuffer_hdr__

#include <cassert>
#include <algorithm>
#include <cmath>

/*! \brief implement a circular buffer of type T
*/
template <class T>
class CRingBuffer
{
public:
	explicit CRingBuffer(int iBufferLengthInSamples) :
		m_iBuffLength(iBufferLengthInSamples),
		m_iReadIdx(0),
		m_iWriteIdx(0),
		m_ptBuff(0)
	{
		assert(iBufferLengthInSamples > 0);

		m_ptBuff = new T[m_iBuffLength];
		reset();
	}

	virtual ~CRingBuffer()
	{
		delete[] m_ptBuff;
		m_ptBuff = 0;
	}

	/*! add a new value of type T to write index
	\param tNewValue the new value
	\return void
	*/
	void put(T tNewValue)
	{
		m_ptBuff[m_iWriteIdx] = tNewValue;
	}

	/*! return the value at the current read index
	\return type T the value from the read index
	*/
	//    T get () const
	//    {
	//        return m_ptBuff[m_iReadIdx];
	//    }

	/*! return the current index for writing/put
	\return int
	*/
	int getWriteIdx() const
	{
		return m_iWriteIdx;
	}

	/*! move the write index to a new position
	\param iNewWriteIdx: new position
	\return void
	*/
	void setWriteIdx(int iNewWriteIdx)
	{
		m_iWriteIdx = iNewWriteIdx;
		int flag = checkReadWrite();
		if (flag == 1) {
			std::cout << "Check read write indices. They can't take negative values, write index should not over take read index, and read cannot be greater than write "<<std::endl;

		}

	}

	/*! return the current index for reading/get
	\return int
	*/
	int getReadIdx() const
	{
		return m_iReadIdx;
	}

	/*! move the read index to a new position
	\param iNewReadIdx: new position
	\return void
	*/
	void setReadIdx(int iNewReadIdx)
	{
		m_iReadIdx = iNewReadIdx;
		int flag = checkReadWrite();
		if (flag == 1) {
			std::cout << "Check read write indices. They can't take negative values, write index should not over take read index, and read cannot be greater than write "<<std::endl;
		}

	}

	/*! add a new value of type T to write index and increment write index
	\param tNewValue the new value
	\return void
	*/
	void putPostInc(T tNewValue)
	{
		m_ptBuff[mod(m_iWriteIdx++, m_iBuffLength)] = tNewValue;
		int flag = checkReadWrite();
		if (flag == 1) {
			std::cout << "Check read write indices. They can't take negative values, write index should not over take read index, and read cannot be greater than write "<<std::endl;
		}

	}

	/*! return the value at the current read index and increment the read pointer
	\return type T the value from the read index
	*/
	T getPostInc()
	{
		return m_ptBuff[mod(m_iReadIdx++, m_iBuffLength)];
		int flag = checkReadWrite();
		if (flag == 1) {
			std::cout << "Check read write indices. They can't take negative values, write index should not over take read index, and read cannot be greater than write "<<std::endl;
		}

	}

	/*! return the value at the index with an arbitrary offset
	\param iOffset: read at offset from read index
	\return type T the value from the read index
	*/
	T get(int iOffset = 0) const
	{
		return m_ptBuff[mod(m_iReadIdx + iOffset, m_iBuffLength)];
	}

	/*! set buffer content and indices to 0
	\return void
	*/
	void reset()
	{
		memset(m_ptBuff, 0, sizeof(T)*m_iBuffLength);
		m_iReadIdx = 0;
		m_iWriteIdx = 0;
	}

	/*! returns the number of values currently buffered (note: 0 could also mean the buffer is full!)
	\return int
	*/
	int getNumValuesInBuffer() const
	{
		return m_iWriteIdx - m_iReadIdx;      //(m_iWriteIdx - m_iReadIdx + m_iBuffLength)%m_iBuffLength;
	}

	/*! returns the length of the internal buffer
	\return int
	*/
	int getLength() const
	{
		return m_iBuffLength;
	}
private:
	CRingBuffer();
	CRingBuffer(const CRingBuffer& that);

    int m_iBuffLength;              //!< length of the internal buffer
    int	m_iReadIdx,                 //!< current read index
		m_iWriteIdx;                //!< current write index

	T   *m_ptBuff;                  //!< data buffer

	int mod(int a, int b) const {
		int c = a%b;
		return (c<0) ? c + b : c;
	}
	//r,w should not be negative.
	//w should not overcross read.
	int checkReadWrite() {
		int flag = 0;
		if ((m_iReadIdx < 0) || (m_iWriteIdx < 0) || (m_iWriteIdx - m_iReadIdx > m_iBuffLength) || (m_iWriteIdx - m_iReadIdx < 0)) {
			flag = 1;
		}
		return flag;
	}

};
#endif // __RingBuffer_hdr__
