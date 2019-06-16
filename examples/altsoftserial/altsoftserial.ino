/*
 * A simple example to interface with rdm6300 rfid reader,
 * using library AltSoftSerial instead of SoftwareSerial
 * (used by default). Since SoftwareSerial is interrupt-based,
 * it can disrupt other libraries that are also interrupt-based,
 * such as Servo and Wire. The downside of using AltSoftSerial
 * is that only pin 8 can be used to connect the rdm6300 reader.
 * If this is not a problem to you, use this sketch as a base
 * instead of read_to_serial.
 * 
 * AltSoftSerial works only with AVR microcontrollers
 * (Arduino Uno, Nano, Pro Mini, etc) and is available at
 * https://github.com/PaulStoffregen/AltSoftSerial
 *
 * Connect the rdm6300 to VCC=5V, GND=GND, TX=pin 8
 * Note that the rdm6300's TX line is 3.3V level,
 * so it's safe to use with both AVR* and ESP* microcontrollers.
 *
 * Arad Eizen (https://github.com/arduino12) 23/09/18, 08/05/19, 09/06/19.
 * Gutierrez PS (https://github.com/gutierrezps) 16/06/2019.
 */

#include <AltSoftSerial.h>
#include <rdm6300.h>

#define READ_LED_PIN 13

AltSoftSerial rfidSerial;
Rdm6300 rdm6300;

void setup()
{
	Serial.begin(115200);

	pinMode(READ_LED_PIN, OUTPUT);
	digitalWrite(READ_LED_PIN, LOW);

	rfidSerial.begin(RDM6300_BAUDRATE);
	rdm6300.begin(&rfidSerial);

	Serial.println("\nPlace RFID tag near the rdm6300...");
}

void loop()
{
	/* if non-zero tag_id, update() returns true- a new tag is near! */
	if (rdm6300.update())
		Serial.println(rdm6300.get_tag_id(), HEX);

	digitalWrite(READ_LED_PIN, rdm6300.is_tag_near());

	delay(10);
}
