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
		[[nodiscard]] inline volatile bool operator==(const Time &o) const { return value == o.value; }
		[[nodiscard]] inline volatile bool operator!=(const Time &o) const { return value != o.value; }
		[[nodiscard]] inline volatile bool operator<(const Time &o) const { return value < o.value; }
		[[nodiscard]] inline volatile bool operator<=(const Time &o) const { return value <= o.value; }
		[[nodiscard]] inline volatile bool operator>(const Time &o) const { return value > o.value; }
		[[nodiscard]] inline volatile bool operator>=(const Time &o) const { return value >= o.value; }

		[[nodiscard]] static Time Now();
		[[nodiscard]] static constexpr Time Millis(uint32_t x) { return Time(x); }
		[[nodiscard]] static constexpr Time Seconds(uint32_t x) { return Millis(x * 1000u); }
		[[nodiscard]] static constexpr Time SecondsF(float x) { return Millis(x * 1000u); }

		inline Time() = default;
	private:
		explicit constexpr Time(uint32_t value) : value(value) {}

		uint32_t value = {};
	};

	[[nodiscard]] inline Time operator+(Time l, const Time &r) { return l += r; }
}

#endif //SPRINTER_SPRINTER_CORE_INCLUDE_CORE_TIME_H
