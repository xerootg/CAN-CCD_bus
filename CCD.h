#include <HardwareSerial.h>
#define SPEED_MULTIPLIER .95
#define VOLTAGE_OFFSET .25
#define DEBUG false
class CCD {
public:
	void init(HardwareSerial& serial, int pin);
	void setRPM(float rpm);
	void setMPH(float mph);
	void setKPH(float kph);
	void setOilPSI(float oilPsi);
	void setVoltage(float voltage);
	void setFuelPercent(float fuelPercent);
	void setCoolantTemperature(float tempF);
	bool setCheckEngineLight(bool on);
	bool setCheckGaugesLight(bool on);
	bool setAirBagLight(bool on);
	bool setSKIMLight(bool on);
	bool setShiftLight(bool on);
	bool setCruiseLight(bool on);
	void setDistancePulses(int pulses);
	bool busTransmit(int id, int numBytes, ...);
	bool busTransmitValidate(int id, int numBytes, ...);
	bool doUpdates();
	void doDistanceUpdates();
	void doUpdateLights();
private:
	HardwareSerial ccdBus;
	const int ccdBaud = 7812; //Due to how AVRs work this ends up being the needed 7812.5 assuming the MHz has no remainder when divided by 1.
	//Gauge values stored as a byte.
	int rpm = 0x00;
	int idlePin = 0;
	int mph = 0x00;
	int kph = 0x00;
	int pulses = 0;
	int pulseTime = 0;
	int pulseMillis = 0;
	int oilPsi = 0x00;
	int voltage = 0x00;
	int fuelPercent = 0x00;
	int coolantTemperature = 0x00;
	bool needsUpdateRPM = false;
	bool needsUpdateSpeed = false;
	bool needsUpdateHealth = false;
	bool needsUpdateFuelPercent = false;
	bool needsUpdateLights = false;
	bool needsDistanceSet = false;
	bool checkEngineLightOn = false;
	bool checkGaugesLightOn = false;
	bool airBagLightOn = true;
	bool skimLightOn = false;
	bool shiftLightOn = false;
	bool cruiseLightOn = false;
	int boolToLight(bool on);
};
