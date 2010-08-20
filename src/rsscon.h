/*
 * rsscon.h
 *
 *  Created on: Aug 19, 2010
 *      Author: devent
 */

#ifndef RSSCON_H_
#define RSSCON_H_

#include <stdlib.h>
#include <stdbool.h>

typedef struct Rsscon Rsscon;

typedef bool (*RssconOpen)(Rsscon*);

typedef bool (*RssconClose)(Rsscon*);

typedef bool (*RssconWrite)(Rsscon*, const void*, size_t, size_t*);

typedef bool (*RssconRead)(Rsscon*, void*, size_t, size_t*);

typedef int (*RssconLastError)(Rsscon*);

/**
 * Structure defines the public interface to rsscon.
 */
struct Rsscon {

	/**
	 * The port data to use the port.
	 */
	void* portdata;

	/**
	 * Private data.
	 */
	void* private;

	RssconOpen rssconOpen;

	RssconClose rssconClose;

	RssconWrite rssconWrite;

	RssconRead rssconRead;

	RssconLastError rssconLastError;
};

/**
 * Initialize the rsscon driver.
 *
 * rsscon: The public interface to the rsscon driver.
 */
bool rssconInit(Rsscon* rsscon);

/**
 * Setup the rsscon public interface.
 *
 * rsscon: The public interface to the rsscon driver.
 */
bool rssconSetupInterface(Rsscon* rsscon);

/**
 * Opens the port for read and write access.
 *
 * rsscon: The public interface to the rsscon driver.
 * return: true if the function succeeds, false if there was an error.
 */
bool rssconOpen(Rsscon* rsscon);

/**
 * Closes the port.
 *
 * rsscon: The public interface to the rsscon driver.
 * return: true if the function succeeds, false if there was an error.
 */
bool rssconClose(Rsscon* rsscon);

/**
 * Writes data to the port.
 *
 * rsscon: The public interface to the rsscon driver.
 * data: contains the data to write to the port.
 * length: the length of data to write.
 * wrote: how much data was really written to the port.
 * return: true if the function succeeds, false if there was an error.
 */
bool
rssconWrite(Rsscon* rsscon, const void* data, size_t length, size_t* wrote);

/**
 * Reads data to the port.
 *
 * rsscon: The public interface to the rsscon driver.
 * data: the buffer to read to.
 * length: how much we want to read.
 * red: how much data was really red from the port.
 * return: true if the function succeeds, false if there was an error.
 */
bool rssconRead(Rsscon* rsscon, void* data, size_t length, size_t* red);

/**
 * Test if the port is open for read/write.
 *
 * rsscon: The public interface to the rsscon driver.
 * return: True when the port is open and ready to read/write, False otherwise.
 */
bool rssconIsOpen(Rsscon* rsscon);

/**
 * Get the last error caused by accessing the port.
 *
 * rsscon: The public interface to the rsscon driver.
 * return: the code of the last error.
 */
int rssconLastError(Rsscon* rsscon);

#endif /* RSSCON_H_ */
