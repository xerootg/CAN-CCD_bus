#ifndef UNIT_TEST
#include <Arduino.h>
#else
#include <fake_arduino.h>
#endif

#include <counter.h>
#include <math.h>

//Implementation from http://www.digitalmisery.com/2013/06/timer-input-capture-on-teensy-3-0/
//and https://github.com/cTn-dev/Phoenix-FlightController/blob/master/libraries/P_Receiver/Receiver_teensy3_HW_PPM.h

/*
TIMER ISR hooks into two places:
  setTime(ms) and setCount();

  setTime(ms) sets the duration for the measured getPulses.
  setCount() sets a pulse for each pulse OR setCount(pulses) sets the number of pulses.
*/


//get the counter class ready for counting
void Counter::init(){
  this->distancePulses = 0;
  this->speedPulses = 0;
  this->time = 0;
};

//reset the distance, i.e. after setting both speed and distance in the same round of updates
void Counter::resetDistance(){
  this->distancePulses = 0;
}

//increase the distance pulses
void Counter::setCount(){
  this->distancePulses++;
  this->speedPulses++;
}

void Counter::setCount(int pulses){
  this->distancePulses+=pulses;
  this->speedPulses+=pulses;
}

//get the distance traveled in miles
float Counter::getDistance_miles(){
  //8000 ppm - pulses/8000=miles
  float miles = (float)this->distancePulses/8000.0;
  this->resetDistance();
  return miles;
}

float Counter::getDistance_miles(int pulses){
  //8000 ppm - pulses/8000=miles
  return (float)pulses/8000.0;
}

//given some time (ms) return the speed
float Counter::getSpeed_MPH(int ms){
  // 3,600,000 ms in hour
  float speed = getDistance_miles()*(3600000/(float)ms);
  if(isnan(speed)){
    return 0;
  } else {
    return speed;
  }
}
//given no args, use time in clock, then reset clock.
float Counter::getSpeed_MPH(){
  float speed = getDistance_miles(this->speedPulses)*(3600000/(float)this->getTime());
  this->speedPulses = 0;
  this->resetTime();
  if(isnan(speed)){
    return 0;
  } else {
    return speed;
  }
}

void Counter::setTime(int ms){
  this->time += ms;
}

int Counter::getTime(){
  return this->time;
}

void Counter::resetTime(){
  this->time = 0;
}

//peek at the pulses in this round of distance
int Counter::getPulses(){
  return this->distancePulses;
};
