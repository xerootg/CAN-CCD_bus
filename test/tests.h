#ifdef UNIT_TEST
#include "../src/controllers/counter.h"
#include "../src/controllers/CAN.h"
#include <ArduinoFake.h>
#ifndef __VehicleState_h
#include "../src/models/VehicleState.h"
#endif

void run_counter_tests();
void run_ccd_tests();
void run_vehicleState_tests();
void setUpCan();
void setUpCounter();
void setUpVehicleState();

#endif