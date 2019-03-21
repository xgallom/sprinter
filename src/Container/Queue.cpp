/*
 * Queue.cpp
 *
 *  Created on: Jun 20, 2018
 *      Author: xgallom
 */

#include "Container/Queue.h"

namespace Container
{

	template<typename T>
	WrapperQueue<T>::WrapperQueue(T *buffer, uint32_t bufferSize) :
		m_bufferBegin(buffer),
		m_bufferSize(bufferSize),
		m_queueStart(0),
		m_queueEnd(0)
	{}

	template<typename T>
	bool WrapperQueue<T>::enqueue(T entry)
	{
		if(m_queueEnd == m_queueStart -1)
			return false;

		m_bufferBegin[m_queueEnd++] = entry;

		if(m_queueEnd == m_bufferSize)
			m_queueEnd = 0;

		return true;
	}

	template<typename T>
	Result<T> WrapperQueue<T>::dequeue()
	{
		if(isEmpty())
			return Result<T>();

		Result<T> rv(m_bufferBegin[m_queueStart++]);

		if(m_queueStart == m_bufferSize)
			m_queueStart = 0;

		return rv;
	}

	template<typename T>
	Result<T> WrapperQueue<T>::peek(uint32_t howFar) const
	{
		if(howFar > size())
			return Result<T>();

		howFar += m_queueStart;

		if(howFar > m_bufferSize)
			howFar -= m_bufferSize;

		return Result<T>(m_bufferBegin[howFar]);
	}

	template class WrapperQueue<volatile uint8_t>;
}
