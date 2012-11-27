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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "logger.h"
#include "rsscon.h"
#ifdef __linux
#include "rssconlinux.h"
#endif
#if defined(_WIN32) || defined(_WIN64)
#include "rssconwindows.h"
#endif

#define LOG_CATEGORY "com.anrisoftware.rsscon.keypenruntest"

void printRssconError(Rsscon* rsscon) {
	int err = rssconGetLastError(rsscon);
	if (err == RSSCON_ERROR_NOERROR) {
		return;
	}
	fprintf(stderr, "Last Error: %d\n", err);
}

void printErrorUnless(bool expression, const char* message) {
	if (!expression) {
		fprintf(stderr, "%s ", message);
	}
}

void exitUnless(bool expression) {
	if (!expression) {
		exit(1);
	}
}

void printMessage(const char* message) {
	fprintf(stderr, "%s\n", message);
}

void cleanupUnless(bool expression, Rsscon* rsscon) {
	if (expression) {
		return;
	}

	bool ret = rssconClose(rsscon);
	printErrorUnless(ret, "rsscon close.");
	printRssconError(rsscon);

	ret = rssconFree(rsscon);
	printErrorUnless(ret, "rsscon close.");
	printRssconError(rsscon);
}

bool readDataSensorBlock(Rsscon* rsscon) {
	size_t sensorCount = 16;
	size_t byteSizeSensor = 3;
	size_t size = sensorCount * byteSizeSensor;
	char data[size];
	size_t readBytes;

	bool ret = rssconRead(rsscon, data, size, &readBytes);
	if (!ret) {
		printErrorUnless(ret, "rsscon read.");
		printRssconError(rsscon);
		return false;
	}
	size_t k, m;
	for (k = 0; k < sensorCount; ++k) {
		for (m = 0; m < byteSizeSensor; ++m) {
			printf("%00hhx", data[m + k*m]);
		}
		printf(" ");
	}
	printf("\n");
	return true;
}

bool writeCommand(Rsscon* rsscon, const void* command, size_t size) {
	LOG4C_CATEGORY log = get_log(LOG_CATEGORY);
	log_info(log, "write command %d bytes.", size);

	size_t wrote;
	bool ret = rssconWrite(rsscon, command, size, &wrote);

	log_info(log, "wrote %d bytes.", wrote);
	free_log();
	return ret;
}

bool readData(Rsscon* rsscon) {
	char startCommand[] = { 0x55, 0xAA, 0x80, 0xFF, 0xFF, 0xFF };
	bool ret = writeCommand(rsscon, &startCommand, sizeof(startCommand));
	if (!ret) {
		return false;
	}

	int i;
	for (i = 0; i < 10; ++i) {
		ret = readDataSensorBlock(rsscon);
		if (!ret) {
			return false;
		}
	}

	char stopCommand[] = { 0x55, 0xAA, 0x00, 0xFF, 0xFF, 0xFF };
	ret = writeCommand(rsscon, stopCommand, sizeof(stopCommand));
	if (!ret) {
		return false;
	}

	return true;
}

int main() {
#ifdef __linux
	const char* device = "/dev/ttyUSB3";
#endif
#if defined(_WIN32) || defined(_WIN64)
	const char* device = "COM:5";
#endif
	LOG4C_CATEGORY log = get_log(LOG_CATEGORY);

	log_info(log, "create rsscon");
	unsigned int baudrate = RSSCON_BAUDRATE_921600;
	Rsscon* rsscon = rssconCreate(device, baudrate);
	printErrorUnless(rsscon != NULL, "rsscon is null.");
	exitUnless(rsscon != NULL );

	log_info(log, "init rsscon");
	bool ret = rssconInit(rsscon);
	printErrorUnless(ret, "rsscon init.");
	printRssconError(rsscon);
	cleanupUnless(ret, rsscon);
	exitUnless(ret);

	ret = rssconSetBlocking(rsscon, true);
	ret = rssconSetWait(rsscon, false);

	log_info(log, "open rsscon");
	ret = rssconOpen(rsscon);
	printErrorUnless(ret, "rsscon open.");
	printRssconError(rsscon);
	cleanupUnless(ret, rsscon);

//	ret = readInfo(rsscon);
//	printErrorUnless(ret, "read info.");
//	printRssconError(rsscon);
//	cleanupUnless(ret, rsscon);

	log_info(log, "read data");
	ret = readData(rsscon);
	printErrorUnless(ret, "read data.");
	printRssconError(rsscon);
	cleanupUnless(ret, rsscon);

	free_log();
	return 0;
}
