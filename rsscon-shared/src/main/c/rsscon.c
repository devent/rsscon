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

#define LOG_CATEGORY "com.anrisoftware.rsscon.rsscon"

Rsscon* rssconCreate(const char* device, unsigned int baudRate) {
	LOG4C_CATEGORY log = get_log(LOG_CATEGORY);
	log_info(log, "create the device '%s' with the baud rate %d.", device, baudRate);

	log_debug(log, "allocate rsscon data structure.");
	Rsscon* rsscon = malloc(sizeof(Rsscon));
	if (rsscon == NULL) {
		free_log();
		return NULL;
	}

	log_debug(log, "allocate rsscon private data structure.");
	RssconPrivate* private = malloc(sizeof(RssconPrivate));
	if (private == NULL) {
		free_log();
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

	log_debug(log, "finish creating device '%s'.", device);
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
	LOG4C_CATEGORY log = get_log(LOG_CATEGORY);

	assert(rsscon != NULL);
	assert(rsscon->private != NULL);

	log_debug(log, "free data from implementation.");
	if (!freeChildIfChildWasSet(rsscon)) {
		log_leave(log, "leave rssconFree:=%d", false);
		free_log();
		return false;
	}

	log_debug(log, "free private and rsscon structure.");
	RssconPrivate* private = (RssconPrivate*) rsscon->private;
	free(private);
	rsscon->private = NULL;
	free(rsscon);

	free_log();
	return true;
}

bool rssconOpen(Rsscon* rsscon) {
	assert(rsscon != NULL);
	assert(rsscon->rssconOpen != NULL);
	return rsscon->rssconOpen(rsscon);
}

bool rssconClose(Rsscon* rsscon) {
	assert(rsscon != NULL);
	assert(rsscon->rssconClose != NULL);
	return rsscon->rssconClose(rsscon);
}

bool rssconWrite(Rsscon* rsscon, const void* data, size_t length, size_t* wrote) {
	assert(rsscon != NULL);
	assert(rsscon->rssconWrite != NULL);
	return rsscon->rssconWrite(rsscon, data, length, wrote);
}

bool rssconRead(Rsscon* rsscon, void* data, size_t length, size_t* red) {
	assert(rsscon != NULL);
	assert(rsscon->rssconRead != NULL);
	return rsscon->rssconRead(rsscon, data, length, red);
}

void rssconSetLastError(Rsscon* rsscon, int lastError) {
	assert(rsscon != NULL);
	assert(rsscon->private != NULL);
	RssconPrivate* private = (RssconPrivate*) rsscon->private;
	private->lastError = lastError;
}

int rssconGetLastError(Rsscon* rsscon) {
	assert(rsscon != NULL);
	assert(rsscon->private != NULL);
	RssconPrivate* private = (RssconPrivate*) rsscon->private;
	return private->lastError;
}

bool rssconIsOpen(Rsscon* rsscon) {
	assert(rsscon != NULL);
	assert(rsscon->private != NULL);
	RssconPrivate* private = (RssconPrivate*) rsscon->private;
	return private->open;
}

const char* rssconGetDevice(Rsscon* rsscon) {
	assert(rsscon != NULL);
	assert(rsscon->private != NULL);
	RssconPrivate* private = (RssconPrivate*) rsscon->private;
	return private->device;
}

unsigned int rssconGetBaudRate(Rsscon* rsscon) {
	assert(rsscon != NULL);
	assert(rsscon->private != NULL);
	RssconPrivate* private = (RssconPrivate*) rsscon->private;
	return private->baudRate;
}
