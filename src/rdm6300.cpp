/*
 * A simple library to interface with rdm6300 rfid reader.
 * Arad Eizen (https://github.com/arduino12).
 */

#include "rdm6300.h"

void Rdm6300::begin(Stream *stream)
{
	_stream = stream;
	if (!_stream)
		return;
	_stream->setTimeout(RDM6300_READ_TIMEOUT);
}

void Rdm6300::begin(int rx_pin, uint8_t uart_nr)
{
	/* init serial port to rdm6300 baud, without TX, and 20ms read timeout */
	end();
#if defined(ARDUINO_ARCH_ESP32)
	_stream = _hardware_serial = new HardwareSerial(uart_nr);
	_hardware_serial->begin(RDM6300_BAUDRATE, SERIAL_8N1, rx_pin, -1);
#elif defined(ARDUINO_ARCH_ESP8266)
	if (rx_pin == 13) {
		_stream = _hardware_serial = &Serial;
		_hardware_serial->begin(RDM6300_BAUDRATE, SERIAL_8N1, SERIAL_RX_ONLY);
		if (uart_nr)
			_hardware_serial->swap();
	}
#elif defined(ARDUINO_ARCH_SAMD)
	_stream = _hardware_serial = (uart_nr == 2 ? &Serial2 : &Serial1);
	_hardware_serial->begin(RDM6300_BAUDRATE, SERIAL_8N1);
#endif
#ifdef RDM6300_SOFTWARE_SERIAL
	if (!_stream) {
		_stream = _software_serial = new SoftwareSerial(rx_pin, -1);
		_software_serial->begin(RDM6300_BAUDRATE);
	}
#endif
	begin(_stream);
}

void Rdm6300::end()
{
	_stream = NULL;
#ifdef RDM6300_HARDWARE_SERIAL
	if (_hardware_serial)
		_hardware_serial->end();
#endif
#ifdef RDM6300_SOFTWARE_SERIAL
	if (_software_serial)
		_software_serial->end();
#endif
}

uint32_t Rdm6300::_read_tag_id(void)
{
	char buff[RDM6300_PACKET_SIZE];
	uint8_t checksum;
	uint32_t tag_id;

	if (!_stream)
		return 0;

	if (!_stream->available())
		return 0;

	/* if a packet doesn't begin with the right byte, remove that byte */
	if (_stream->peek() != RDM6300_PACKET_BEGIN && _stream->read())
		return 0;

	/* if read a packet with the wrong size, drop it */
	if (RDM6300_PACKET_SIZE != _stream->readBytes(buff, RDM6300_PACKET_SIZE))
		return 0;

	/* if a packet doesn't end with the right byte, drop it */
	if (buff[13] != RDM6300_PACKET_END)
		return 0;

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
		return 0;

	return tag_id;
}

void Rdm6300::_update(void)
{
	uint32_t cur_ms = millis();
	uint32_t tag_id = _read_tag_id();

	/* if a new tag appears- return it */
	if (tag_id) {
		_tag_id = tag_id;
		if (_last_tag_id != tag_id) {
			_last_tag_id = tag_id;
			_new_tag_id = tag_id;
		}
		_last_tag_ms = cur_ms;
		return;
	}

	/* if the old tag id is still valid- leve it */
	if (!_tag_id || (cur_ms - _last_tag_ms < _tag_timeout_ms))
		return;

	_tag_id = _last_tag_id = _new_tag_id = 0;
}

/*
 * Sets the tag "valid" timeout,
 * RDM6300 sends packet every 65ms when tag is near-
 * so theoretically this timeout should be the same,
 * but it is better to debounce it by setting it higher,
 * like the RDM6300_DEFAULT_TAG_TIMEOUT_MS = 300.
 */
void Rdm6300::set_tag_timeout(uint32_t tag_timeout_ms)
{
	_tag_timeout_ms = tag_timeout_ms;
}

/*
 * Returns the tag_id as long as it is near (and the tag timout is valid),
 * or 0 if no tag is near.
 */
uint32_t Rdm6300::get_tag_id(void)
{
	_update();
	return _tag_id;
}

/*
 * Returns the tag_id only once per "new" near tag,
 * or 0 if no tag is near.
 * Following calls will return 0 as long as the same tag is kept near.
 */
uint32_t Rdm6300::get_new_tag_id(void)
{
	_update();
	uint32_t tag_id = _new_tag_id;
	_new_tag_id = 0;
	return tag_id;
}

#ifdef RDM6300_SOFTWARE_SERIAL
void Rdm6300::listen(void)
{
	_software_serial->listen();
}

bool Rdm6300::is_listening(void)
{
	return _software_serial->isListening();
}
#endif
