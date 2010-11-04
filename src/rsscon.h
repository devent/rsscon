/**
 * Copyright 2010 Erwin Mueller, <erwin.mueller@deventm.org>
 * Version 1.0
 *
 * This is the rsscon driver API. The purpose is to open a port, read and write
 * data to the port and close it. The API is designed to abstract the system
 * and can be used with a Linux or Windows implementation.
 *
 * The C header file of the implementation is included and the function
 * rssconInit() setup the private implementation for the system. For an
 * example, see rsscontest.c and rssruntest.c.
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
