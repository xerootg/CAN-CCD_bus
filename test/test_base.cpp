#ifdef UNIT_TEST

#include <unity.h>

#include "tests.h"

void setUp(){
  setUpCounter();
  setUpCan();
  setUpVehicleState();
}

int main( int argc, char **argv) {
    int counter_tests = run_counter_tests();
    int CCD_tests = run_ccd_tests();
    int state_tests = run_vehicleState_tests();
    //return counter_tests;
    return counter_tests+CCD_tests+state_tests;
}

#endif
