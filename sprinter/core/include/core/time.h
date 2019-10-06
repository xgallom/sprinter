//
// Created by xgallom on 10/5/19.
//

#ifndef SPRINTER_SPRINTER_CORE_INCLUDE_CORE_TIME_H
#define SPRINTER_SPRINTER_CORE_INCLUDE_CORE_TIME_H

#include <cstdint>

namespace core {
	class Time {
	public:
		inline Time &operator+=(const Time &o)
		{
			value += o.value;
			return *this;
		}
		inline bool operator==(const Time &o) const { return value == o.value; }
		inline bool operator!=(const Time &o) const { return value != o.value; }
		inline bool operator<(const Time &o) const { return value < o.value; }
		inline bool operator<=(const Time &o) const { return value <= o.value; }
		inline bool operator>(const Time &o) const { return value > o.value; }
		inline bool operator>=(const Time &o) const { return value >= o.value; }

		static Time Now();
		static constexpr Time Millis(uint32_t x) { return Time(x); }
		static constexpr Time Seconds(uint32_t x) { return Millis(x * 1000u); }
		static constexpr Time SecondsF(float x) { return Millis(x * 1000u); }

		inline Time() = default;
	private:
		explicit constexpr Time(uint32_t value) : value(value) {}

		uint32_t value = {};
	};

	inline Time &operator+(Time l, const Time &r) { return l += r; }
}

#endif //SPRINTER_SPRINTER_CORE_INCLUDE_CORE_TIME_H
