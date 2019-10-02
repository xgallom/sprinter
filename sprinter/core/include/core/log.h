//
// Created by xgallom on 10/2/19.
//

#ifndef SPRINTER_SPRINTER_CORE_INCLUDE_CORE_LOG_H
#define SPRINTER_SPRINTER_CORE_INCLUDE_CORE_LOG_H

#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace core {
	void log(const char *message);
	void log(const char *message, size_t length);

	namespace impl {
		template<typename T>
		inline char hex(T value)
		{
			const uint8_t v = uint8_t(value) & 0x0fu;
			return v > 0x09 ? v - 0x0a + 'a' : v + '0';
		}

		template<size_t Size>
		void logHex(uint64_t value)
		{
			static constexpr size_t Count = Size * 2;
			char buf[Count];
			for(size_t n = 1; n <= Count; ++n) {
				buf[Count - n] = hex(value);
				value >>= 4u;
			}
			core::log(buf, Count);
		}
	}

	template<typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
	void log(T value) { impl::logHex<sizeof(value)>(value); }
}

template<typename ... Args>
void log(Args ... args) { (core::log(args), ...); }

#endif //SPRINTER_SPRINTER_CORE_INCLUDE_CORE_LOG_H
