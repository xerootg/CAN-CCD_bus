#include <FlexCAN.h>

class CAN {
public:
	float init();
	float getRPM();
	float getMPH();
	float getKPH();
	float getOilPSI();
	float getVoltage();
	float getCoolantTemperature();
	bool getCheckEngineLight();
	bool getCheckGaugesLight();
	virtual bool getUpdates() = 0;
	virtual void busRecieve() = 0;
private:
	FlexCAN canBus;
	float rpm = 0.0;
	float mph = 0.0;
	float kph = 0.0;
	float oilPsi = 0.0;
	float voltage = 0.0;
	float coolantTemperature = 0.0;
	bool checkEngineLight = false;
	bool checkGaugesLight = false;
};