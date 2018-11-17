#ifndef UNIT_TEST

#include <main.h>
#include <Arduino.h>
#include "../lib/FlexCAN_Library/FlexCAN.h"
#include <Eventually.h>
#include <counter.h>
#include <CCD.h>
#include <stdint.h>

EvtManager mgr;
CCD ccdBus;
Counter counter;
FlexCAN CANbus(500000);
EvtPinListener *steeringWheelEvt;

/* CCD distance notation is as follows:
          8388 bits in a mile
          5280 feet in mile
          7.55 inches in bit
          .629 feet in bit
          1 bit = 0.00011912879 miles
*/

bool send_ccd_data(){
  //ccdBus.setDistance(counter.getPulses())
  ccdBus.doUpdates();
  return true;
}

bool read_speed(){
  int mph = counter.getSpeed_MPH();
  ccdBus.setMPH(mph);
  //TODO convert MPH to EDC16 8200 ppm
  analogWriteFrequency(VSS_OUT, 133.3333); //133.333 is 60mph
  return true;
}

bool read_distance(){
  float distance = counter.getDistance_miles();
  //TODO teach CCD to send miles to buss
  return true;
}

//sets the oil pressure in percent
bool check_fuel_level(){
  int fuel_level = constrain(map(analogRead(FUEL_LEVEL_IN), FUEL_MIN, FUEL_MAX, 0, 100),0,100);
  ccdBus.setFuelPercent(fuel_level);
  return true;
}

//sets the oil pressure in PSI
bool check_oil_pressure(){
  int oil_pressure = constrain(map(analogRead(OIL_PRESSURE_IN), OIL_PRESSURE_MIN, OIL_PRESSURE_MAX, 0, OIL_PRESSURE_SENSOR_MAX),0,OIL_PRESSURE_SENSOR_MAX);
  ccdBus.setOilPSI(oil_pressure);
  return true;
}

//TODO
bool manage_steering_buttons(){
  mgr.removeListener(steeringWheelEvt);

  //read the steering wheel values
  /*
  cruiseUp = false;
  cruiseDown = false;
  cruiseEnabled = false;
  cruiseSet = false;
  cruiseCancel = false;
  cruiseResume = false;
   */
  ccdBus.setCruiseLight(cruiseEnabled);
  mgr.addListener(steeringWheelEvt);
  return true;
}

void setup() {
  CANbus.begin();
  ccdBus.init(Serial1);
  ccdBus.setSKIMLight(false);
  counter.init();

  //these is all values read from CAN that belong on CCD
  /*
  ccdBus.setVoltage(14);
  ccdBus.setCoolantTemperature(210);
  ccdBus.setCheckGaugesLight(false);
  ccdBus.setCheckEngineLight(false);
  ccdBus.setShiftLight(false);
  */

  //these are all PHYSICAL signals that have CCD outputs
  //ALTERNATOR_HEALTH_IN
  //VSS_IN

  //these are all values read from CAN that have PHYSICAL outputs
  //GLOWPLUG_LAMP_OUT

  //these are all PHYSICAL signals that have PHYSICAL outputs
  //STEERING_WHEEL_IN

  mgr.addListener(new EvtTimeListener(100, true, (EvtAction)read_speed));
  mgr.addListener(new EvtTimeListener(500, true, (EvtAction)read_distance));

  steeringWheelEvt = new EvtPinListener(STEERING_WHEEL_IN, (EvtAction)manage_steering_buttons);
  mgr.addListener(steeringWheelEvt);
  mgr.addListener(new EvtTimeListener(100, true, (EvtAction)send_ccd_data));
  mgr.addListener(new EvtTimeListener(1000, true, (EvtAction)check_oil_pressure));
  mgr.addListener(new EvtTimeListener(1000, true, (EvtAction)check_fuel_level));
}

USE_EVENTUALLY_LOOP(mgr)
#endif
