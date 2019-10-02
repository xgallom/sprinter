//
// Created by xgallom on 10/3/19.
//

#include <cstdint>
#include <core/log.h>

extern "C" {

extern const uintptr_t __stack_chk_guard = 0xdeadbeef;

[[noreturn]]
void __stack_chk_fail()
{
	log("Stack check failed - stack overflow\n");

	for(;;) {}
}

}
