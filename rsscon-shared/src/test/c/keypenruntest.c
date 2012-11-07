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
#ifdef RSSCON_WINDOWS
#include "rssconwindows.h"
#endif

#define LOG_CATEGORY "com.anrisoftware.rsscon.keypenruntest"


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

bool readData(Rsscon* rsscon) {
	bool ret = writeCommand(rsscon, "\xFF\x80");
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
#ifdef __linux
	const char* device = "/dev/ttyUSB5";
#endif
#ifdef RSSCON_WINDOWS
	const char* device = "COM:5";
#endif
	LOG4C_CATEGORY log = get_log(LOG_CATEGORY);

	log_info(log, "create rsscon");
	unsigned int baudrate = RSSCON_BAUDRATE_921600;
	Rsscon* rsscon = rssconCreate(device, baudrate);
	printErrorUnless(rsscon != NULL, "rsscon is null.");
	exitUnless(rsscon != NULL);

	log_info(log, "init rsscon");
	bool ret = rssconInit(rsscon);
	printErrorUnless(ret, "rsscon init.");
	printRssconError(rsscon);
	cleanupUnless(ret, rsscon);
	exitUnless(ret);

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
