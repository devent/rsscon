/**
 * Copyright 2010 Erwin Müller <erwin.mueller@deventm.org>
 * Version 1.0
 *
 * Defines wrapper functions for using with log4c.
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
#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdarg.h>

#ifndef RSSCON_NOLOG4C
#include <log4c.h>
#endif

#ifndef RSSCON_NOLOG4C
#define LOG4C_CATEGORY log4c_category_t *
#else
#define LOG4C_CATEGORY void *
#endif

LOG4C_CATEGORY get_log(const char* name);

int free_log();

void log_vdebug(const LOG4C_CATEGORY category, const char* format, va_list args);

void log_debug(const LOG4C_CATEGORY category, const char* format, ...);

void log_enter(const LOG4C_CATEGORY category, const char* name, ...);

void log_leave(const LOG4C_CATEGORY category, const char* name, ...);


#endif /* LOGGER_H_ */
