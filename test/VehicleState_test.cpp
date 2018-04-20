#ifdef UNIT_TEST
#include "tests.h"
#include <unity.h>

VehicleState* state;

void setUpVehicleState(){
  state = new VehicleState();
}

void vehicleState_tests_load (){
  TEST_ASSERT_EQUAL_INT(1,state->init());
}

int run_vehicleState_tests(){
    UNITY_BEGIN();
    RUN_TEST(vehicleState_tests_load);
    return UNITY_END();
}
#endif
