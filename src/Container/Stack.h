/*
 * Stack.h
 *
 *  Created on: Jun 19, 2018
 *      Author: xgallom
 */

#ifndef CONTAINER_STACK_H_
#define CONTAINER_STACK_H_

#include "stm32f4xx.h"
#include "OperationResult.h"

namespace Container {

template<typename T>
class WrapperStack {
	T *m_bufferBegin;
	uint32_t m_bufferSize;
	volatile uint32_t m_stackSize;

public:
	explicit WrapperStack(T *buffer, uint32_t bufferSize);

	bool push(T entry);

	OperationResult<T> pop();
	OperationResult<T> peek(uint32_t howFar = 1) const;

	inline uint32_t size() const { return m_stackSize; }
	inline bool isEmpty() const { return m_stackSize == 0; }
};

template<typename T, uint32_t Size>
class Stack : public WrapperStack<T> {
	T m_internalBuffer[Size];

public:
	explicit Stack() : WrapperStack<T>(m_internalBuffer, Size) {}
};

} /* namespace Container */

#endif /* CONTAINER_STACK_H_ */
