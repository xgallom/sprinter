/*
 * State.h
 *
 *  Created on: Jun 20, 2018
 *      Author: xgallom
 */

#ifndef UTIL_STATE_H_
#define UTIL_STATE_H_

namespace Util {

struct StateMask {
	StateMask(uint8_t offset, uint8_t mask) : offset(offset), mask(mask) {}

	uint8_t offset;
	uint8_t mask;
};

template<typename T>
struct State {
	T m_stateVariable;

	State(T initialState = 0) : m_stateVariable(initialState) {}

	bool flag(uint8_t offset) const { return m_stateVariable & (1 << offset); }
	void setFlag(uint8_t offset) { m_stateVariable |= 1 << offset; }
	void setFlagTo(uint8_t offset, bool value) { if(value) setFlag(offset); else resetFlag(offset); }
	void resetFlag(uint8_t offset) { m_stateVariable &= ~(1 << offset); }
	void toggleFlag(uint8_t offset) { m_stateVariable ^= 1 << offset; }

	T mask(StateMask stateMask) const { return (m_stateVariable >> stateMask.offset) & stateMask.mask; }
	void setMask(StateMask stateMask, T value) {
		m_stateVariable = (m_stateVariable & ~(stateMask.mask << stateMask.offset)) | (value << stateMask.offset);
	}
};

} /* namespace Util */

#endif /* UTIL_STATE_H_ */
