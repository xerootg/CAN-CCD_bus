#include "CAN.h"
#include <FlexCAN.h>

class HaltechCAN: public CAN {
public:
	void init();
	float getRPM() = 0;
	float getMPH() = 0;
	float getKPH() = 0;
	float getOilPSI() = 0;
	float getVoltage() = 0;
	float getCoolantTemperature() = 0;
	bool getCheckEngineLight() = 0;
	bool getCheckGaugesLight() = 0;
	bool getUpdates();
	void busRecieve();
private:
	float rpm = 0.0;
	float mph = 0.0;
	float kph = 0.0;
	float oilPsi = 0.0;
	float voltage = 0.0;
	float coolantTemperature = 0.0;
};