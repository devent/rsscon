#include <stdlib.h>
#include <check.h>

#include "rsscon.h"
#ifdef LINUX
#include "rssconlinux.h"
#endif

START_TEST (testRssconInit)
	{
		Rsscon rsscon = { 0 };
		int ret = rssconInit(&rsscon);
		fail_unless(ret);
		fail_unless(rsscon.private != NULL);
		fail_unless(rsscon.portdata == NULL);
		fail_unless(rsscon.rssconOpen == NULL);
		fail_unless(rsscon.rssconClose == NULL);
		fail_unless(rsscon.rssconWrite == NULL);
		fail_unless(rsscon.rssconRead == NULL);
		fail_unless(rsscon.rssconLastError == NULL);
	}END_TEST

START_TEST (testRssconSetupInterface)
	{
		Rsscon rsscon = { 0 };
		int ret = rssconInit(&rsscon);
		fail_unless(ret);
		ret = rssconSetupInterface(&rsscon);
		fail_unless(ret);
		fail_unless(rsscon.rssconOpen != NULL);
		fail_unless(rsscon.rssconClose != NULL);
		fail_unless(rsscon.rssconWrite != NULL);
		fail_unless(rsscon.rssconRead != NULL);
		fail_unless(rsscon.rssconLastError != NULL);
	}END_TEST

START_TEST (testRssconOpenWithNotSetDevice)
	{
		Rsscon rsscon = { 0 };
		int ret = rssconInit(&rsscon);
		fail_unless(ret);
		ret = rssconSetupInterface(&rsscon);
		fail_unless(ret);
		ret = rssconOpen(&rsscon);
		fail_if(ret);
		int lastError = rssconLastError(&rsscon);
#ifdef LINUX
		fail_unless(lastError == RSSLINUX_ERROR_OPENDEVICE);
#endif
	}END_TEST

Suite* addCoreTestCase(Suite* s) {
	TCase *tc_core = tcase_create("core");
	tcase_add_test (tc_core, testRssconInit);
	tcase_add_test (tc_core, testRssconSetupInterface);
	tcase_add_test (tc_core, testRssconOpenWithNotSetDevice);
	suite_add_tcase(s, tc_core);
	return s;
}

Suite* createRssconSuite(void) {
	Suite *s = suite_create("rsscon");
	s = addCoreTestCase(s);
	return s;
}

int runSuite(Suite *s) {
	SRunner *sr = srunner_create(s);
	srunner_run_all(sr, CK_NORMAL);
	int ret = srunner_ntests_failed(sr);
	srunner_free(sr);

	return ret;
}

int main() {
	Suite *s = createRssconSuite();
	int ret = runSuite(s);
	return (ret == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
