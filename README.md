# rdm6300
A simple library to interface with RDM6300 RFID reader.

## Features
* Fast and single tag reading, even if it held near the antenna for a while.
* Using a single given GPIO pin or a `Stream`.
* Can tell if the tag is still near the antenna.
* Both hardware and software uart (serial) support on esp8266.
* SAMD hardware uart (serial) support.
* Both SoftwareSerial and [AltSoftSerial](https://github.com/PaulStoffregen/AltSoftSerial) support.

## Getting Started

### Hardware
* RDM6300 module with matching antenna.
* 125KHz RFID tags.
* Microcontroller (arduino, esp8266, esp32...).

![rdm6300_connections](../master/doc/rdm6300_connections.png "rdm6300_connections")

### Software
A simple RFID tag reader with textual serial output:
[```examples/read_to_serial/read_to_serial.ino```](examples/read_to_serial/read_to_serial.ino)

#### API
* ```void begin(Stream *stream)``` - Initialize instance to read from a given stream.
* ```void begin(int rx_pin, uint8_t uart_nr=1)``` - Initialize instance to read from a given pin.
* ```void set_tag_timeout(uint32_t tag_timeout_ms)``` - sets the tag "valid" timeout, (300ms default)  
RDM6300 sends packet every 65ms when tag is near- better higher values for debouncing.  
* ```uint32_t get_tag_id()``` - Returns the tag_id as long as it is near, 0 otherwise.
* ```uint32_t get_new_tag_id()``` - Returns the tag_id of a "new" near tag,  
following calls will return 0 as long as the same tag is kept near.
* ~~```bool update()``` - Updates the internal values must be called repeatedly!~~ **deprecated!**
* ~~```bool is_tag_near()``` - Returns whether a tag is held near.~~ **deprecated!** use ```get_tag_id()```.
## Enjoy!
A.E.TECH
