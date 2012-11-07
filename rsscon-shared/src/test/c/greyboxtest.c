/**
 * Copyright 2010 Erwin Müller <erwin.mueller@deventm.org>
 * Version 1.0
 *
 * A test suite to test the rsscon driver with the help of the check
 * framework.
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
#include <check.h>
#include "CMemLeak.h"

#include "rsscon.h"
#ifdef __linux
#include "rssconlinux.h"
#endif

#ifdef __linux
const char* device_path = "/dev/ttyUSB5";
#endif

START_TEST (testRssconCreateAndFree)
	{
		const char* device = "mydevice";
		unsigned int baudRate = 0;
		Rsscon* rsscon = rssconCreate(device, baudRate);

		fail_if(rsscon == NULL);
		fail_unless(rsscon->portdata == NULL);
		fail_if(rsscon->private == NULL);

		bool ret = rssconFree(rsscon);
		fail_unless(ret);

	}END_TEST

START_TEST (testRssconCreateAndFreeAndProperties)
	{
		const char* device = "mydevice";
		unsigned int baudrate = 999;
		Rsscon* rsscon = rssconCreate(device, baudrate);

		fail_if(rsscon == NULL);
		fail_unless(rsscon->portdata == NULL);
		fail_if(rsscon->private == NULL);

		const char* thatdevice = rssconGetDevice(rsscon);
		fail_unless(strcmp(device, thatdevice) == 0);

		unsigned int thatbaudrate = rssconGetBaudRate(rsscon);
		fail_unless(baudrate == thatbaudrate);

		bool ret = rssconFree(rsscon);
		fail_unless(ret);

	}END_TEST

START_TEST (testRssconInit)
	{
		const char* device = "mydevice";
		unsigned int baudrate = 999;
		Rsscon* rsscon = rssconCreate(device, baudrate);
		fail_if(rsscon == NULL);

		bool ret = rssconInit(rsscon);
		fail_unless(ret);
		fail_if(rsscon->rssconClose == NULL);
		fail_if(rsscon->rssconFree == NULL);
		fail_if(rsscon->rssconInit == NULL);
		fail_if(rsscon->rssconOpen == NULL);
		fail_if(rsscon->rssconRead == NULL);
		fail_if(rsscon->rssconWrite == NULL);

		ret = rssconFree(rsscon);
		fail_unless(ret);

	}END_TEST

START_TEST (testRssconOpenWithNoDevice)
	{
		const char* device = "mydevice";
		unsigned int baudrate = RSSCON_BAUDRATE_115200;
		Rsscon* rsscon = rssconCreate(device, baudrate);
		fail_if(rsscon == NULL);

		bool ret = rssconInit(rsscon);
		fail_unless(ret);

		ret = rssconOpen(rsscon);
		fail_if(ret);

		int error = rssconGetLastError(rsscon);
		fail_unless(error == RSSCON_ERROR_OPENDEVICE);

		ret = rssconFree(rsscon);
		fail_unless(ret);

	}END_TEST

START_TEST (testRssconOpenClose)
	{
		const char* device = device_path;
		unsigned int baudrate = RSSCON_BAUDRATE_921600;
		Rsscon* rsscon = rssconCreate(device, baudrate);
		fail_if(rsscon == NULL);

		bool ret = rssconInit(rsscon);
		fail_unless(ret);

		ret = rssconOpen(rsscon);
		fail_unless(ret);

		ret = rssconClose(rsscon);
		fail_unless(ret);

		ret = rssconFree(rsscon);
		fail_unless(ret);

	}END_TEST

START_TEST (testRssconWrite)
	{
		const char* device = device_path;
		unsigned int baudrate = RSSCON_BAUDRATE_921600;
		Rsscon* rsscon = rssconCreate(device, baudrate);
		fail_if(rsscon == NULL);

		bool ret = rssconInit(rsscon);
		fail_unless(ret);

		ret = rssconOpen(rsscon);
		fail_unless(ret);

		void* data = "i\r\n";
		size_t length = 3;
		size_t wrote;
		ret = rssconWrite(rsscon, data, length, &wrote);
		fail_unless(ret);
		fail_unless(wrote == length);

		ret = rssconClose(rsscon);
		fail_unless(ret);

		ret = rssconFree(rsscon);
		fail_unless(ret);

	}END_TEST

START_TEST (testRssconWriteRead)
	{
		const char* device = device_path;
		unsigned int baudrate = RSSCON_BAUDRATE_921600;
		Rsscon* rsscon = rssconCreate(device, baudrate);
		fail_if(rsscon == NULL);

		bool ret = rssconInit(rsscon);
		fail_unless(ret);

		ret = rssconOpen(rsscon);
		fail_unless(ret);

		char* data = "i\r\n";
		size_t length = 3;
		size_t wrote;
		ret = rssconWrite(rsscon, data, length, &wrote);
		fail_unless(ret);
		fail_unless(wrote == length);

		char* reddata = malloc(sizeof(char)*255);
		size_t redlength = 38;
		size_t red;
		ret = rssconRead(rsscon, reddata, redlength, &red);
		fail_unless(ret);
		fail_unless(red == redlength);
		free(reddata);

		reddata = malloc(sizeof(char)*255);
		redlength = 63;
		ret = rssconRead(rsscon, reddata, redlength, &red);
		fail_unless(ret);
		fail_unless(red == redlength);
		free(reddata);

		ret = rssconClose(rsscon);
		fail_unless(ret);

		ret = rssconFree(rsscon);
		fail_unless(ret);

	}END_TEST

Suite* addCoreTestCase(Suite* s) {
	TCase *tc_core = tcase_create("core");
	tcase_add_test (tc_core, testRssconCreateAndFree);
	tcase_add_test (tc_core, testRssconCreateAndFreeAndProperties);
	tcase_add_test (tc_core, testRssconInit);
	tcase_add_test (tc_core, testRssconOpenWithNoDevice);
	tcase_add_test (tc_core, testRssconOpenClose);
	tcase_add_test (tc_core, testRssconWrite);
	tcase_add_test (tc_core, testRssconWriteRead);
	suite_add_tcase(s, tc_core);
	return s;
}

Suite* createRssconSuite(void) {
	Suite *s = suite_create("rsscon");
	s = addCoreTestCase(s);
	return s;
}

int runSuite(Suite *s, bool fork) {
	SRunner *sr = srunner_create(s);
	if (!fork) {
		srunner_set_fork_status(sr, CK_NOFORK);
	}
	srunner_run_all(sr, CK_NORMAL);
	int ret = srunner_ntests_failed(sr);
	srunner_free(sr);

	return ret;
}

int main() {
	Suite *s = createRssconSuite();
	int ret = runSuite(s, false);
	return (ret == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
