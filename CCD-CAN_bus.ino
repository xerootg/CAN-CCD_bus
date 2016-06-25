#include <FlexCAN.h>
#include <kinetis_flexcan.h>
#include <Metro.h>
#include "CCDCodes.h"

//Metro sysTimer = Metro(1);// milliseconds

const int led = LED_BUILTIN;
const float baud = 7812;
//FlexCAN CANbus(7812.5);

byte sentbyte;
unsigned long prevmillis;
byte testbyte=0x00;

byte question = ECHO_ID;
byte checksum = ECHO_ID;

void setup() {
	//CANbus.begin();
	pinMode(led, OUTPUT);
	digitalWrite(led, HIGH);

	delay(1000);
	Serial.println(F("CCD-CAN Bus Test"));

	Serial1.begin(baud);

	delay(500);

	question = VOLTS_OILPSI_COOLTEMP_ID;
	checksum = (question + 0x70 + 0x5F + 0x64 + 0x45) & 0xFF;
	Serial1.write(question);
	Serial1.write(0x70);
	Serial1.write(0x5F);
	Serial1.write(0x64);
	Serial1.write(0x45);
	Serial1.write(checksum);
	Serial1.flush();

	delay(100);

	question = AIRBAG_GOOD_ID;
	checksum = (question + 0xFF) & 0xFF;
	Serial1.write(question);
	Serial1.write(0xFF);
	Serial1.write(checksum);
	Serial1.flush();

	delay(100);

	question = CG_LIGHT_ID;
	checksum = (question + 0x00) & 0xFF;
	Serial1.write(question);
	Serial1.write(0x00);
	Serial1.write(checksum);
	Serial1.flush();

	delay(100);

	question = CE_LIGHT_ID;
	checksum = (question + 0x00) & 0xFF;
	Serial1.write(question);
	Serial1.write(0x00);
	Serial1.write(checksum);
	Serial1.flush();
}

void loop() {
	for (int i = 0; i <= 255; ++i) {
		Serial.println(testbyte, HEX);
		Serial.println(i, HEX);
		question = RPM_MAP_ID;
		checksum = (question + i + i) & 0xFF;
		Serial1.write(question);
		Serial1.write(i);
		Serial1.write(i);
		Serial1.write(checksum);
		Serial1.flush();
		delay(50);
	}

	digitalWrite(led, LOW);
	delay(50);
	digitalWrite(led, HIGH);

	if (Serial1.availableForWrite()) {
		//RPM/MAP Swinger
		/*byte checksum = (RPM_MAP_ID + testbyte + testbyte) & 0xFF;
		Serial1.write(RPM_MAP_ID);
		Serial1.write(testbyte);
		Serial1.write(testbyte);
		Serial1.write(checksum);
		testbyte = testbyte + 5;
		//Serial.println(testbyte);*/

		//Brute Force IDs
		/*byte checksum = (testbyte + 0xD7 + 0x8D) & 0xFF;
		Serial1.write(testbyte);
		Serial1.write(0xD7);
		Serial1.write(0x8D);
		Serial1.write(checksum);
		Serial1.flush();
		Serial.println("--");
		Serial.println(testbyte, HEX);*/
	}
	//Serial.println("0xFF");
	/*checksum = (question + 0xFF + 0xFF + 0xFF + 0xFF) & 0xFF;
	Serial1.write(question);
	Serial1.write(0xFF);
	Serial1.write(0xFF);
	Serial1.write(0xFF);
	Serial1.write(0xFF);
	Serial1.write(checksum);*/
	testbyte = testbyte + 1;
}

byte buffer;
void serialEvent1() {
	buffer = Serial1.read();
	//Serial.println(buffer, HEX);
}

/*void checksum(byte id, ...) {
    va_list vl;
    va_start(vl, id);
    int max = va_arg(vl, int);
    for(int i = 2; i <= n_args; i++) {
        int a = va_arg(ap, int);
        if(a > max) max = a;
    }
    va_end(ap);
    return max;
}*/