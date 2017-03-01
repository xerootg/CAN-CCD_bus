#include "CCD.h"
#include "CCDCodes.h"
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
void CCD::init(HardwareSerial& serial, int pin) {
	this->ccdBus = serial;
	this->ccdBus.begin(this->ccdBaud);
	this->idlePin = pin;
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
* Set pulses recieved from the 8000pulse per mile sensor and store time since last update for speed calculation
* 
* @access public
* @param int pulses
* return void
*/

void CCD::setDistancePulses(int pulses) {
	//if the gauges have not seen this count, need to add, else need to set
	int newPulses;
	if (!this->needsDistanceSet) {
		newPulses = pulses;
	}else{
		newPulses = this->pulses + pulses;
	}
	//if the pulse count is greater than what is stored, update it and set the accumilitave time
	if (newPulses > this->pulses) {
		//need to figure out how long its been since pulses were last set
		//this->pulseTime = millis of last bus update
		//this->pulseMillis  = duration of pulses
		//duration of pulses is current millis - lasst bus update
		if (this->pulseTime == 0) { //unlikely 0 millis will have ever elapsed... so lets use that as an initial value
			this->pulseTime = millis();
		}
		if (this->pulseTime > millis()) {//time rollover calculation, at 47ish days this becomes a concern. If my jeep ever runs that long straight i deserve the resulting error
			this->pulseTime = millis();
		}

		this->pulseMillis = millis() - this->pulseTime; //millis since time was last updates
		this->pulses = newPulses;
		this->needsDistanceSet = true;
		float millisInHour = 3600000.0;
		float hours = this->pulseMillis / millisInHour;
		float pulsesInMile = 8400.0;
		float miles = newPulses / pulsesInMile;
		float mph = miles / hours;
		float kphInMph = 1.60934;
		float kph = mph*kphInMph;
		if (DEBUG) {
			Serial.print(F("Setting speed: "));
			Serial.print(String(this->pulseMillis));
			Serial.print(F(" Hours: "));
			Serial.print(String(hours, 20));
			Serial.print(F(" Miles: "));
			Serial.println(String(miles, 20));
			Serial.print(F("Speed (MPH): "));
			Serial.print(String(mph, 20));
			Serial.print(F("Speed (KPH): "));
			Serial.println(String(kph, 20));
		}
		setKPH(kph);
	}
}

void CCD::doDistanceUpdates(void){
	pulses = this->pulses;
	if (DEBUG) {
		Serial.print("Updating distance by ");
		Serial.println(String(pulses));
	}
	 while (pulses > 0xff) {
		 if (!this->busTransmit(DIST_INCR_ID, 2, 0x00, 0xff)) { //only subtract on sucess. False = sucess
			 pulses = pulses - 0xff;
		 }
	 }
	 while (pulses>0){
		 if (!this->busTransmit(DIST_INCR_ID, 2, 0x00, pulses)) { //only subtract on sucess. False = sucess
			 pulses = pulses - pulses;
		 }
	 }
	
	this->needsDistanceSet = false; //the distance has been set, kill it
	if(!this->needsDistanceSet){
		this->pulses = 0;
		this->pulseTime = millis();
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
	int newKPH = round(constrain(kph*SPEED_MULTIPLIER, 0, 255));
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
	int newVoltage = round(constrain((voltage+VOLTAGE_OFFSET) * 8, 0, 255));
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
	 //Updating MAP not yet implemented.
	this->needsUpdateRPM = this->busTransmit(RPM_MAP_ID, 2, this->rpm, 0x00);;
	didUpdates = true;

	//Speed also has to be updated within about three seconds or it falls back to zero.
	
	this->needsUpdateSpeed = this->busTransmit(SPEED_ID, 2, this->mph, this->kph);
	didUpdates = true;

	if (this->needsUpdateHealth) {
		//XJ gauge clusters continually hold the last known value.
		
		this->needsUpdateHealth = this->busTransmit(VOLTS_OILPSI_COOLTEMP_ID, 4, this->voltage, this->oilPsi, this->coolantTemperature, 0xFF);
		didUpdates = true;
	}

	if (this->needsUpdateFuelPercent) {
		this->needsUpdateFuelPercent = this->busTransmit(FUEL_ID, 1, this->fuelPercent);
		didUpdates = true;
	}

	if (this->needsUpdateLights) {
		this->doUpdateLights();
		didUpdates = true;
	}

	doDistanceUpdates();

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
	this->busTransmit(SKIM_LIGHT_ID, 1, this->boolToLight(this->skimLightOn));
	this->busTransmit(CG_LIGHT_ID, 2, this->boolToLight(this->checkGaugesLightOn), 0x00);
	this->busTransmit(CE_LIGHT_ID, 2, this->boolToLight(this->checkEngineLightOn), 0x00);
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
 * @return	bool false on sucess, true on fail (failure means the update needs to be sent again)
 */
bool CCD::busTransmitValidate(int id, int numBytes, ...) {
	va_list bytes;
	va_start(bytes, numBytes);
	delay(50);
	int checksum = id;
	while (digitalReadFast(this->idlePin) != 1) {
		delayMicroseconds(1);
	}
	Serial.println(F("Message begin"));
	
	this->ccdBus.flush();
	this->ccdBus.clear();
	this->ccdBus.write(id);
	this->ccdBus.flush();

	int timeout = 0;
	while (ccdBus.peek() == -1) {
		delayMicroseconds(1);
		timeout++;
		if (timeout >= 1035) {
			Serial.println(F("packet header timeout"));
			return true;
		}
	}

	if (ccdBus.peek() != id) {
		Serial.print(F("BAD header "));
		Serial.print(String(id));
		Serial.print(F(" transmitted, recieved "));
		Serial.print(String(ccdBus.read()));
		return true; //Failure, start over
	}
	for (int i = 0; i < numBytes; ++i) {
		int toSend = va_arg(bytes, int);
		this->ccdBus.clear();
		this->ccdBus.write(toSend);
		this->ccdBus.flush();
		int timeout = 0;
		while (ccdBus.peek() == -1) {
			delayMicroseconds(1);
			timeout++;
			if (timeout >= 1035) {
				Serial.println(F("packet data timeout"));
				return true;
			}
		}

		if (ccdBus.peek() != toSend) {
			Serial.print(F("BAD packet value "));
			Serial.print(String(toSend));
			Serial.print(F(" transmitted, recieved "));
			Serial.print(String(ccdBus.read()));
			return true; //Failure, start over
		}

	}
	va_end(bytes);
	checksum = checksum & 0xFF;
	this->ccdBus.clear();
	this->ccdBus.write(checksum);
	this->ccdBus.flush();
	timeout = 0;
	while (ccdBus.peek() == -1) {
		delayMicroseconds(1);
		timeout++;
		if (timeout >= 1035) {
			Serial.println(F("packet data timeout"));
			return true;
		}
	}

	if (ccdBus.peek() != checksum) {
		Serial.print(F("BAD CRC value "));
		Serial.print(String(checksum));
		Serial.print(F(" transmitted, recieved "));
		Serial.print(String(ccdBus.read()));
		return true; //Failure, start over
	}
	Serial.println("Good message");
	return false;
}

/*
* Transmit to the serial port.
*
* @access	private
* @return	void
*/
bool CCD::busTransmit(int id, int numBytes, ...) {
	int bytesSent = 0;
	va_list bytes;
	va_start(bytes, numBytes);
	delay(50);
	while (digitalReadFast(this->idlePin) != 1) {
		if (DEBUG) {
			Serial.println("idle");
		}
		delay(50);
	}
	if (DEBUG) {
		Serial.println(F("Message begin"));
	}
	this->ccdBus.clear();
	int checksum = id;
	this->ccdBus.write(id);
	bytesSent++;
	for (int i = 0; i < numBytes; ++i) {
		int toSend = va_arg(bytes, int);
		this->ccdBus.write(toSend);
		bytesSent++;
		checksum += toSend;
	}
	va_end(bytes);
	checksum = checksum & 0xFF;
	this->ccdBus.write(checksum);
	bytesSent++;
	this->ccdBus.flush();
	int packetCount = 0;
	int timeout = 0;

	while (packetCount < bytesSent) {
		if (this->ccdBus.available() > 0) {
			//DEBUGGING
			//Serial.print(String(this->ccdBus.read()));
			//Serial.print(F(" "));
			packetCount++;
		}
		else {
			timeout++;
			if (timeout > 1040) {
				break;
			}
			delayMicroseconds(1);
		}
	}
	if (packetCount == bytesSent) {
		if (DEBUG) {
		Serial.print(String(packetCount));
		Serial.println(" bytes sent, packet GOOD");
		}
		return false;
	}
	else {
		if (DEBUG) {
			Serial.print(String(packetCount));
			Serial.print(" bytes sent, expected ");
			Serial.print(String(bytesSent));
			Serial.println(" packet BAD");
		}
		return true;
	}
	while (digitalRead(this->idlePin) != 0) {
		if (DEBUG) {
			Serial.println("not idle");
		}
		delay(50);
	}
	
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