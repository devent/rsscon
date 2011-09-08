/**
 * Copyright 2011 Erwin Müller <erwin.mueller@deventm.org>
 * Version 1.1
 *
 * This file is part of rsscon.
 *
 * rsscon is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * rsscon is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with
 * rsscon. If not, see <http://www.gnu.org/licenses/>.
 */
#ifdef RSSCON_LOGSIMPLE

#include "logger.h"
#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {

	const char* categoryName;

} RssconLoggerSimple;

RssconLoggerSimple* loggerGlobal = NULL;

int loggerReferences = 0;

LOG4C_CATEGORY get_log(const char* name) {
	if (loggerReferences == 0) {
		assert(loggerGlobal == NULL);
		loggerGlobal = malloc(sizeof(RssconLoggerSimple));
	}
	loggerReferences++;
	loggerGlobal->categoryName = name;
	return loggerGlobal;
}

int free_log() {
	loggerReferences--;
	if (loggerReferences == 0) {
		assert(loggerGlobal != NULL);
		free(loggerGlobal);
		loggerGlobal = NULL;
	}
	return true;
}

void log_vinfo(const LOG4C_CATEGORY category, const char* format, va_list args){
	RssconLoggerSimple* logger = (RssconLoggerSimple*)category;

	fprintf(stderr, "[info] %s: ", logger->categoryName);
	vfprintf(stderr, format, args);
	fprintf(stderr, "\n");
}

void log_info(const LOG4C_CATEGORY category, const char* format, ...) {
	va_list va;
	va_start(va, format);
	log_vinfo(category, format, va);
	va_end(va);
}

void log_vdebug(const LOG4C_CATEGORY category, const char* format, va_list args){
	RssconLoggerSimple* logger = (RssconLoggerSimple*)category;

	fprintf(stderr, "[debug] %s: ", logger->categoryName);
	vfprintf(stderr, format, args);
	fprintf(stderr, "\n");
}

void log_debug(const LOG4C_CATEGORY category, const char* format, ...) {
	va_list va;
	va_start(va, format);
	log_vdebug(category, format, va);
	va_end(va);
}

void log_enter(const LOG4C_CATEGORY category, const char* name, ...) {
	va_list va;
	va_start(va, name);
	log_vdebug(category, name, va);
	va_end(va);
}

void log_leave(const LOG4C_CATEGORY category, const char* name, ...) {
	va_list va;
	va_start(va, name);
	log_debug(category, name, va);
	va_end(va);
}

void log_verror(const LOG4C_CATEGORY category, const char* format, va_list args){
	RssconLoggerSimple* logger = (RssconLoggerSimple*)category;

	fprintf(stderr, "[error] %s: ", logger->categoryName);
	vfprintf(stderr, format, args);
	fprintf(stderr, "\n");
}

void log_error(const LOG4C_CATEGORY category, const char* format, ...) {
	va_list va;
	va_start(va, format);
	log_verror(category, format, va);
	va_end(va);
}

#endif
