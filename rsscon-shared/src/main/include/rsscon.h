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

/**
 * No error.
 */
#define RSSCON_ERROR_NOERROR 0

/**
 * Error opening the device.
 */
#define RSSCON_ERROR_OPENDEVICE -1

/**
 * Error closing the device.
 */
#define RSSCON_ERROR_CLOSEDEVICE -2

/**
 * Error setup the device.
 */
#define RSSCON_ERROR_SETUPDEVICE -3

/**
 * Error setting flags because device is already open.
 */
#define RSSCON_ERROR_DEVICE_OPENED -4

/**
 * Error writing to the device.
 */
#define RSSCON_ERROR_WRITE -5

/**
 * Error reading from the device.
 */
#define RSSCON_ERROR_READ -6

typedef struct Rsscon Rsscon;

typedef bool (*RssconInit)(Rsscon*);

typedef bool (*RssconFree)(Rsscon*);

typedef bool (*RssconOpen)(Rsscon*);

typedef bool (*RssconClose)(Rsscon*);

typedef bool (*RssconSetBlocking)(Rsscon*, bool);

typedef bool (*RssconGetBlocking)(Rsscon*);

typedef bool (*RssconSetWait)(Rsscon*, bool);

typedef bool (*RssconGetWait)(Rsscon*);

typedef bool (*RssconWrite)(Rsscon*, const void*, size_t, size_t*);

typedef bool (*RssconRead)(Rsscon*, void*, size_t, size_t*);

typedef bool (*RssconSetErrorNumber)(Rsscon*, int);

typedef int (*RssconGetErrorNumber)(Rsscon*);

typedef const char* (*RssconGetErrorNumberAsString)(Rsscon*);

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

	/**
	 * Initialize the private data.
	 *
	 * (Rsscon*): the Rsscon data structure.
	 * returns bool: true upon success, false upon failure.
	 */
	RssconInit rssconInit;

	/**
	 * Frees the private data.
	 *
	 * (Rsscon*): the Rsscon data structure.
	 * returns bool: true upon success, false upon failure.
	 */
	RssconFree rssconFree;

	/**
	 * Opens the serial port for read and write.
	 *
	 * (Rsscon*): the Rsscon data structure.
	 * returns bool: true upon success, false upon failure.
	 */
	RssconOpen rssconOpen;

	/**
	 * Closes the serial port.
	 *
	 * (Rsscon*): the Rsscon data structure.
	 * returns bool: true upon success, false upon failure.
	 */
	RssconClose rssconClose;

	/**
	 * Writes data to the serial port.
	 *
	 * (Rsscon*): the Rsscon data structure.
	 * const void*: the data to write.
	 * size_t: the size of the data.
	 * size_t*: will be set to how much data was written.
	 * returns bool: true upon success, false upon failure.
	 */
	RssconWrite rssconWrite;

	/**
	 * Reads data from the serial port.
	 *
	 * (Rsscon*): the Rsscon data structure.
	 * void*: the pointer to where to write the data.
	 * size_t: how much data to read.
	 * size_t*: will be set to how much data was red.
	 * returns bool: true upon success, false upon failure.
	 */
	RssconRead rssconRead;

	/**
	 * Sets whether the read and write access should block.
	 * Defaults to false.
	 *
	 * (Rsscon*): the Rsscon data structure.
	 * bool: set to true to block, false to not to block.
	 * returns bool: true upon success, false upon failure.
	 */
	RssconSetBlocking rssconSetBlocking;

	/**
	 * Returns if it was set to wait for the device.
	 *
	 * (Rsscon*): the Rsscon data structure.
	 * returns bool: true if it was set to wait, false otherwise.
	 */
	RssconGetBlocking rssconGetBlocking;

	/**
	 * Sets whether to wait for the device.
	 * Defaults to false.
	 *
	 * (Rsscon*): the Rsscon data structure.
	 * bool: set to true to wait, false to not to wait.
	 * returns bool: true upon success, false upon failure.
	 */
	RssconSetWait rssconSetWait;

	/**
	 * Returns if it was set to wait for the device.
	 *
	 * (Rsscon*): the Rsscon data structure.
	 * returns bool: true if it was set to wait, false otherwise.
	 */
	RssconGetWait rssconGetWait;

	/**
	 * Sets the system specific error number from the last error caused.
	 *
	 * (Rsscon*): the Rsscon data structure.
	 * int: the code of the system error.
	 * returns bool: true upon success, false upon failure.
	 */
	RssconSetErrorNumber rssconSetErrorNumber;

	/**
	 * Get the system specific error number from the last error caused.
	 *
	 * (Rsscon*): the Rsscon data structure.
	 * return int: the code of the system error.
	 */
	RssconGetErrorNumber rssconGetErrorNumber;

	/**
	 * Returns a string for the system specific error number.
	 *
	 * (Rsscon*): the Rsscon data structure.
	 * return const char*: the string of the system error.
	 */
	RssconGetErrorNumberAsString rssconGetErrorNumberAsString;
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
 * Sets whether the read and write access should block.
 * Defaults to false.
 *
 * rsscon: The public interface to the rsscon driver.
 * block: set to true to block, false to not to block.
 * return: true if it was set to wait, false otherwise.
 */
bool rssconSetBlocking(Rsscon* rsscon, bool block);

/**
 * Returns if it was set to wait for the device.
 *
 * rsscon: The public interface to the rsscon driver.
 * return: true if it was set to wait, false otherwise.
 */
bool rssconGetBlocking(Rsscon* rsscon);

/**
 * Sets whether to wait for the device.
 * Defaults to false.
 *
 * rsscon: The public interface to the rsscon driver.
 * wait: set to true to wait, false to not to wait.
 * return: true upon success, false upon failure.
 */
bool rssconSetWait(Rsscon* rsscon, bool wait);

/**
 * Returns if it was set to wait for the device.
 *
 * rsscon: The public interface to the rsscon driver.
 * return: true if it was set to wait, false otherwise.
 */
bool rssconGetWait(Rsscon* rsscon);

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
 * Sets the last error caused.
 *
 * rsscon: The public interface to the rsscon driver.
 * lastError: The last error, see the RSSCON_ERROR_* constants.
 * errorNumber: The system specific error number.
 * return: true upon success, false upon failure.
 */
bool rssconSetLastError(Rsscon* rsscon, int lastError, int errorNumber);

/**
 * Get the last error caused.
 *
 * rsscon: The public interface to the rsscon driver.
 * return: the code of the last error.
 */
int rssconGetLastError(Rsscon* rsscon);

/**
 * Get the system specific error number from the last error caused.
 *
 * rsscon: The public interface to the rsscon driver.
 * return: the code of the system error.
 */
int rssconGetErrorNumber(Rsscon* rsscon);

/**
 * Returns a string for the system specific error number from the last
 * error caused.
 *
 * rsscon: The public interface to the rsscon driver.
 * return: the string of the system error.
 */
const char* rssconGetErrorNumberAsString(Rsscon* rsscon);

#endif /* RSSCON_H_ */
