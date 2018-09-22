/*
 * A simple example to interface with rdm6300 rfid reader.
 *
 * Connect the rdm6300 to VCC=5V, GND=GND, TX=any GPIO (this case GPIO_2)
 * Note: for ESP* or other 3.3V based microcontroller,
 *       you must divide the TX out of the rdm6300 to the 3.3V levels,
 *       I used the following resistor divider:
 *       GND--<2K resistor>--GPIO_2--<1K resistor>--TX(rdm6300).
 *
 * Arad Eizen (https://github.com/arduino12) 23/09/18.
 */
#include <rdm6300.h>

#define RDM6300_RX_PIN 2
#define READ_LED_PIN 13

Rdm6300 rdm6300;

void setup()
{
	Serial.begin(115200);

	pinMode(READ_LED_PIN, OUTPUT);
	digitalWrite(READ_LED_PIN, LOW);

	rdm6300.begin(RDM6300_RX_PIN);

	Serial.println("Place RFID tag near the rdm6300...");
}

void loop()
{
	/* if non-zero tag_id, update() returns true- a new tag is near! */
	if (rdm6300.update())
		Serial.println(rdm6300.get_tag_id(), HEX);

	digitalWrite(READ_LED_PIN, rdm6300.is_tag_near());

	delay(10);
}
