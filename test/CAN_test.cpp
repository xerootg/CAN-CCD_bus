#ifdef UNIT_TEST
#include "tests.h"
#include <unity.h>

CAN* can;

void setUpCan(){
  can = new CAN();
}

void CCD_tests_load (){
  TEST_ASSERT_EQUAL_INT(1,can->init());
}

void run_ccd_tests(){
    RUN_TEST(CCD_tests_load);
}
#endif
