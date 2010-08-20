#include <assert.h>
#include "rsscon.h"

/**
 * Structure defines the public interface to rsscon.
 */
typedef struct {

	/**
	 * True when the port is open and ready to read/write, False otherwise.
	 */
	bool open;

	/**
	 * The port data to use the port.
	 */
	void* portdata;

	RssconOpen rssconOpen;

	RssconClose rssconClose;

	RssconWrite rssconWrite;

	RssconRead rssconRead;

	RssconLastError rssconLastError;
} RssconInterface;

bool rssconOpen(Rsscon rsscon) {
	assert(rsscon != NULL);
	RssconInterface* rssconcast = (RssconInterface*)rsscon;
	assert(rssconcast->rssconOpen != NULL);
	return rssconcast->rssconOpen(rsscon);
}

bool rssconClose(Rsscon rsscon) {
	assert(rsscon != NULL);
	RssconInterface* rssconcast = (RssconInterface*)rsscon;
	assert(rssconcast->rssconClose != NULL);
	return rssconcast->rssconClose(rsscon);
}

bool rssconWrite(Rsscon rsscon, const void* data, size_t length, size_t* wrote) {
	assert(rsscon != NULL);
	RssconInterface* rssconcast = (RssconInterface*)rsscon;
	assert(rssconcast->rssconWrite != NULL);
	return rssconcast->rssconWrite(rsscon, data, length, wrote);
}

bool rssconRead(Rsscon rsscon, void* data, size_t length, size_t* red) {
	assert(rsscon != NULL);
	RssconInterface* rssconcast = (RssconInterface*)rsscon;
	assert(rssconcast->rssconRead != NULL);
	return rssconcast->rssconRead(rsscon, data, length, red);
}

int rssconLastError(Rsscon rsscon) {
	assert(rsscon != NULL);
	RssconInterface* rssconcast = (RssconInterface*)rsscon;
	assert(rssconcast->rssconLastError != NULL);
	return rssconcast->rssconLastError(rsscon);
}

bool rssconIsOpen(Rsscon rsscon) {
	assert(rsscon != NULL);
	RssconInterface* rssconcast = (RssconInterface*)rsscon;
	return rssconcast->open;
}
