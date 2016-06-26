#include "CCD.h"
#include "CCDCodes.h"
#include "Vehicles.h"
#include <Arduino.h>
#include <HardwareSerial.h>
#include <Math.h>

/*
 * Constructor
 *
 * @access	public
 * @return	void
 */
CCD::CCD() {
	//Nothing.
}

/*
 * Constructor
 *
 * @access	public
 * @param	object	HardwareSerial reference - Typically Serial1 on Arduino and Teensy
 * @return	void
 */
void CCD::init(HardwareSerial& serial) {
	this->ccdBus = serial;
	this->ccdBus.begin(this->ccdBaud);
}

/*
 * Set new engine revolutions per minute.
 *
 * @access	public
 * @param	float	RPM
 * @return	void
 */
void CCD::setRPM(float rpm) {
	int newRPM = round(constrain(rpm / 32, 0, 255));
	if (newRPM != this->rpm) {
		this->rpm = newRPM;
		this->needsUpdateRPM = true;
	}
}

/*
 * Set new speed in miles per hour.
 *
 * @access	public
 * @param	float	Speed in MPH
 * @return	void
 */
void CCD::setMPH(float mph) {
	//float kph = mph * 1.60934400061; //Update KPH at the same time for consistency.
	int newMPH = round(constrain(mph * SPEED_MULTIPLIER, 0, 255));
	if (newMPH != this->mph) {
		this->mph = newMPH;
		this->needsUpdateSpeed = true;
	}
}

/*
 * Set new speed in kilometers per hour.
 * Note: This functionality is incomplete due to needing a KPH cluster for testing.
 *
 * @access	public
 * @param	float	Speed in KPH
 * @return	void
 */
void CCD::setKPH(float kph) {
	//float mph = kph * 0.621371192; //Update MPH at the same time for consistency.
	int newKPH = round(constrain(kph, 0, 255));
	if (newKPH != this->kph) {
		this->kph = newKPH;
		this->needsUpdateSpeed = true;
	}
}

/*
 * Set new oil pressure in pounds per square inch.
 *
 * @access	public
 * @param	float	Oil PSI
 * @return	void
 */
void CCD::setOilPSI(float oilPsi) {
	int newPSI = round(constrain(oilPsi * 2, 0, 255));
	if (newPSI != this->oilPsi) {
		this->oilPsi = newPSI;
		this->needsUpdateHealth = true;
	}
}

/*
 * Set new voltage.
 *
 * @access	public
 * @param	float	Voltage
 * @return	void
 */
void CCD::setVoltage(float voltage) {
	int newVoltage = round(constrain(voltage * 8, 0, 255));
	if (newVoltage != this->voltage) {
		this->voltage = newVoltage;
		this->needsUpdateHealth = true;
	}
}

/*
 * Set new coolant temperature
 *
 * @access	public
 * @param	float	Temperature in fahrenheit.
 * @return	void
 */
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

/*
 * Do updates to CDD bus.
 *
 * @access	public
 * @return	boolean	Update performed.
 */
bool CCD::doUpdates() {
	bool didUpdates = false;

	//RPM has to be updated every tick, even with the engine off, to prevent the "no bus" error after twenty seconds.
	this->busTransmit(RPM_MAP_ID, 2, this->rpm, 0x00); //Updating MAP not yet implemented.
	this->needsUpdateRPM = false;
	didUpdates = true;

	delay(50);

	//Speed also has to be updated within about three seconds or it falls back to zero.
	this->busTransmit(SPEED_ID, 2, this->mph, this->kph);
	this->needsUpdateSpeed = false;
	didUpdates = true;

	delay(50);

	if (this->needsUpdateHealth) {
		this->busTransmit(VOLTS_OILPSI_COOLTEMP_ID, 4, this->voltage, this->oilPsi, this->coolantTemperature, 0x00);
		this->needsUpdateHealth = false;
		didUpdates = true;
	}

	return didUpdates;
}

/*
 * Do updates to CDD bus.
 *
 * @access	private
 * @return	boolean	Update performed.
 */
void CCD::busTransmit(int id, int numBytes, ...) {
	va_list bytes;
	va_start(bytes, numBytes);

	int checksum = id;
	Serial.print("SEND: ");
	Serial.println(id, HEX);
	this->ccdBus.write(id);
	Serial.println(numBytes);
	for (int i = 0; i < numBytes; ++i) {
		int toSend = va_arg(bytes, int);
		this->ccdBus.write(toSend);
		Serial.println(toSend, HEX);
		checksum += toSend;
	}

	va_end(bytes);
	Serial.println(checksum, HEX);
	checksum = checksum & 0xFF;
	this->ccdBus.write(checksum);
}