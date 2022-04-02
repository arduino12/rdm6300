/*
 * A simple example to interface with multiple software serials.
 * Use multiple software serials on some boards can only listen one UART at a time like Uno.
 * 
 * Note:
 *     You can use it for connect other software serials devices not only rdm6300.
 *     In these case, we use two rdm6300 readers for example.
 * 
 *     The LED pin of rdm6300 is set at 5V (HIGH) at initial, 
 *     and if an RFID was read, the LED pin goes to 0V (LOW) for some moments.
 *     We can use this property to switch between two devices,
 *     but it still can only read from a device at a time.
 * 
 *     And be aware about usage of listening switch between devices.
 * 
 * Connections:
 *     | Uno | rdm6300-1 | rdm6300-2 | notes                               |
 *     |-----+-----------+-----------+-------------------------------------|
 *     | D5  | TX        |           |                                     |
 *     | D6  | LED       |           |                                     |
 *     | D7  |           | TX        |                                     |
 *     | D8  |           | LED       |                                     |
 *     | GND | GND       | GND       |                                     |
 *     | VCC | VCC       | VCC       | The rdm6300 must be powered with 5V |
 * 
 * Zeng (https://github.com/asas1asas200)
 */

#include <rdm6300.h>

#define RDM6300_1_RX_PIN	(5)
#define RDM6300_1_LED_PIN	(6)
#define RDM6300_2_RX_PIN	(7)
#define RDM6300_2_LED_PIN	(8)

Rdm6300 rdm6300_1;
Rdm6300 rdm6300_2;

Rdm6300 *current_rdm6300;

void setup()
{
	Serial.begin(115200);
	pinMode(RDM6300_1_LED_PIN, INPUT);
	pinMode(RDM6300_2_LED_PIN, INPUT);

	rdm6300_1.begin(RDM6300_1_RX_PIN);
	rdm6300_2.begin(RDM6300_2_RX_PIN);

	rdm6300_1.listen();
	current_rdm6300 = &rdm6300_1;

	Serial.println("Place RFID tag near any rdm6300...");
}

void loop()
{
	/* listen to a "talking" RDM6300 if needed */
	if (!digitalRead(RDM6300_1_LED_PIN) && !rdm6300_1.is_listening()) {
		Serial.println("Switch to listening RDM6300_1");
		rdm6300_1.listen();
		current_rdm6300 = &rdm6300_1;
	}
	else if (!digitalRead(RDM6300_2_LED_PIN) && !rdm6300_2.is_listening()) {
		Serial.println("Switch to listening RDM6300_2");
		rdm6300_2.listen();
		current_rdm6300 = &rdm6300_2;
	}

	/* if you use rdm6300 with other devices,
	you had better set a TIMEOUT and run a while-loop to
	make sure the tag can be read correctely */
	if (current_rdm6300->get_new_tag_id()) 
		Serial.println(current_rdm6300->get_tag_id(), HEX);

	delay(10);
}
