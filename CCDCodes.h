/*
ID - 0xFF
*/
#define ECHO_ID (0xFF)

/*
ID - MPH - KPH
MPH = Value
KPH = Value
*/
#define SPEED_ID (0x24)

/*
ID - RPM - MAP
RPM = Value * 32
MAP = (Value * 41) / 100
*/
#define RPM_MAP_ID (0xE4)

/*
ID - TPS - Cruise On/Off
Float TPS = (Value * 67) / 100
Cruise = 0x00 (off) or 0x?? (on)
*/
#define CRUISE_ID (0x42)

/*
Pre-OBD-II and/or export vehicles.
ID - Current Voltage - Target Voltage
Float voltage = Value / 16
Float target voltage = Value / 16
*/
#define VOLTAGE_ID (0xD4)

/*
Pre-OBD-II and/or export vehicles.
ID - Coolant Temperature - Battery Temperature
Coolant = Value - 128
Battery = Value - 128
*/
#define TEMPERATURE_ID (0x8C)

/*
ID - 0x000000
*/
#define TRIP_DIST_ID (0xEE)

/*
ID - Shift Light - ??
Shift Light = 0x00 off, 0xFF on
Cruise Light = 0x00 off, 0xFF on
*/
#define SHIFT_CRUISE_LIGHT_ID (0xA4)

/*
ID - Check Gauges Light - ??
Check Gauges Light = 0x00 off, 0xFF on
?? = Unknown
*/
#define CG_LIGHT_ID (0xEC)

/*
ID - Check Engine Light - ??
Check Engine Light = 0x00 off, 0xFF on
?? = Unknown
*/
#define CE_LIGHT_ID (0xF5)

/*
ID - Buzzer
Buzzer = 0x00 off, 0xFF buzz briefly.
*/
#define BUZZER_ID (0x9A)

/*
ID - Airbag Good Status
Status = Any value turns off airbag light.
*/
#define AIRBAG_GOOD_ID (0x50)

/*
ID - Airbag Bad Status
Status = Any value turns on airbag light.
*/
#define AIRBAG_BAD_ID (0x51)

/*
Variable byte width, minimum 4 bytes.  Unknown functionality past byte #3.
ID - Voltage - Oil PSI - Coolant Temperature
Voltage = Value / 8
Oil PSI = Value * 3 = kPa OR Value
Coolant Temperature = Value * 1.3 OR Value / 0.7676
*/
#define VOLTS_OILPSI_COOLTEMP_ID (0x0C)

/*
Increments vehicle odometer and trip odometer.
ID - ?? - Miles Increased
?? = This is most likely kilometers increased.
Miles Increased = Value * 0.3~ OR about 0.0416~ miles per bit.
*/
#define DIST_INCR_ID (0x84)
