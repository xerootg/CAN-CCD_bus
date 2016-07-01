#include <FlexCAN.h>

class CAN {
public:
	void init();
	virtual float getRPM() = 0;
	virtual float getMPH() = 0;
	virtual float getKPH() = 0;
	virtual float getOilPSI() = 0;
	virtual float getVoltage() = 0;
	virtual float getCoolantTemperature() = 0;
	virtual bool getCheckEngineLight() = 0;
	virtual bool getCheckGaugesLight() = 0;
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