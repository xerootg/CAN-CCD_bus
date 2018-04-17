//pin definitions
#define CCD_RXD 0
#define CCD_TXD 1
#define CCD_IDLE 2 //RISING = message incoming, FALLING = message ending
#define CAN_TX 3
#define CAN_RX 4
#define CRUISE_SET_OUT 5
#define CRUISE_RESUME_OUT 6
#define CRUISE_ENABLE_OUT 7
#define GLOWPLUG_LAMP_OUT 8
#define VSS_OUT 9
#define FUEL_LEVEL_IN 14
#define OIL_PRESSURE_IN 16
#define STEERING_WHEEL_IN 17
#define ALTERNATOR_HEALTH_IN 23
#define VSS_IN 22
#define MISC_OUT 21 //TC4427 output

//Fuel level trims
#define FUEL_MAX 1023
#define FUEL_MIN 0

//oil pressure trims
#define OIL_PRESSURE_MAX 1023 //this is whatever MAX oil pressure is (ADC)
#define OIL_PRESSURE_MIN 0 //this is whatever 0 PSI is (ADC)
#define OIL_PRESSURE_SENSOR_MAX 90 //this is what the sensor pressure is at OIL_PRESSURE_MAX

bool cruiseUp = false;
bool cruiseDown = false;
bool cruiseEnabled = false;
bool cruiseSet = false;
bool cruiseCancel = false;
bool cruiseResume = false;
