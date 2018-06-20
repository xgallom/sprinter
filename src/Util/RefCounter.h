/*
 * RefCounter.h
 *
 *  Created on: Jun 20, 2018
 *      Author: xgallom
 */

#ifndef REFCOUNTER_H_
#define REFCOUNTER_H_

namespace Util {

class RefCounter {
	uint32_t m_refCount = 0;

public:
	void operator++() { m_refCount++; }
	void operator--() { m_refCount--; }

	bool referenceExists() const { return m_refCount; }
	bool destroyReference() const { return !m_refCount; }
};

} /* namespace Util */

#endif /* REFCOUNTER_H_ */
