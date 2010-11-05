/**
 * Copyright 2010 Erwin Müller <erwin.mueller@deventm.org>
 * Version 1.0
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
#include <assert.h>
#include <string.h>
#include "rsscon.h"
#include "logger.h"

typedef struct {

	/**
	 * True when the port is open and ready to read/write, False otherwise.
	 */
	bool open;

	/**
	 * The name of the device. Under Linux it's /dev/ttyUSB0.
	 */
	char device[256];

	/**
	 * The input and output baud rate of the device.
	 */
	unsigned int baudRate;

	/**
	 * Saves the last error.
	 */
	int lastError;

} RssconPrivate;

#define LOG_CATEGORY "com.globalscalingsoftware.rsscon"

Rsscon* rssconCreate(const char* device, unsigned int baudRate) {
	log4c_category_t *log = get_log(LOG_CATEGORY);
	log_enter(log, "rssconCreate('%s', %d)", device, baudRate);

	log_debug(log, "allocate rsscon data structure...");
	Rsscon* rsscon = malloc(sizeof(Rsscon));
	if (rsscon == NULL) {
		return NULL;
	}

	log_debug(log, "allocate rsscon private data structure...");
	RssconPrivate* private = malloc(sizeof(RssconPrivate));
	if (private == NULL) {
		return NULL;
	}

	rsscon->rssconClose = NULL;
	rsscon->rssconFree = NULL;
	rsscon->rssconInit = NULL;
	rsscon->rssconOpen = NULL;
	rsscon->rssconRead = NULL;
	rsscon->rssconWrite = NULL;

	rsscon->portdata = NULL;
	rsscon->private = private;
	strncpy(private->device, device, strlen(device) + 1);
	private->baudRate = baudRate;
	private->open = false;
	private->lastError = RSSCON_ERROR_NOERROR;

	log_leave(log, "leave rssconCreate:=%d", rsscon);
	free_log();
	return rsscon;
}

bool freeChildIfChildWasSet(Rsscon* rsscon) {
	if (rsscon->rssconFree != NULL) {
		return rsscon->rssconFree(rsscon);
	}
	return true;
}

bool rssconFree(Rsscon* rsscon) {
	log4c_category_t *log = get_log(LOG_CATEGORY);
	log_enter(log, "rssconFree(%d)", rsscon);

	assert(rsscon != NULL);
	assert(rsscon->private != NULL);

	if (!freeChildIfChildWasSet(rsscon)) {
		log_leave(log, "leave rssconFree:=%d", false);
		return false;
	}

	log_debug(log, "free private and rsscon structure...");
	RssconPrivate* private = (RssconPrivate*) rsscon->private;
	free(private);
	free(rsscon);

	log_leave(log, "leave rssconFree:=true");
	return true;
}

bool rssconOpen(Rsscon* rsscon) {
	log4c_category_t *log = get_log(LOG_CATEGORY);
	log_enter(log, "rssconOpen(%d)", rsscon);

	assert(rsscon != NULL);
	assert(rsscon->rssconOpen != NULL);
	return rsscon->rssconOpen(rsscon);
}

bool rssconClose(Rsscon* rsscon) {
	log4c_category_t *log = get_log(LOG_CATEGORY);
	log_enter(log, "rssconClose(%d)", rsscon);

	assert(rsscon != NULL);
	assert(rsscon->rssconClose != NULL);
	return rsscon->rssconClose(rsscon);
}

bool rssconWrite(Rsscon* rsscon, const void* data, size_t length, size_t* wrote) {
	log4c_category_t *log = get_log(LOG_CATEGORY);
	log_enter(log, "rssconWrite(%d, '%s', %d, %d)", rsscon, data, length, wrote);

	assert(rsscon != NULL);
	assert(rsscon->rssconWrite != NULL);
	return rsscon->rssconWrite(rsscon, data, length, wrote);
}

bool rssconRead(Rsscon* rsscon, void* data, size_t length, size_t* red) {
	log4c_category_t *log = get_log(LOG_CATEGORY);
	log_enter(log, "rssconRead(%d, '%s', %d, %d)", rsscon, data, length, red);

	assert(rsscon != NULL);
	assert(rsscon->rssconRead != NULL);
	return rsscon->rssconRead(rsscon, data, length, red);
}

void rssconSetLastError(Rsscon* rsscon, int lastError) {
	log4c_category_t *log = get_log(LOG_CATEGORY);
	log_enter(log, "rssconSetLastError(%d)", rsscon);

	assert(rsscon != NULL);
	assert(rsscon->private != NULL);
	RssconPrivate* private = (RssconPrivate*) rsscon->private;
private->lastError = lastError;
}

int rssconGetLastError(Rsscon* rsscon) {
	log4c_category_t *log = get_log(LOG_CATEGORY);
	log_enter(log, "rssconGetLastError(%d)", rsscon);

	assert(rsscon != NULL);
	assert(rsscon->private != NULL);
	RssconPrivate* private = (RssconPrivate*) rsscon->private;
	return private->lastError;
}

bool rssconIsOpen(Rsscon* rsscon) {
	log4c_category_t *log = get_log(LOG_CATEGORY);
	log_enter(log, "rssconIsOpen(%d)", rsscon);

	assert(rsscon != NULL);
	assert(rsscon->private != NULL);
	RssconPrivate* private = (RssconPrivate*) rsscon->private;
	return private->open;
}

const char* rssconGetDevice(Rsscon* rsscon) {
	log4c_category_t *log = get_log(LOG_CATEGORY);
	log_enter(log, "rssconGetDevice(%d)", rsscon);

	assert(rsscon != NULL);
	assert(rsscon->private != NULL);
	RssconPrivate* private = (RssconPrivate*) rsscon->private;
	return private->device;
}

unsigned int rssconGetBaudRate(Rsscon* rsscon) {
	log4c_category_t *log = get_log(LOG_CATEGORY);
	log_enter(log, "rssconGetBaudRate(%d)", rsscon);

	assert(rsscon != NULL);
	assert(rsscon->private != NULL);
	RssconPrivate* private = (RssconPrivate*) rsscon->private;
	return private->baudRate;
}
