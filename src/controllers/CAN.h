#ifndef __VehicleState_h
#include "../models/VehicleState.h"
#endif
#include <FlexCAN.h>

class CAN {
public:
  CAN(FlexCAN* flexcan, VehicleState* vehicleState);
  void init();
  VehicleState* getStatePointer(){return this->vehicleState;}
private:
  FlexCAN* flexcan;
  VehicleState* vehicleState;
};
