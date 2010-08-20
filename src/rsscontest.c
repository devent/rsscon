#include <stdlib.h>
#include <check.h>

#include "rsscon.h"
#ifdef LINUX
#include "rssconlinux.h"
#endif

START_TEST (testRssconSetupInterface)
	{
		Rsscon rsscon = { 0 };
		int ret = rssconSetupInterface(&rsscon);
		fail_unless(ret);
	}END_TEST

Suite* addCoreTestCase(Suite* s) {
	TCase *tc_core = tcase_create("core");
	tcase_add_test (tc_core, testRssconSetupInterface);
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
