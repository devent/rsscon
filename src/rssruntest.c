#include <stdlib.h>
#include <stdio.h>

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

bool readInfo(Rsscon* rsscon) {
	char* data = "i\r\n";
	size_t length = 3;
	size_t wrote;
	bool ret = rssconWrite(rsscon, data, length, &wrote);
	if (!ret) {
		return false;
	}

	char* reddata = malloc(sizeof(char) * 255);
	size_t redlength = 8;
	size_t red;
	while (red >= redlength) {
		ret = rssconRead(rsscon, reddata, redlength, &red);
		printErrorUnless(ret, "rsscon close.");
		printRssconError(rsscon);
		if (!ret) {
			break;
		}
		printf("%s", reddata);
	}

	free(reddata);
	return true;
}

int main() {
#ifdef LINUX
	const char* device = "/dev/ttyUSB0";
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

	ret = readInfo(rsscon);
	printErrorUnless(ret, "read info.");
	printRssconError(rsscon);
	cleanupUnless(ret, rsscon);

	return 0;
}
