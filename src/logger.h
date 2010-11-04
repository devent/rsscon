/**
 * Copyright 2010 Erwin Mueller, <erwin.mueller@deventm.org>
 * Version 1.0
 *
 * Defines wrapper functions for using with log4c.
 */
#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdarg.h>
#include <log4c.h>

log4c_category_t *get_log(const char* name);

int free_log();

void log_vdebug(const log4c_category_t *category, const char* format, va_list args);

void log_debug(const log4c_category_t *category, const char* format, ...);

void log_enter(const log4c_category_t *category, const char* name, ...);

void log_leave(const log4c_category_t *category, const char* name, ...);


#endif /* LOGGER_H_ */
