/**
 * Copyright 2010 Erwin Müller <erwin.mueller@deventm.org>
 * Version 1.0
 *
 * A system test for the rsscon driver.
 *
 * This file is part of rsscon.
 *
 * rsscon is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * rsscon is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with
 * rsscon. If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "rsscon.h"
#ifdef LINUX
#include "rssconlinux.h"
#endif

void printRssconError(const Rsscon* rsscon) {
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

bool readDataToMax(Rsscon* rsscon, size_t max) {
	char reddata;
	size_t redlength = 1;
	size_t red;
	size_t count = 0;

	while (count < max) {
		count++;
		bool ret = rssconRead(rsscon, &reddata, redlength, &red);
		printErrorUnless(ret, "rsscon read.");
		printRssconError(rsscon);
		if (!ret) {
			break;
		}
		printf("%s", &reddata);
	}

	return true;
}

bool writeCommand(Rsscon* rsscon, const char* command) {
	size_t length = strlen(command);
	size_t wrote;
	bool ret = rssconWrite(rsscon, command, length, &wrote);
	return ret;
}

bool readInfo(Rsscon* rsscon) {
	bool ret = writeCommand(rsscon, "i");
	if (!ret) {
		return false;
	}

	ret = readDataToMax(rsscon, 96);
	if (!ret) {
		return false;
	}

	return true;
}

bool readData(Rsscon* rsscon) {
	bool ret = writeCommand(rsscon, "g");
	if (!ret) {
		return false;
	}

	ret = readDataToMax(rsscon, 63);
	if (!ret) {
		return false;
	}

	ret = writeCommand(rsscon, "s");
	if (!ret) {
		return false;
	}

	return true;
}

int main() {
#ifdef LINUX
	const char* device = "/dev/ttyUSB5";
#endif
	unsigned int baudrate = RSSCON_BAUDRATE_921600;
	Rsscon* rsscon = rssconCreate(device, baudrate);
	printErrorUnless(rsscon != NULL, "rsscon is null.");
	exitUnless(rsscon != NULL);

	bool ret = rssconInit(rsscon);
	printErrorUnless(ret, "rsscon init.");
	printRssconError(rsscon);
	cleanupUnless(ret, rsscon);
	exitUnless(ret);

	ret = rssconOpen(rsscon);
	printErrorUnless(ret, "rsscon open.");
	printRssconError(rsscon);
	cleanupUnless(ret, rsscon);

//	ret = readInfo(rsscon);
//	printErrorUnless(ret, "read info.");
//	printRssconError(rsscon);
//	cleanupUnless(ret, rsscon);

	ret = readData(rsscon);
	printErrorUnless(ret, "read data.");
	printRssconError(rsscon);
	cleanupUnless(ret, rsscon);

	return 0;
}
