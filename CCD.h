#include <HardwareSerial.h>

class CCD {
public:
	CCD();
	void init(HardwareSerial& serial);
	void setRPM(float rpm);
	void setMPH(float mph);
	void setKPH(float kph);
	void setOilPSI(float oilPsi);
	void setVoltage(float voltage);
	void setCoolantTemperature(float tempF);
	bool doUpdates();
private:
	HardwareSerial ccdBus;
	const int ccdBaud = 7812; //Due to how AVRs work this ends up being the needed 7812.5 assuming the MHz has no remainder when divided by 1.
	//Gauge values stored as a byte.
	int rpm = 0x00;
	int mph = 0x00;
	int kph = 0x00;
	int oilPsi = 0x00;
	int voltage = 0x00;
	int coolantTemperature = 0x00;
	bool needsUpdateRPM = false;
	bool needsUpdateSpeed = false;
	bool needsUpdateHealth = false;
	void busTransmit(int id, int numBytes, ...);
};