#include "CCD.h"
const int idlePin = 2;

const boolean debug = false;

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
	ccdBus.setOilPSI(40);
	ccdBus.setFuelPercent(50);
	ccdBus.setCoolantTemperature(200);
	ccdBus.setAirBagLight(false);
	ccdBus.setCheckGaugesLight(false);
	ccdBus.setCheckEngineLight(false);
	ccdBus.setSKIMLight(false);
	ccdBus.setShiftLight(false);
	ccdBus.setCruiseLight(false);
	ccdBus.setDistancePulses(0);
	ccdBus.doUpdates();
	lastMillis = millis();
}

String dataIn;
char CR = 10;
float rpm = 2500;
byte bitfield = 0x00;
int count = 0;
void loop() {
	delay(50);
	
	//ccdBus.setRPM(4000);
	//ccdBus.setKPH(100); //jeep diesel cluster uses KPH not MPH. MPH is ignored.
	//ccdBus.setVoltage(random(10, 18)); //.25 is added to this value
	//ccdBus.setOilPSI(random(0,80));
	//ccdBus.setCoolantTemperature(random(100,240));
	//ccdBus.setFuelPercent(50);
	if (count < 600) {
		ccdBus.setDistancePulses(14);
		count++;
		Serial.print("update");
		Serial.println(String(count));
	}
	else {
		ccdBus.setKPH(180);
	}
	ccdBus.doUpdates();
}
