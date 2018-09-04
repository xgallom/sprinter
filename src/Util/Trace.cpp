/*
 * Trace.cpp
 *
 *  Created on: 4,9, 2018
 *      Author: Zahorack
 */


#include <Util/Trace.h>
#include "Application.h"


void  TRACE(char* fmt,...){
#ifdef	TRACE_ENABLE
	char msg[1000] = {0};
//	const uint8_t mark[10] = {"TRACE: "};
	va_list arg;

    va_start(arg, fmt);
    vsnprintf(msg, sizeof(msg)-1, fmt, arg);
//    usartTrace.write(mark, sizeof(mark));
    App->usartLog.write((uint8_t*)msg, strlen(msg));
    va_end(arg);
#endif
}

void  DTRACE(char* fmt,...){
#ifdef	DTRACE_ENABLE
	char msg[1000] = {0};
	va_list arg;

    va_start(arg, fmt);
    vsnprintf(msg, sizeof(msg)-1, fmt, arg);
    App->usartLog.write((uint8_t*)msg, strlen(msg));
    va_end(arg);
#endif
}
