#include <stdlib.h>
#include <stdio.h>

#include "rsscon.h"
#ifdef LINUX
#include "rssconlinux.h"
#endif

void printError(Rsscon* rsscon) {
	int err = rssconGetLastError(rsscon);
	fprintf(stderr, "Last Error: %d\n", err);
}

void exitUnless(const char* message, bool expression, Rsscon* rsscon) {
	if (!expression) {
		fprintf(stderr, "%s ", message);
		printError(rsscon);
		exit(1);
	}
}

void printMessage(const char* message) {
	fprintf(stderr, "%s\n", message);
}

int main() {
	Rsscon rsscon = { 0 };
	const char* device = "/dev/ttyUSB0\0";
	unsigned int baudRate = RSSCON_BAUDRATE_921600;

	bool ret = rssconSetupInterface(&rsscon);
	exitUnless("Error in rssconSetupInterface", ret, &rsscon);
	printMessage("setup interface finished.");

	ret = rssconInit(&rsscon, device, baudRate);
	exitUnless("Error in rssconInit.", ret, &rsscon);
	printMessage("init finished.");

	ret = rssconOpen(&rsscon);
	exitUnless("Error in rssconOpen.", ret, &rsscon);
	printMessage("open finished.");

	bool isopen = rssconIsOpen(&rsscon);
	exitUnless("Error in rssconIsOpen.", isopen, &rsscon);
	printMessage("is open finished.");

	return 0;
}
