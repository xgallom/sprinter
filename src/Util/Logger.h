/*
 * Logger.h
 *
 *  Created on: Jun 22, 2018
 *      Author: xgallom
 */

#ifndef UTIL_LOGGER_H_
#define UTIL_LOGGER_H_

#include "../Periph/Usart.h"

namespace Util {

class Logger {
	Periph::Usart *m_usart;

public:
	Logger(Periph::Usart *usart);

	void write(const char module[], const char message[]);

	static Logger *instance();
};

} /* namespace Util */

#define LOGGER_MODULE(MOD) static const char module[] = #MOD;

#define LOG(MODULE, MSG) ::Util::Logger *logger = ::Util::Logger::instance(); if(logger) logger->write(MODULE, MSG);

#define DBG_LOG(MSG) do { LOG(module, ": DEBUG: " MSG "\n") } while(false)
#define INF_LOG(MSG) do { LOG(module, ": INFO_: " MSG "\n") } while(false)
#define WRN_LOG(MSG) do { LOG(module, ": WARN_: " MSG "\n") } while(false)
#define ERR_LOG(MSG) do { LOG(module, ": ERROR: " MSG "\n") } while(false)

#endif /* UTIL_LOGGER_H_ */
