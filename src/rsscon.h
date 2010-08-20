/*
 * rsscon.h
 *
 *  Created on: Aug 19, 2010
 *      Author: devent
 */

#ifndef RSSCON_H_
#define RSSCON_H_

#include <stdbool.h>

/**
 *   bool rssconOpen(void* portdata)
 *
 *       Opens the port for read and write access.
 *
 *       portdata: contains all information needed to use the port.
 *       return: true if the function succeeds, false if there was an error.
 */
typedef bool (*RssconOpen)(void*);

/**
 *   bool rssconClose(void* portdata)
 *
 *       Closes the port.
 *
 *       portdata: contains all information needed to use the port.
 *       return: true if the function succeeds, false if there was an error.
 */
typedef bool (*RssconClose)(void*);

/**
 *   bool rssconWrite(void* portdata, char* data, size_t length, size_t* writed)
 *
 *       Writes data to the port.
 *
 *       portdata: contains all information needed to use the port.
 *       data: contains the data to write to the port.
 *       length: the length of data to write.
 *       writed: how mush data was really written to the port.
 *       return: true if the function succeeds, false if there was an error.
 */
typedef bool (*RssconWrite)(void*, const void*, size_t, size_t*);

/**
 *   bool rssconRead(void* portdata, char* data, size_t length, size_t* read)
 *
 *       Reads data to the port.
 *
 *       portdata: contains all information needed to use the port.
 *       data: the buffer to read to.
 *       length: how mush we want to read.
 *       writed: how mush data was really readed from the port.
 *       return: true if the function succeeds, false if there was an error.
 */
typedef bool (*RssconRead)(void*, void*, size_t, size_t*);

/**
 *   int rssconLastError(void* portdata)
 *
 *       Get the last error caused by accessing the port.
 *
 *       portdata: contains all information needed to use the port.
 *       return: the code of the last error.
 */
typedef int (*RssconLastError)(void*);

/**
 * Structure defines the public interface to rsscon.
 */
typedef struct {
	int open; //* is 1 when the port is open and
	//* ready to read/write, 0 otherwise.

	void* portdata; //* The portdata to use the port.

	RssconOpen rssconOpen; //* Opens the port for read and
	//* write access.

	RssconClose rssconClose; //* Closes the port.

	RssconWrite rssconWrite; //* Writes data to the port.

	RssconRead rssconRead; //* Reads data to the port.

	RssconLastError rssconLastError; //* Get the last error caused by
//* accessing the port.
} Rsscon;

/**
 * Setup the rsscon public interface.
 */
bool rssconSetupInterface(Rsscon* rsscon);

#endif /* RSSCON_H_ */
