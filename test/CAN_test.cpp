#ifdef UNIT_TEST
#include "tests.h"
#include <unity.h>

CAN* can;

void setUpCan(){
  can = new CAN();
}

void CCD_tests_load (){
  TEST_ASSERT_EQUAL_INT(0,can->init());
}

int run_ccd_tests(){
    UNITY_BEGIN();
    RUN_TEST(CCD_tests_load);
    return UNITY_END();
}
#endif
