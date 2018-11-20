#ifdef UNIT_TEST

#include "tests.h"
#include <unity.h>
#include <ArduinoFake.h>
//#include <iostream>

using namespace fakeit;

CAN* can;
VehicleState* canTestState;


void setUpCan(){
  canTestState = new VehicleState();
  canTestState->init();

  When(Method(ArduinoFake(FlexCAN), begin)).AlwaysReturn();
  FlexCAN* flexCanMock = ArduinoFakeMock(FlexCAN);
 
  can = new CAN(flexCanMock, canTestState);
}

void CAN_holds_valid_state_pointer (){
    VehicleState* canTestStatePtr = can->getStatePointer();
    //std::cout << "State address is: " << canTestStatePtr << " VS " << canTestState << std::endl;
    TEST_ASSERT_TRUE(canTestStatePtr == canTestState)
    canTestStatePtr->setRPMs(1.234);
    TEST_ASSERT_EQUAL_FLOAT(1.234, canTestState->getRPMs());
    TEST_ASSERT_NOT_NULL(can);
}

void run_ccd_tests(){
    RUN_TEST(CAN_holds_valid_state_pointer);
}
#endif
