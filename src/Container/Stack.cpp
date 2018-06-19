/*
 * Stack.cpp
 *
 *  Created on: Jun 19, 2018
 *      Author: xgallom
 */

#include "Stack.h"

namespace Container {

template<typename T>
WrapperStack<T>::WrapperStack(T *data, uint32_t dataSize) :
	m_bufferBegin(data),
	m_bufferSize(dataSize),
	m_stackSize(0)
{}

template<typename T>
bool WrapperStack<T>::push(T entry)
{
	if(m_stackSize == m_bufferSize -1)
		return false;

	m_bufferBegin[m_stackSize++] = entry;

	return true;
}

template<typename T>
typename WrapperStack<T>::OperationResult WrapperStack<T>::pop()
{
	if(m_stackSize == 0)
		return OperationResult();

	return OperationResult(m_bufferBegin[--m_stackSize]);
}

template<typename T>
typename WrapperStack<T>::OperationResult WrapperStack<T>::peek(uint32_t howFar) const
{
	if(howFar < m_stackSize)
		return OperationResult();

	return OperationResult(m_bufferBegin[m_stackSize - howFar]);
}

template class WrapperStack<volatile uint8_t>;

} /* namespace Container */
