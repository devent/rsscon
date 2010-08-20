#include <assert.h>
#include "rsscon.h"

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

bool rssconInit(Rsscon* rsscon, const char* device, unsigned int baudRate) {
	assert(rsscon != NULL);
	assert(rsscon->private == NULL);

	RssconPrivate private = { 0 };
private.device = device;
private.baudRate = baudRate;
	rsscon->private = &private;

	assert(rsscon->rssconInit != NULL);
	return rsscon->rssconInit(rsscon);
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
	RssconPrivate* private = (RssconPrivate*) rsscon->private;
	assert(private != NULL);
private->lastError = lastError;
}

int rssconGetLastError(Rsscon* rsscon) {
	assert(rsscon != NULL);
	RssconPrivate* private = (RssconPrivate*) rsscon->private;
	assert(private != NULL);
	return private->lastError;
}

bool rssconIsOpen(Rsscon* rsscon) {
	assert(rsscon != NULL);
	RssconPrivate* private = (RssconPrivate*) rsscon->private;
	assert(private != NULL);
	return private->open;
}

const char* rssconGetDevice(Rsscon* rsscon) {
	assert(rsscon != NULL);
	RssconPrivate* private = (RssconPrivate*) rsscon->private;
	assert(private != NULL);
	return private->device;
}

unsigned int rssconGetBaudRate(Rsscon* rsscon) {
	assert(rsscon != NULL);
	RssconPrivate* private = (RssconPrivate*) rsscon->private;
	assert(private != NULL);
	return private->baudRate;
}
