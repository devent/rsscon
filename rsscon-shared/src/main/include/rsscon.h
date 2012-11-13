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
#ifndef RSSCON_H_
#define RSSCON_H_

#include <stdlib.h>
#include <stdbool.h>

#define RSSCON_BAUDRATE_57600 57600
#define RSSCON_BAUDRATE_115200 115200
#define RSSCON_BAUDRATE_230400 230400
#define RSSCON_BAUDRATE_460800 460800
#define RSSCON_BAUDRATE_500000 500000
#define RSSCON_BAUDRATE_576000 576000
#define RSSCON_BAUDRATE_921600 921600
#define RSSCON_BAUDRATE_1000000 1000000
#define RSSCON_BAUDRATE_1152000 1152000
#define RSSCON_BAUDRATE_1500000 1500000
#define RSSCON_BAUDRATE_2000000 2000000
#define RSSCON_BAUDRATE_2500000 2500000
#define RSSCON_BAUDRATE_3000000 3000000
#define RSSCON_BAUDRATE_3500000 3500000
#define RSSCON_BAUDRATE_4000000 4000000

#define RSSCON_ERROR_NOERROR 0
#define RSSCON_ERROR_OPENDEVICE -1
#define RSSCON_ERROR_CLOSEDEVICE -2
#define RSSCON_ERROR_SETUPDEVICE -3
#define RSSCON_ERROR_WRITE -4
#define RSSCON_ERROR_READ -5

typedef struct Rsscon Rsscon;

typedef bool (*RssconInit)(Rsscon*);

typedef bool (*RssconFree)(Rsscon*);

typedef bool (*RssconOpen)(Rsscon*);

typedef bool (*RssconClose)(Rsscon*);

typedef bool (*RssconSetBlocking)(Rsscon*, bool block);

typedef bool (*RssconGetBlocking)(Rsscon*);

typedef bool (*RssconSetWait)(Rsscon*, bool wait);

typedef bool (*RssconGetWait)(Rsscon*);

typedef bool (*RssconWrite)(Rsscon*, const void*, size_t, size_t*);

typedef bool (*RssconRead)(Rsscon*, void*, size_t, size_t*);

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

	RssconInit rssconInit;

	RssconFree rssconFree;

	RssconOpen rssconOpen;

	RssconClose rssconClose;

	RssconWrite rssconWrite;

	RssconRead rssconRead;

	RssconSetBlocking rssconSetBlocking;

	RssconGetBlocking rssconGetBlocking;

	RssconSetWait rssconSetWait;

	RssconGetWait rssconGetWait;

};

/**
 * Create the rsscon driver.
 *
 * device: The name of the device. Under Linux it's /dev/ttyUSB0.
 * baudRate: The input and output baud rate of the device.
 * return: the public interface of the rsscon driver.
 */
Rsscon* rssconCreate(const char* device, unsigned int baudRate);

/**
 * Frees the allocated memory of the rsscon driver.
 *
 * rsscon: The public interface to the rsscon driver.
 */
bool rssconFree(Rsscon* rsscon);

/**
 * Setup the rsscon driver.
 *
 * rsscon: The public interface to the rsscon driver.
 */
bool rssconInit(Rsscon* rsscon);

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
 * Returns the device name that we read and write data to and from.
 *
 * rsscon: The public interface to the rsscon driver.
 */
const char* rssconGetDevice(Rsscon* rsscon);

/**
 * Returns the input and output baud rate of the device.
 *
 * rsscon: The public interface to the rsscon driver.
 */
unsigned int rssconGetBaudRate(Rsscon* rsscon);

/**
 * Sets the last error caused by accessing the port.
 *
 * rsscon: The public interface to the rsscon driver.
 * lastError: The last error.
 */
void rssconSetLastError(Rsscon* rsscon, int lastError);

/**
 * Get the last error caused by accessing the port.
 *
 * rsscon: The public interface to the rsscon driver.
 * return: the code of the last error.
 */
int rssconGetLastError(Rsscon* rsscon);

#endif /* RSSCON_H_ */
