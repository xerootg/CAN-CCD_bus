#ifdef UNIT_TEST

#include <unity.h>

#ifdef ARDUINO
#include "arduino.h"
#endif

#include "tests.h"

#include <ArduinoFake.h>

using namespace fakeit;

void setUp(){
  ArduinoFakeReset();
  setUpCounter();
  setUpCan();
  setUpVehicleState();
}

int main( int argc, char **argv) {
    #ifdef ARDUINO
    delay(2000);
    #endif
    UNITY_BEGIN();
    run_counter_tests();
    run_ccd_tests();
    run_vehicleState_tests();
    //return counter_tests;
    //return counter_tests+CCD_tests+state_tests;
    return UNITY_END();
}

#endif
