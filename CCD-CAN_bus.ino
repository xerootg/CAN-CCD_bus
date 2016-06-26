#include <FlexCAN.h>
#include "CCDCodes.h"

const boolean debug = true;

const int led = LED_BUILTIN;
const int ccdBaud = 7812; //Due to how AVRs work this ends up being the needed 7812.5 assuming the MHz has no remainder when divided by 1.
//FlexCAN canBus;
HardwareSerial& ccdBus = Serial1;

byte buffer;

void setup() {
	pinMode(led, OUTPUT);
	digitalWrite(led, HIGH);

	if (debug) {
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

	ccdBusTx(VOLTS_OILPSI_COOLTEMP_ID, 0x00, 0x00, 0x00, 0x00);

	/*delay(100);

	Serial1.write(AIRBAG_GOOD_ID);
	Serial1.write(0xFF);
	Serial1.write(checksum(AIRBAG_GOOD_ID, byte[1]{0xFF}));
	Serial1.flush();

	delay(100);

	Serial1.write(CG_LIGHT_ID);
	Serial1.write(0x00);
	Serial1.write(checksum(CG_LIGHT_ID, byte[1]{0x00}));
	Serial1.flush();

	delay(100);

	Serial1.write(CE_LIGHT_ID);
	Serial1.write(0x00);
	Serial1.write(checksum(CE_LIGHT_ID, byte[1]{0x00}));
	Serial1.flush();*/
}

void loop() {
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

void ccdBusTx(byte id, ...) {
	va_list bytes;
	va_start(bytes, id);

	byte checksum = id;
	ccdBus.write(id);
	for (size_t i = 0; i < sizeof(bytes); ++i) {
		byte toSend = va_arg(bytes, int);
		ccdBus.write(toSend);
		checksum += toSend;
	}

	checksum = checksum & 0xFF;
	ccdBus.write(checksum);
}