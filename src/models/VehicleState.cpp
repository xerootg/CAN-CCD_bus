#include "VehicleState.h"

void VehicleState::init(){
  this->volts = 0.0;
  this->fuelLevel = 0.0;
  this->tankCapacity = 30;
  this->kph = 0.0;
  this->mph = 0.0;
  this->rpm = 0;
  this->psi = 0.0;
  this->deg_c = 0.0;
  this->deg_f = 32.0;
  this->pats_illuminated = false;
  this->cruise_illuminated = false;
  this->shift_illuminated = false;
  this->cel_illuminated = false;
  this->checkGauges_illuminated = false;
  this->airbag_illuminated = false;
  }

float VehicleState::getVolts(){
  return this->volts;
}

void VehicleState::setVolts(float volts){
  this->volts=volts;
}

int VehicleState::getFuelLevel(){
  return this->fuelLevel;
}

void VehicleState::setFuelLevel(int fuelLevel){
  this->fuelLevel = fuelLevel;
}

float VehicleState::getFuelLeters(){
  float actualPercentage = this->fuelLevel/100;
  float gallons = tankCapacity * actualPercentage;
  float leters = gallons * 3.79;
  return leters;
}

void VehicleState::setSpeedMPH(float mph){
   this->mph = mph;
   this->kph = mph * 1.60934;
 }
float VehicleState::getSpeedMPH(){
  return this->mph;
}
float VehicleState::getSpeedKPH(){
  return this->kph;
}

void VehicleState::setRPMs(int rpm){
  this->rpm = rpm;
}

int VehicleState::getRPMs(){
  return this->rpm;
}

void VehicleState::setOilPSI(float psi){
  this->psi = psi;
}

float VehicleState::getOilPSI(){
  return this->psi;
}

void VehicleState::setTempF(float deg_f){
  this->deg_f = deg_f;
  this->deg_c = (deg_f - 32)*5/9;
}

void VehicleState::setTempC(float deg_c){
  this->deg_c = deg_c;
  this->deg_f = deg_c*9/5 + 32;
}

float VehicleState::getTempF(){
  return this->deg_f;
}

float VehicleState::getTempC(){
  return this->deg_c;
}

bool VehicleState::getPats(){
  return this->pats_illuminated;
}

void VehicleState::setPats(bool pats_illuminated){
  this->pats_illuminated = pats_illuminated;
}

void VehicleState::patsOn(){
  this->setPats(true);
}

void VehicleState::patsOff(){
  this->setPats(false);
}

bool VehicleState::getCruiseLamp(){
  return this->cruise_illuminated;
}

void VehicleState::setCruiseLamp(bool status){
  this->cruise_illuminated = status;
}

void VehicleState::cruiseLampOn(){
  this->setCruiseLamp(true);
}

void VehicleState::cruiseLampOff(){
  this->setCruiseLamp(false);
}

bool VehicleState::getShiftLamp(){
  return this->shift_illuminated;
}

void VehicleState::setShiftLamp(bool status){
  this->shift_illuminated = status;
}

void VehicleState::shiftLampOn(){
  this->setShiftLamp(true);
}

void VehicleState::shiftLampOff(){
  this->setShiftLamp(false);
}

bool VehicleState::getCel(){
  return this->cel_illuminated;
}

void VehicleState::setCel(bool status){
  this->cel_illuminated = status;
}

void VehicleState::celOn(){
  this->setCel(true);
}

void VehicleState::celOff(){
  this->setCel(false);
}

bool VehicleState::getCheckGauges(){
  return this->checkGauges_illuminated;
}

void VehicleState::setCheckGauges(bool state){
  this->checkGauges_illuminated = state;
}

void VehicleState::checkGaugesOff(){
  this->setCheckGauges(false);
}

void VehicleState::checkGaugesOn(){
  this->setCheckGauges(true);
}

bool VehicleState::getAirbagLamp(){
  return this->airbag_illuminated;
}

void VehicleState::setAirbagLamp(bool state){
  this->airbag_illuminated = state;
}

void VehicleState::airbagLampOff(){
  this->setAirbagLamp(false);
}

void VehicleState::airbagLampOn(){
  this->setAirbagLamp(true);
}