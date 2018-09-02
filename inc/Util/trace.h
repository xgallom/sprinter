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
#include "Application.h"

#define TRACE_ENABLE


extern void  TRACE(char* fmt,...);


#endif /* UTIL_TRACE_H_ */
