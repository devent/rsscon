/*
 * rsscontest_linux.c
 *
 *  Created on: Aug 19, 2010
 *      Author: devent
 */

#include <stdio.h>

#include "rsscon.h"
#ifdef LINUX
#include "rssconlinux.h"
#endif

void printError(Rsscon* rsscon) {
	int err = rsscon->rssconLastError(rsscon);
	fprintf(stderr, "Last Error: %d\n", err);
}

void error(Rsscon* rsscon, const char* str) {
	fprintf(stderr, str);
	printError(rsscon);
	scanf("press a key");
}

int main() {
	printf("start.");
	Rsscon rsscon = { 0 };
	int ret = rssconSetupInterface(&rsscon);
	if (!ret) {
		error(&rsscon, "error by rssconSetupInterface()...\n");
		printf("error.");
		return 1;
	}

	printf("done.");
	return 0;
}
