//
// Created by xgallom on 10/3/19.
//

#include <cstdint>
#include <core/fatal.h>

extern "C" {

extern const uintptr_t __stack_chk_guard = 0xe2dee396;

[[noreturn]] void __stack_chk_fail()
{
	fatal("Stack check failed - stack overflow\n");
}

}
