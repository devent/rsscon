/**
 * Copyright 2012 Erwin Müller <erwin.mueller@deventm.org>
 *
 * This file is part of rsscon-shared.
 *
 * rsscon-shared is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * rsscon-shared is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with rsscon-shared. If not, see <http://www.gnu.org/licenses/>.
 */
#include "logger.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>

static int log4c_init_counter = 0;

LOG4C_CATEGORY get_log(const char* name) {
	if (log4c_init_counter == 0) {
		log4c_init();
		log4c_init_counter++;
	}
	return log4c_category_get(name);
}

int free_log() {
	if (log4c_init_counter == 0) {
		int ret = log4c_fini();
		return ret == 0 ? true : false;
	} else {
		log4c_init_counter--;
		return true;
	}
}

void log_vdebug(const LOG4C_CATEGORY category, const char* format, va_list args){
	log4c_category_vlog(category, LOG4C_PRIORITY_DEBUG, format, args);
}

void log_vinfo(const LOG4C_CATEGORY category, const char* format, va_list args){
	log4c_category_vlog(category, LOG4C_PRIORITY_INFO, format, args);
}

void log_verror(const LOG4C_CATEGORY category, const char* format, va_list args){
	log4c_category_vlog(category, LOG4C_PRIORITY_ERROR, format, args);
}

void log_vtrace(const LOG4C_CATEGORY category, const char* format, va_list args){
	log4c_category_vlog(category, LOG4C_PRIORITY_TRACE, format, args);
}

void log_info(const LOG4C_CATEGORY category, const char* format, ...) {
	va_list va;
	va_start(va, format);
	log_vinfo(category, format, va);
	va_end(va);
}

void log_debug(const LOG4C_CATEGORY category, const char* format, ...) {
	va_list va;
	va_start(va, format);
	log_vdebug(category, format, va);
	va_end(va);
}

void log_error(const LOG4C_CATEGORY category, const char* format, ...) {
	va_list va;
	va_start(va, format);
	log_verror(category, format, va);
	va_end(va);
}

void log_enter(const LOG4C_CATEGORY category, const char* name, ...) {
	char buffer[1024];
	va_list va;
	va_start(va, name);
	snprintf(buffer, sizeof(buffer), "%s%s", "enter: ", name);
	log_vtrace(category, buffer, va);
	va_end(va);
}

void log_leave(const LOG4C_CATEGORY category, const char* name, ...) {
	char buffer[1024];
	va_list va;
	va_start(va, name);
	snprintf(buffer, sizeof(buffer), "%s%s", "leave: ", name);
	log_vtrace(category, buffer, va);
	va_end(va);
}
