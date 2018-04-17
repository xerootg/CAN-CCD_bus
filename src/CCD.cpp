#ifndef UNIT_TEST

#include "CCD.h"
#include "CCDCodes.h"
#define SPEED_MULTIPLIER 1.59
#include <Arduino.h>
#include <HardwareSerial.h>
#include <Math.h>

/*
 * Initialize serial bus and other necessary information.
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
 * Set new fuel percentage.
 *
 * @access	public
 * @param	float	Fuel Percentage
 * @return	void
 */
void CCD::setFuelPercent(float fuelPercent) {
	int newFuelPercent = round(constrain(254 * (fuelPercent / 100), 0, 254));
	if (newFuelPercent != this->fuelPercent) {
		this->fuelPercent = newFuelPercent;
		this->needsUpdateFuelPercent = true;
	}
}

/*
 * Turn check engine light on or off.
 *
 * @access	public
 * @param	boolean	True to turn on.
 * @return	boolean	Previous on/off state.
 */
bool CCD::setCheckEngineLight(bool on) {
	bool previousState = this->checkEngineLightOn;
	if (on != previousState) {
		this->needsUpdateLights = true;
	}
	this->checkEngineLightOn = on;
	return previousState;
}

/*
 * Turn check gauges light on or off.
 *
 * @access	public
 * @param	boolean	True to turn on.
 * @return	boolean	Previous on/off state.
 */
bool CCD::setCheckGaugesLight(bool on) {
	bool previousState = this->checkGaugesLightOn;
	if (on != previousState) {
		this->needsUpdateLights = true;
	}
	this->checkGaugesLightOn = on;
	return previousState;
}

/*
 * Turn air bag light on or off.
 *
 * @access	public
 * @param	boolean	True to turn on.
 * @return	boolean	Previous on/off state.
 */
bool CCD::setAirBagLight(bool on) {
	bool previousState = this->airBagLightOn;
	if (on != previousState) {
		this->needsUpdateLights = true;
	}
	this->airBagLightOn = on;
	return previousState;
}

/*
 * Turn skim light on or off.
 *
 * @access	public
 * @param	boolean	True to turn on.
 * @return	boolean	Previous on/off state.
 */
bool CCD::setSKIMLight(bool on) {
	bool previousState = this->skimLightOn;
	if (on != previousState) {
		this->needsUpdateLights = true;
	}
	this->skimLightOn = on;
	return previousState;
}

/*
 * Turn shift light on or off.
 *
 * @access	public
 * @param	boolean	True to turn on.
 * @return	boolean	Previous on/off state.
 */
bool CCD::setShiftLight(bool on) {
	bool previousState = this->shiftLightOn;
	if (on != previousState) {
		this->needsUpdateLights = true;
	}
	this->shiftLightOn = on;
	return previousState;
}

/*
 * Turn cruise light on or off.
 *
 * @access	public
 * @param	boolean	True to turn on.
 * @return	boolean	Previous on/off state.
 */
bool CCD::setCruiseLight(bool on) {
	bool previousState = this->cruiseLightOn;
	if (on != previousState) {
		this->needsUpdateLights = true;
	}
	this->cruiseLightOn = on;
	return previousState;
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
 * Do updates to CCD bus.
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
		//XJ gauge clusters continually hold the last known value.
		this->busTransmit(VOLTS_OILPSI_COOLTEMP_ID, 4, this->voltage, this->oilPsi, this->coolantTemperature, 0xFF);
		this->needsUpdateHealth = false;
		didUpdates = true;

		if (this->needsUpdateFuelPercent) {
			delay(50);
		}
	}

	if (this->needsUpdateFuelPercent) {
		this->busTransmit(FUEL_ID, 1, this->fuelPercent);
		this->needsUpdateFuelPercent = false;
		didUpdates = true;

		if (this->needsUpdateLights) {
			delay(50);
		}
	}

	if (this->needsUpdateLights) {
		this->doUpdateLights();
		didUpdates = true;
	}

	return didUpdates;
}

/*
 * Do updates to CCD bus for status lights only.
 *
 * @access	public
 * @return	void
 */
void CCD::doUpdateLights() {
	//@TODO: Fix feature status to assemble bits correctly.
	this->busTransmit(FEATURE_STATUS_ID, 2, this->boolToLight(this->shiftLightOn), this->boolToLight(this->cruiseLightOn));
	delay(50);
	this->busTransmit(SKIM_LIGHT_ID, 1, this->boolToLight(this->skimLightOn));
	delay(50);
	this->busTransmit(CG_LIGHT_ID, 2, this->boolToLight(this->checkGaugesLightOn), 0x00);
	delay(50);
	this->busTransmit(CE_LIGHT_ID, 2, this->boolToLight(this->checkEngineLightOn), 0x00);
	delay(50);
	if (this->airBagLightOn) {
		this->busTransmit(AIRBAG_BAD_ID, 1, 0xFF);
	} else {
		this->busTransmit(AIRBAG_GOOD_ID, 1, 0xFF);
	}
	this->needsUpdateLights = false;
}

/*
 * Transmit to the serial port.
 *
 * @access	private
 * @return	void
 */
 //TODO
void CCD::busTransmit(int id, int numBytes, ...) {
	va_list bytes;
	va_start(bytes, numBytes);

	int checksum = id;
	//ONLY DO THIS IF BUS IS NOT BUSY, ELSE WAIT
	this->ccdBus.write(id);
	for (int i = 0; i < numBytes; ++i) {
		int toSend = va_arg(bytes, int);
		this->ccdBus.write(toSend);
		checksum += toSend;
	}

	va_end(bytes);
	checksum = checksum & 0xFF;
	this->ccdBus.write(checksum);
}

/*
 * Turn a boolean into 0 or 255 to turn a light off or on.
 *
 * @access	private
 * @return	integer	0 or 255
 */
int CCD::boolToLight(bool on) {
	if (on) {
		return 255;
	}
	return 0;
}

#endif
