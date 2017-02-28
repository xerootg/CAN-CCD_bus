//Thanks to oh2nlt for documenting OBD-I information over a decade ago.  Codes listed here were reverse engineered from OBD-II Jeep electronics.

/*
Not actually an ID in usage?  This is the same as an open bus.
ID - 0xFF
*/
#define ECHO_ID (0xFF)

/*
ID - MPH - KPH
MPH = Value * 0.625  Example: 128 * 0.625 = 80 MPH
KPH = Value ? - Need a KPH gauge cluster to test and verify.
*/
#define SPEED_ID (0x24)

/*
Note: The RPM and MAP need to be updated on the CCD bus every few seconds if the engine is not running.
Otherwise the odometer will display "no bus" on late 1990s models after twenty seconds.
ID - RPM - MAP
RPM = Value * 32  Example: 25(DEC) * 32 = 800 RPM
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
ID - Fuel Percent
Float fuel percent = Value / 254
255 = Open sensor circuit
*/
#define FUEL_ID (0x25)

/*
Pre-OBD-II and/or export vehicles.
ID - Coolant Temperature - Battery Temperature
Coolant = Value - 128
Battery = Value - 128
*/
#define TEMPERATURE_ID (0x8C)

/*
Pre-OBD-II and/or export vehicles.
ID - 0x000000
*/
#define TRIP_DIST_ID (0xEE)

/*
ID - Shift Light - Cruise Control
The payload is two bytes long with each bit indicating a different message.  Originally I thought that each byte controlled actually one function, but that assumption was wrong after prompts from laszlodaniel that 0xA4 was being used for more than two functions.  Some single bits control individual features.  However, in the case of the "shift up" light for manual transmissions I have only been able to toggle it by setting the first four bites of the first byte to on.
So far:
First byte:
|              | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
|--------------|---|---|---|---|---|---|---|---|
| Shift Light  | 1 |   |   | 1 |   |   |   |   | 0x90 - Note: One of these bits might be the transmission type identifier and be required for the shift light to also work.

Second byte:
|              | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
|--------------|---|---|---|---|---|---|---|---|
| Cruise Light |   |   |   |   |   | 1 |   |   | 0x08

Additional features controlled, but undetermined what their positions are yet:
* A/C Clutch Engaged
* A/C EMCC Request
* Brake Pedal - I assume this means the brake pedal switched is triggerred.
* Emission Reminder/Status Light
* Engine Light Status - This is duplicated on 0xF5 as such, this might be for OBD-I.
* "SC" Override
* Torque Converter Lockup
* Transmission in Park/Neutral
* Transmission Type(See note on Shift Light bit field above.)
* Lock Door Request
* Water in Fuel (Diesel)
*/
#define FEATURE_STATUS_ID (0xA4)

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
ID - Sentry Key Immobilizer Light
SKIM Light = 0x00 off, 0xFF on (with brief buzz)
*/
#define SKIM_LIGHT_ID (0x0B)

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
Voltage = Value / 8  Example: 112 / 8 = 14 volts
Oil PSI = Value / 2  Example: 160 / 2 = 80 PSI
Coolant Temperature = Uhhhh?  The scaling on XJ cluster is fucky.
*ºF: 100--127.5--155--182.5--210--222.5--235--247.5--260
DEC: 105--120----135--150----165--185----N/A--188+---193
*/
#define VOLTS_OILPSI_COOLTEMP_ID (0x0C)

/*
Increments vehicle odometer and trip odometer.
ID - ?? - Miles Increased
?? = This is most likely kilometers increased.
Miles Increased = Value * 0.3~ OR about 0.0416~ miles per bit.
*/
#define DIST_INCR_ID (0x84)
