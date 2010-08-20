#include <assert.h>
#include "rsscon.h"

typedef struct {

	/**
	 * True when the port is open and ready to read/write, False otherwise.
	 */
	bool open;

} RssconPrivate;

bool rssconInit(Rsscon* rsscon) {
	assert(rsscon != NULL);
	assert(rsscon->private == NULL);

	RssconPrivate private = { 0 };
	rsscon->private = &private;
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

int rssconLastError(Rsscon* rsscon) {
	assert(rsscon != NULL);
	assert(rsscon->rssconLastError != NULL);
	return rsscon->rssconLastError(rsscon);
}

bool rssconIsOpen(Rsscon* rsscon) {
	assert(rsscon != NULL);
	RssconPrivate* private = (RssconPrivate*) rsscon->private;
	assert(private != NULL);
	return private->open;
}
