/*
 * A simple example to interface with rdm6300 rfid reader using esp8266.
 * We use hardware uart "Serial" instead of the default software uart driver.
 *
 * Note:
 *     The esp8266 let us use 1.5 uarts:
 *     Serial=uart0=rx+tx, Serial1=uart1=tx only (rx pin is used as flash io).
 *     Here we sawp the uart pins so uart0_rx goes to the rdm6300_tx,
 *     and uart1_tx goes to what used to be uart0_tx-
 *     so debug message goes from Serial1.print(...) to the pc debug terminal.
 *     https://github.com/esp8266/Arduino/blob/master/doc/reference.rst#serial
 *
 * Connections:
 *     | esp8266 | nodemcu | rdm6300 | notes                               |
 *     |---------+---------+---------|-------------------------------------|
 *     | GPIO-01 | TX      |         | This is TXD0, connect it to GPIO-02 |
 *     | GPIO-02 | D4      |         | This is TXD1, connect it to GPIO-01 |
 *     | GPIO-03 | RX      |         | Leave it unconnected for flashing   |
 *     | GPIO-13 | D7      | TX      | TX level is same 3.3V as esp8266    |
 *     |         | VU (5V) | VCC     | The rdm6300 must be powered with 5V |
 *     | GND     | GND     | GND     |                                     |
 *
 *     * GPIO-01 to GPIO-02 is for debug terminal output.
 *
 * Arad Eizen (https://github.com/arduino12).
 */

#include <rdm6300.h>

#define RDM6300_RX_PIN 13 // can be only 13 - on esp8266 force hardware uart!
#define READ_LED_PIN 16

Rdm6300 rdm6300;

void setup()
{
	/* Serial1 is the debug! remember to bridge GPIO-01 to GPIO-02 */
	Serial1.begin(115200);

	pinMode(READ_LED_PIN, OUTPUT);
	digitalWrite(READ_LED_PIN, LOW);

	rdm6300.begin(RDM6300_RX_PIN);

	Serial1.println("\nPlace RFID tag near the rdm6300...");
}

void loop()
{
	/* get_new_tag_id returns the tag_id of a "new" near tag,
	following calls will return 0 as long as the same tag is kept near. */
	if (rdm6300.get_new_tag_id())
		Serial1.println(rdm6300.get_tag_id(), HEX);

	/* get_tag_id returns the tag_id as long as it is near, 0 otherwise. */
	digitalWrite(READ_LED_PIN, rdm6300.get_tag_id());

	delay(10);
}
