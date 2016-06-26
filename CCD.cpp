#include "CCD.h"
#include "Vehicles.h"
#include <Arduino.h>
#include <HardwareSerial.h>
#include <Math.h>

CCD::CCD(HardwareSerial& serial) : ccdBus(serial) {
	this->ccdBus.begin(this->ccdBaud);
}

void CCD::setRPM(float rpm) {
	int newRPM = round(constrain(rpm / 32, 0, 255));
	if (newRPM != this->rpm) {
		this->rpm = newRPM;
		this->needsUpdateRPM = true;
	}
}

void CCD::setMPH(float mph) {
	//float kph = mph * 1.60934400061; //Update KPH at the same time for consistency.
	int newMPH = round(constrain(mph * SPEED_MULTIPLIER, 0, 255));
	if (newMPH != this->mph) {
		this->mph = newMPH;
		this->needsUpdateSpeed = true;
	}
}

void CCD::setKPH(float kph) {
	//Note: This functionality is incomplete due to needing a KPH cluster for testing.
	//float mph = kph * 0.621371192; //Update MPH at the same time for consistency.
	int newKPH = round(constrain(kph, 0, 255));
	if (newKPH != this->kph) {
		this->kph = newKPH;
		this->needsUpdateSpeed = true;
	}
}

void CCD::setOilPSI(float oilPsi) {
	int newPSI = round(constrain(oilPsi * 2, 0, 255));
	if (newPSI != this->oilPsi) {
		this->oilPsi = newPSI;
		this->needsUpdateHealth = true;
	}
}

void CCD::setVoltage(float voltage) {
	int newVoltage = round(constrain(voltage * 8, 0, 255));
	if (newVoltage != this->voltage) {
		this->voltage = newVoltage;
		this->needsUpdateHealth = true;
	}
}

void CCD::setCoolantTemperature(float tempF) {
	int hex = 0xC1; //193 - Also, fail safe to alerting in the red for temperature.
	//If you are looking at this code for temperature ranges and thinking, "WTF", so am I.
	if (tempF < 100) {
		hex = 0x00;
	} else if (tempF >= 100 && tempF <= 210) {
		hex = ((tempF - 100) * 0.54545455) + 105;
	} else if (tempF > 210 && tempF < 223) {
		hex = ((tempF - 210) * 1.6) + 165;
	} else if (tempF >= 223 && tempF < 248) {
		hex = 0xBC; //188
	}
	int newTemp = round(constrain(hex, 0, 255));

	if (newTemp != this->coolantTemperature) {
		this->coolantTemperature = newTemp;
		this->needsUpdateHealth = true;
	}
}

bool CCD::doUpdates() {
	return true;
}