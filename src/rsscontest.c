#include <stdlib.h>
#include <check.h>

#include "rsscon.h"
#ifdef LINUX
#include "rssconlinux.h"
#endif

START_TEST (testRssconInit)
	{
		Rsscon rsscon = { 0 };
		const char* device = "";
		unsigned int baudRate = 0;
		bool ret = rssconSetupInterface(&rsscon);
		fail_unless(ret);
		fail_unless(rsscon.portdata != NULL);
		ret = rssconInit(&rsscon, device, baudRate);
		fail_unless(ret);
		fail_unless(rsscon.private != NULL);
	}END_TEST

START_TEST (testRssconSetupInterface)
	{
		Rsscon rsscon = { 0 };
		bool ret = rssconSetupInterface(&rsscon);
		fail_unless(ret);
		fail_unless(rsscon.portdata != NULL);
		fail_unless(rsscon.rssconInit != NULL);
		fail_unless(rsscon.rssconOpen != NULL);
		fail_unless(rsscon.rssconClose != NULL);
		fail_unless(rsscon.rssconWrite != NULL);
		fail_unless(rsscon.rssconRead != NULL);
	}END_TEST

START_TEST (testRssconOpenWithNotSetDevice)
	{
		Rsscon rsscon = { 0 };
		const char* device = "";
		unsigned int baudRate = 0;
		bool ret = rssconSetupInterface(&rsscon);
		fail_unless(ret);
		ret = rssconInit(&rsscon, device, baudRate);
		fail_unless(ret);
		ret = rssconOpen(&rsscon);
		fail_if(ret);
		int lastError = rssconGetLastError(&rsscon);
		fail_unless(lastError == RSSCON_ERROR_OPENDEVICE);
	}END_TEST

START_TEST (testRssconOpen)
	{
		Rsscon rsscon = { 0 };
		const char* device = "/dev/ttyUSB0";
		unsigned int baudRate = RSSCON_BAUDRATE_921600;
		bool ret = rssconSetupInterface(&rsscon);
		fail_unless(ret);
		ret = rssconInit(&rsscon, device, baudRate);
		fail_unless(ret);
		ret = rssconOpen(&rsscon);
		fail_unless(ret);
		bool isOpen = rssconIsOpen(&rsscon);
		fail_unless(isOpen);
	}END_TEST

Suite* addCoreTestCase(Suite* s) {
	TCase *tc_core = tcase_create("core");
	tcase_add_test (tc_core, testRssconSetupInterface);
	tcase_add_test (tc_core, testRssconInit);
	tcase_add_test (tc_core, testRssconOpenWithNotSetDevice);
	tcase_add_test (tc_core, testRssconOpen);
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
