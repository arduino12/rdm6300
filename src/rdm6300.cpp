/*
 * A simple library to interface with rdm6300 rfid reader.
 * Arad Eizen (https://github.com/arduino12) 23/09/18.
 */

#include "rdm6300.h"
#include <Arduino.h>


void Rdm6300::begin(int rx_pin, uint8_t uart_nr)
{
	/* init serial port to rdm6300 baud, without TX, and 20ms read timeout */
#ifdef ARDUINO_ARCH_ESP32
	_serial = new HardwareSerial(uart_nr);
	_serial->begin(RDM6300_BAUDRATE, SERIAL_8N1, rx_pin, -1);
#else
	_serial = new SoftwareSerial(rx_pin, -1);
	_serial->begin(RDM6300_BAUDRATE);
#endif
	_serial->setTimeout(20);
}

bool Rdm6300::update(void)
{
	char buff[RDM6300_PACKET_SIZE];
	uint32_t tag_id;
	uint8_t checksum;

	if (!_serial->available())
		return false;

	/* if a packet doesn't begin with the right byte, remove that byte */
	if (_serial->peek() != RDM6300_PACKET_BEGIN && _serial->read())
		return false;

	/* if read a packet with the wrong size, drop it */
	if (RDM6300_PACKET_SIZE != _serial->readBytes(buff, RDM6300_PACKET_SIZE))
		return false;

	/* if a packet doesn't end with the right byte, drop it */
	if (buff[13] != RDM6300_PACKET_END)
		return false;

	/* add null and parse checksum */
	buff[13] = 0;
	checksum = strtol(buff + 11, NULL, 16);
	/* add null and parse tag_id */
	buff[11] = 0;
	tag_id = strtol(buff + 3, NULL, 16);
	/* add null and parse version (needs to be xored with checksum) */
	buff[3] = 0;
	checksum ^= strtol(buff + 1, NULL, 16);

	/* xore the tag_id and validate checksum */
	for (uint8_t i = 0; i < 32; i += 8)
		checksum ^= ((tag_id >> i) & 0xFF);
	if (checksum)
		return false;

	/* if a new tag appears- return it */
	if (_last_tag_id != tag_id) {
		_last_tag_id = tag_id;
		_next_read_ms = 0;
	}
	/* if the old tag is still here set tag_id to zero */
	if (_next_read_ms > millis())
		tag_id = 0;
	_next_read_ms = millis() + RDM6300_NEXT_READ_MS;

	_tag_id = tag_id;
	return tag_id;
}

bool Rdm6300::is_tag_near(void)
{
	return _next_read_ms > millis();
}

uint32_t Rdm6300::get_tag_id(void)
{
	uint32_t tag_id = _tag_id;
	_tag_id = 0;
	return tag_id;
}
