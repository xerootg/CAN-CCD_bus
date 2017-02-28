#define SPEED_MULTIPLIER 1.3
#include "CCD.h"
const int idlePin = 2;

const boolean debug = true;

const int led = LED_BUILTIN;
CCD ccdBus;

float lastMPH = 0;
unsigned long lastMillis = 0;

void setup() {
	pinMode(led, OUTPUT);
	pinMode(idlePin, INPUT);

	if (debug) {
		delay(1000);
		Serial.println(F("CCD-CAN Bus"));
	}

	ccdBus.init(Serial1, idlePin);


	ccdBus.setVoltage(14);
	ccdBus.setOilPSI(35);
	ccdBus.setFuelPercent(50);
	ccdBus.setCoolantTemperature(210);
	ccdBus.setAirBagLight(false);
	ccdBus.setCheckGaugesLight(false);
	ccdBus.setCheckEngineLight(false);
	ccdBus.setSKIMLight(false);
	ccdBus.setShiftLight(false);
	ccdBus.setCruiseLight(false);
	ccdBus.doUpdates();
	lastMillis = millis();
}

String dataIn;
char CR = 10;
float rpm = 2500;
byte bitfield = 0x00;
void loop() {
	delay(50);
	
	ccdBus.setRPM(rpm + random(0, 1000));
	ccdBus.setMPH(25);
	ccdBus.setKPH(random(0,150));
	ccdBus.setVoltage(random(10, 15));
	ccdBus.setOilPSI(random(0,80));
	ccdBus.setCoolantTemperature(random(100,240));
	ccdBus.setFuelPercent(random(50,75));

	ccdBus.doUpdates();
}
