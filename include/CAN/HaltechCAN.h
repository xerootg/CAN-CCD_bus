#include "CAN.h"

class HaltechCAN: public CAN {
public:
	float getRPM();
	float getMPH();
	float getKPH();
	float getOilPSI();
	float getVoltage();
	float getCoolantTemperature();
	bool getCheckEngineLight();
	bool getCheckGaugesLight();
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