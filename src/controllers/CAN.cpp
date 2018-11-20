#include "CAN.h"
#ifndef UNIT_TEST
#include <FlexCAN.h>
#endif

/*
This class will NOT be the hardware interface, but will
provide logic to read the hardware interface view to 
set/get some vehicle state.
*/

CAN::CAN(FlexCAN * _flexCan, VehicleState * _vehicleState){
  this->flexcan = _flexCan;
  this->vehicleState = _vehicleState;
}

void CAN::init(){
  this->flexcan->begin();
}


