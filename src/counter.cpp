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
static Counter * isr_ptr; //pointer to hold reference to Counter class for the ISR to hook into
void Counter::init(){
  this->distancePulses = 0;
  this->speedPulses = 0;
  this->time = 0;
  isr_ptr = this;

  // Input filter to help prevent glitches from triggering the capture
  // 4+4×val clock cycles, 48MHz = 4+4*7 = 32 clock cycles = 0.75us
  FTM0_FILTER = 0x07;

  // Must set the Write-protect disable (WPDIS) bit to allow modifying other registers
  // The enable (FTMEN) bit is also set to enable the FlexTimer0 module
  // FAULTIE=0, FAULTM=00, CAPTEST=0, PWMSYNC=0, WPDIS=1, INIT=0, FTMEN=1
  FTM0_MODE = 0x05;

  // FLEXTimer0 configuration
  // Clock source is Fixed Frequency Clock running at 31.25kHz (FLL Clock input = MCGFFCLK)
  // Dividing that by 2 would have the counter roll over about every 4 seconds
  FTM0_SC = 0x00; // Set this to zero before changing the modulus
  FTM0_CNT = 0x0000; // Reset the count to zero
  FTM0_MOD = 0xFFFF; // max modulus = 65535
  FTM0_SC = 0x11; // TOF=0 TOIE=0 CPWMS=0 CLKS=10 (FF clock) PS=001 (divide by 2)
  FTM0_C0SC = 0x48; // CHF=0 CHIE=1 (enable interrupt) MSB=0 MSA=0 ELSB=1 (input capture) ELSA=0 DMA=0
  NVIC_ENABLE_IRQ(IRQ_FTM0);   // Enable FTM0 interrupt inside NVIC
  CORE_PIN22_CONFIG = PORT_PCR_MUX(4);
  //PORTC_PCR1 |= 0x400; // PIN configuration, alternative function 4 on Pin 44 (Teensy 22) (FTM0_CH0)
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

void ftm0_isr(void){
  isr_ptr->setCount(); //one rotation has occured
  FTM0_CNT = 0x0000; // Reset count value
  isr_ptr->setTime(FTM0_C0V/(31250.0/2));   //TODO this number is in seconds, not milliseconds.

  if ((FTM0_SC&FTM_SC_TOF) != 0) {   // Read the timer overflow flag (TOF) in the status and control register (FTM0_SC)
    FTM0_SC &= ~FTM_SC_TOF; //reset overflow
  }
  FTM0_C0SC &= ~0x80; // clear channel interrupt flag (CHF)
}
