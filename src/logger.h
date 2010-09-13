#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdarg.h>
#include <log4c.h>

log4c_category_t *get_log(const char* name) {
	log4c_init();
	return log4c_category_get(name);
}

int free_log() {
	return log4c_fini();
}

void log_vdebug(const log4c_category_t *category, const char* format, va_list args){
	log4c_category_vlog(category, LOG4C_PRIORITY_DEBUG, format, args);
}

void log_debug(const log4c_category_t *category, const char* format, ...) {
	va_list va;
	va_start(va, format);
	log_vdebug(category, format, va);
	va_end(va);
}

void log_enter(const log4c_category_t *category, const char* name, ...) {
	va_list va;
	va_start(va, name);
	log_vdebug(category, name, va);
	va_end(va);
}

void log_leave(const log4c_category_t *category, const char* name, ...) {
	va_list va;
	va_start(va, name);
	log_debug(category, name, va);
	va_end(va);
}


#endif /* LOGGER_H_ */
