#include <FlexCAN.h>
#include "CCDCodes.h"

const boolean debug = true;

const int led = LED_BUILTIN;
const int ccdBaud = 7812; //Due to how AVRs work this ends up being the needed 7812.5 assuming the MHz has no remainder when divided by 1.
//FlexCAN canBus;
HardwareSerial& ccdBus = Serial1;

void setup() {
	pinMode(led, OUTPUT);
	digitalWrite(led, HIGH);

	if (debug) {
		delay(1000);
		Serial.println(F("CCD-CAN Bus"));
	}

	ccdBus.begin(ccdBaud);

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

	ccdBusTx(VOLTS_OILPSI_COOLTEMP_ID, 4, 0x70, 0x30, 0xA0, 0x00);

	delay(50);

	ccdBusTx(AIRBAG_GOOD_ID, 1, 0xFF);
}

String dataIn;
char CR = 10;
byte volts = 0x70;
byte psi = 0x30;
byte tempF = 0xA0;
byte rpm = 0x25;
void loop() {
	delay(50);
	ccdBusTx(RPM_MAP_ID, 2, rpm, 0x00);

	if (Serial.available() > 0) {
		dataIn = Serial.readStringUntil(CR);
		tempF = coolTempToCcdByte(dataIn.toFloat());
	}
	delay(50);
	ccdBusTx(VOLTS_OILPSI_COOLTEMP_ID, 4, volts, psi, tempF, 0x00);
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

void ccdBusTx(byte id, int numBytes, ...) {
	va_list bytes;
	va_start(bytes, numBytes);

	byte checksum = id;
	ccdBus.write(id);
	for (int i = 0; i < numBytes; ++i) {
		byte toSend = va_arg(bytes, int);
		ccdBus.write(toSend);
		checksum += toSend;
	}

	va_end(bytes);

	checksum = checksum & 0xFF;
	ccdBus.write(checksum);
}

byte rpmToCcdByte(int rpm) {
	return constrain(rpm / 32, 0, 255);
}

byte voltsToCcdByte(float volts) {
	return constrain(volts * 8, 0, 255);
}

byte oilPsiToCcdByte(float psi) {
	return constrain(psi * 2, 0, 255);
}

byte coolTempToCcdByte(float tempF) {
	byte hex = 0xC1; //193 - Also, fail safe to alerting in the red for temperature.
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
	//float tempC = (tempF - 32) * 5 / 9;
	return constrain(hex, 0, 255);
}