#include "HaltechCAN.h"
#include <Arduino.h>
#include <Math.h>

/*
 * Initialize CAN bus and other necessary information.
 *
 * @access	public
 * @return	void
 */
void HaltechCAN::init() {
	
}

float getRPM() {
	return 0.0;
}

float getMPH() {
	return 0.0;
}

float getKPH() {
	return 0.0;
}

float getOilPSI() {
	return 0.0;
}

float getVoltage() {
	return 0.0;
}

float getCoolantTemperature() {
	return 0.0;
}

bool getCheckEngineLight() {
	return true;
}

bool getCheckGaugesLight() {
	return true;
}

bool HaltechCAN::getUpdates() {
	return true;
}

void HaltechCAN::busRecieve() {
	
}