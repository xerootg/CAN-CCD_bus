#include <HardwareSerial.h>

class CCD {
public:
	CCD(HardwareSerial& serial);
	HardwareSerial& ccdBus;
	void setRPM(float rpm);
	void setMPH(float mph);
	void setKPH(float kph);
	void setOilPSI(float oilPsi);
	void setVoltage(float voltage);
	void setCoolantTemperature(float tempF);
	bool doUpdates();
private:
	const int ccdBaud = 7812; //Due to how AVRs work this ends up being the needed 7812.5 assuming the MHz has no remainder when divided by 1.
	//Gauge values stored as a byte.
	int rpm;
	int mph;
	int kph;
	int oilPsi;
	int voltage;
	int coolantTemperature;
	bool needsUpdateRPM = false;
	bool needsUpdateSpeed = false;
	bool needsUpdateHealth = false;
};