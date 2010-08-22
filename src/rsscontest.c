#include <stdlib.h>
#include <check.h>
#include "CMemLeak.h"

#include "rsscon.h"
#ifdef LINUX
#include "rssconlinux.h"
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
#ifdef LINUX
		const char* device = "/dev/ttyUSB0";
#endif
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

Suite* addCoreTestCase(Suite* s) {
	TCase *tc_core = tcase_create("core");
	tcase_add_test (tc_core, testRssconCreateAndFree);
	tcase_add_test (tc_core, testRssconCreateAndFreeAndProperties);
	tcase_add_test (tc_core, testRssconInit);
	tcase_add_test (tc_core, testRssconOpenWithNoDevice);
	tcase_add_test (tc_core, testRssconOpenClose);
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
