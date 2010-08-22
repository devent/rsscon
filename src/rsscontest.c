#include <stdlib.h>
#include <check.h>

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

Suite* addCoreTestCase(Suite* s) {
	TCase *tc_core = tcase_create("core");
	tcase_add_test (tc_core, testRssconCreateAndFree);
	tcase_add_test (tc_core, testRssconCreateAndFreeAndProperties);
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
