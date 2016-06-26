#define XJ_PLATFORM true

#include <FlexCAN.h>
#include "CCDCodes.h"
#include "CCD.h"

const boolean debug = true;

const int led = LED_BUILTIN;
const int ccdBaud = 7812; //Due to how AVRs work this ends up being the needed 7812.5 assuming the MHz has no remainder when divided by 1.
//FlexCAN canBus;
CCD ccdBus;

void setup() {
	pinMode(led, OUTPUT);
	digitalWrite(led, HIGH);

	if (debug) {
		delay(1000);
		Serial.println(F("CCD-CAN Bus"));
	}

	ccdBus.init(Serial1);

	/*int speedIndex = canBus.connect();
	if (debug) {
		Serial.print("Speed Index -> ");
		Serial.println(speedIndex);
	}
	if (speedIndex != 255) {
		digitalWrite(led, HIGH);
	} else {
		digitalWrite(led, LOW);
	}*/

	ccdBus.setVoltage(14);
	ccdBus.setOilPSI(35);
	ccdBus.setCoolantTemperature(210);
	ccdBus.doUpdates();
}

String dataIn;
char CR = 10;
void loop() {
	delay(50);
	ccdBus.setRPM(800 + random(0, 50));
	ccdBus.doUpdates();

	/*if (Serial.available() > 0) {
		dataIn = Serial.readStringUntil(CR);
		speed = speedToCcdByte(dataIn.toFloat());
	}
	delay(50);
	ccdBusTx(SPEED_ID, 2, speed, speed);*/
	/*if (rx == true) {
		if (1 == can.available()) {
			can.read(rxmsg);
			Serial.print("PID: ");
			Serial.print(rxmsg.id, HEX);
			Serial.print(" Data:");
			for (int x = 0; x < rxmsg.len; x ++) {
				Serial.print(" ");
				if (rxmsg.req == 0) {
					if (rxmsg.buf[x] < 10) {
						Serial.print("0");
					}
					Serial.print(rxmsg.buf[x], HEX);
				} else {
					Serial.print("00");
				}
			}
			Serial.print(" Request: ");
			Serial.print(rxmsg.req);
			Serial.print(" Timestamp: ");
			Serial.println(rxmsg.timestamp);
		}
	}*/
}