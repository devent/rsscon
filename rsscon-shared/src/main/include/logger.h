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
#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdarg.h>
#include <log4c.h>

#define LOG4C_CATEGORY log4c_category_t *

LOG4C_CATEGORY get_log(const char* name);

int free_log();

void log_info(const LOG4C_CATEGORY category, const char* format, ...);

void log_debug(const LOG4C_CATEGORY category, const char* format, ...);

void log_error(const LOG4C_CATEGORY category, const char* format, ...);

void log_trace(const LOG4C_CATEGORY category, const char* format, ...);

void log_enter(const LOG4C_CATEGORY category, const char* name, ...);

void log_leave(const LOG4C_CATEGORY category, const char* name, ...);


#endif /* LOGGER_H_ */
