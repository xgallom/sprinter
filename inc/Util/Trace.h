/*
 * Control.h
 *
 *  Created on: 1.9. 2018
 *      Author: Zahorack
 */

#ifndef UTIL_TRACE_H_
#define UTIL_TRACE_H_

#include "stm32f4xx.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>


#define TRACE_ENABLE
//efine DTRACE_ENABLE

#ifdef __cplusplus
extern "C" {
#endif

extern void  TRACE(char* fmt,...);
extern void  DTRACE(char* fmt,...);


#ifdef __cplusplus
}
#endif

#endif /* UTIL_TRACE_H_ */
