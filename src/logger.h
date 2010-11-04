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
#include <log4c.h>

log4c_category_t *get_log(const char* name);

int free_log();

void log_vdebug(const log4c_category_t *category, const char* format, va_list args);

void log_debug(const log4c_category_t *category, const char* format, ...);

void log_enter(const log4c_category_t *category, const char* name, ...);

void log_leave(const log4c_category_t *category, const char* name, ...);


#endif /* LOGGER_H_ */
