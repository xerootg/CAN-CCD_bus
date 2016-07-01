#include "CAN.h"
#include <Arduino.h>
#include <Math.h>

/*
 * Initialize CAN bus and other necessary information.
 *
 * @access	public
 * @return	void
 */
float CAN::init() {
	int speedIndex = canBus.connect();

	if (speedIndex != 255) {
		return true;
	}
	return false;
}

float CAN::getRPM() {
	return rpm;
}

float CAN::getMPH() {
	return mph;
}

float CAN::getKPH() {
	return kph;
}

float CAN::getOilPSI() {
	return oilPsi;
}

float CAN::getVoltage() {
	return voltage;
}

float CAN::getCoolantTemperature() {
	return coolantTemperature;
}

bool CAN::getCheckEngineLight() {
	return checkEngineLight;
}

bool CAN::getCheckGaugesLight() {
	return checkGaugesLight;
}