/*
 * A simple library to interface with rdm6300 rfid reader.
 * Arad Eizen (https://github.com/arduino12) 23/09/18.
 */

#ifndef _RDM6300_h_
#define _RDM6300_h_

#ifdef ARDUINO_ARCH_ESP32
	#include <HardwareSerial.h>
#else
	#include <SoftwareSerial.h>
#endif

#define RDM6300_BAUDRATE		9600
#define RDM6300_PACKET_SIZE		14
#define RDM6300_PACKET_BEGIN	0x02
#define RDM6300_PACKET_END		0x03
#define RDM6300_NEXT_READ_MS	220

class Rdm6300
{
	public:
		void begin(int rxPin, uint8_t uart_nr=1);
		bool update(void);
		uint32_t get_tag_id(void);
		bool is_tag_near(void);
	private:
#ifdef ARDUINO_ARCH_ESP32
		HardwareSerial * _serial;
#else
		SoftwareSerial * _serial;
#endif
		uint32_t _tag_id = 0;
		uint32_t _last_tag_id = 0;
		uint32_t _next_read_ms = 0;
};

#endif
