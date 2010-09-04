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
	const char* device;

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
	private->device = device;
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
	assert(rsscon != NULL);
	assert(rsscon->private != NULL);

	if (!freeChildIfChildWasSet(rsscon)) {
		return false;
	}

	RssconPrivate* private = (RssconPrivate*) rsscon->private;
	free(private);
	free(rsscon);

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
