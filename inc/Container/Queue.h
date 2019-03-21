/*
 * Queue.h
 *
 *  Created on: Jun 20, 2018
 *      Author: xgallom
 */

#ifndef CONTAINER_QUEUE_H_
#define CONTAINER_QUEUE_H_

#include <Container/Result.h>
#include "stm32f4xx.h"

namespace Container
{
	template<typename T>
	class WrapperQueue {
		T *m_bufferBegin;
		uint32_t m_bufferSize;

		volatile uint32_t m_queueStart, m_queueEnd;

	public:
		WrapperQueue(T *buffer, uint32_t bufferSize);

		bool enqueue(T entry);

		Result<T> dequeue();
		Result<T> peek(uint32_t howFar = 1) const;

		inline uint32_t size() const {
			return m_queueEnd >= m_queueStart ?
					m_queueEnd - m_queueStart :
					m_queueEnd + m_bufferSize - m_queueStart;
		}
		inline bool isEmpty() const { return m_queueStart == m_queueEnd; }
	};

	template<typename T, uint32_t Size>
	class Queue : public WrapperQueue<T> {
		T m_internalBuffer[Size];

	public:
		explicit Queue() : WrapperQueue<T>(m_internalBuffer, Size) {}
	};
}

#endif /* CONTAINER_QUEUE_H_ */
