#ifdef UNIT_TEST
#include "tests.h"
#include <unity.h>

VehicleState* state;

void setUpVehicleState(){
  state = new VehicleState();
  state->init();
}

void VehicleState_tests_load(){
  TEST_ASSERT_EQUAL_FLOAT(0.0, state->getVolts());
  TEST_ASSERT_EQUAL_FLOAT(0.0, state->getFuelLevel());
  TEST_ASSERT_EQUAL_FLOAT(0.0, state->getFuelLeters());
  TEST_ASSERT_EQUAL_FLOAT(0.0, state->getSpeedKPH());
  TEST_ASSERT_EQUAL_FLOAT(0.0, state->getSpeedMPH());
  TEST_ASSERT_EQUAL_INT(0, state->getRPMs());
  TEST_ASSERT_EQUAL_FLOAT(0.0, state->getOilPSI());
  TEST_ASSERT_EQUAL_FLOAT(0.0, state->getTempC());
  TEST_ASSERT_EQUAL_FLOAT(32.0, state->getTempF());
  TEST_ASSERT_FALSE(state->getPats());
  TEST_ASSERT_FALSE(state->getCruiseLamp());
  TEST_ASSERT_FALSE(state->getShiftLamp());
  TEST_ASSERT_FALSE(state->getCel());
  TEST_ASSERT_FALSE(state->getCheckGauges());
  TEST_ASSERT_FALSE(state->getAirbagLamp());
}

void can_manage_volts(){
  state->setVolts(14.7);
  TEST_ASSERT_EQUAL_FLOAT(14.7,state->getVolts());

}

void can_manage_fuel(){
  state->setFuelLevel(100);
  TEST_ASSERT_EQUAL_INT(100,state->getFuelLevel());

  //the leters conversion is... rough.
  TEST_ASSERT_EQUAL_FLOAT(113.7, state->getFuelLeters());
  state->setFuelLevel(50);
  TEST_ASSERT_EQUAL_FLOAT(56.85, state->getFuelLeters());
}

void can_manage_speed(){
  state->setSpeedMPH(100);
  TEST_ASSERT_EQUAL_FLOAT(100.0, state->getSpeedMPH());
  TEST_ASSERT_EQUAL_FLOAT(160.934, state->getSpeedKPH());
  state->setSpeedMPH(50);
  TEST_ASSERT_EQUAL_FLOAT(50.0, state->getSpeedMPH());
  TEST_ASSERT_EQUAL_FLOAT(80.4672, state->getSpeedKPH());

}

void can_manage_engine_speed(){
state->setRPMs(100);
TEST_ASSERT_EQUAL_INT(100, state->getRPMs());
state->setRPMs(7000);
TEST_ASSERT_EQUAL_INT(7000, state->getRPMs());
}

void can_manage_oil_pressure(){
  state->setOilPSI(40.0);
  TEST_ASSERT_EQUAL_FLOAT(40.0, state->getOilPSI());
}

void can_manage_engine_temperature(){
state->setTempC(100.0);
TEST_ASSERT_EQUAL_FLOAT(212.0, state->getTempF());
TEST_ASSERT_EQUAL_FLOAT(100, state->getTempC());

state->setTempC(0);
TEST_ASSERT_EQUAL_FLOAT(32, state->getTempF());
TEST_ASSERT_EQUAL_FLOAT(0, state->getTempC());

state->setTempF(212.0);
TEST_ASSERT_EQUAL_FLOAT(212.0, state->getTempF());
TEST_ASSERT_EQUAL_FLOAT(100, state->getTempC());

state->setTempF(32.0);
TEST_ASSERT_EQUAL_FLOAT(32, state->getTempF());
TEST_ASSERT_EQUAL_FLOAT(0, state->getTempC());
}

void can_manage_PATS(){
  state->patsOn();
  TEST_ASSERT_TRUE(state->getPats());

  state->patsOff();
  TEST_ASSERT_FALSE(state->getPats());

  state->setPats(true);
  TEST_ASSERT_TRUE(state->getPats());

  state->setPats(false);
  TEST_ASSERT_FALSE(state->getPats());
}

void can_manage_cruise_lamp(){
  state->cruiseLampOn();
  TEST_ASSERT_TRUE(state->getCruiseLamp());

  state->cruiseLampOff();
  TEST_ASSERT_FALSE(state->getCruiseLamp());

  state->setCruiseLamp(true);
  TEST_ASSERT_TRUE(state->getCruiseLamp());

  state->setCruiseLamp(false);
  TEST_ASSERT_FALSE(state->getCruiseLamp());
}

void can_manage_shift_lamp(){
  state->shiftLampOn();
  TEST_ASSERT_TRUE(state->getShiftLamp());

  state->shiftLampOff();
  TEST_ASSERT_FALSE(state->getShiftLamp());

  state->setShiftLamp(true);
  TEST_ASSERT_TRUE(state->getShiftLamp());

  state->setShiftLamp(false);
  TEST_ASSERT_FALSE(state->getShiftLamp());
}

void can_manage_CEL(){
  state->celOn();
  TEST_ASSERT_TRUE(state->getCel());

  state->celOff();
  TEST_ASSERT_FALSE(state->getCel());

  state->setCel(true);
  TEST_ASSERT_TRUE(state->getCel());

  state->setCel(false);
  TEST_ASSERT_FALSE(state->getCel());

}

void can_manage_checkGauges_lamp(){
  state->checkGaugesOn();
  TEST_ASSERT_TRUE(state->getCheckGauges());

  state->checkGaugesOff();
  TEST_ASSERT_FALSE(state->getCheckGauges());

  state->setCheckGauges(true);
  TEST_ASSERT_TRUE(state->getCheckGauges());

  state->setCheckGauges(false);
  TEST_ASSERT_FALSE(state->getCheckGauges());
}

void can_manage_airbag_lamp(){
  state->airbagLampOn();
  TEST_ASSERT_TRUE(state->getAirbagLamp());

  state->airbagLampOff();
  TEST_ASSERT_FALSE(state->getAirbagLamp());

  state->setAirbagLamp(true);
  TEST_ASSERT_TRUE(state->getAirbagLamp());

  state->setAirbagLamp(false);
  TEST_ASSERT_FALSE(state->getAirbagLamp());
}

void run_vehicleState_tests(){
    RUN_TEST(VehicleState_tests_load);
    RUN_TEST(can_manage_volts);
    RUN_TEST(can_manage_fuel);
    RUN_TEST(can_manage_speed);
    RUN_TEST(can_manage_engine_speed);
    RUN_TEST(can_manage_oil_pressure);
    RUN_TEST(can_manage_engine_temperature);
    RUN_TEST(can_manage_PATS);
    RUN_TEST(can_manage_cruise_lamp);
    RUN_TEST(can_manage_shift_lamp);
    RUN_TEST(can_manage_CEL);
    RUN_TEST(can_manage_checkGauges_lamp);
    RUN_TEST(can_manage_airbag_lamp);
}
#endif
