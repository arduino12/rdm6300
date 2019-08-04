# rdm6300
A simple library to interface with RDM6300 RFID reader.

## Features
* Fast and single tag reading, even if it held near the antenna for a while.
* Using a single configurable GPIO pin.
* Can tell if the tag is still near the antenna.
* Both hardware and software uart (serial) support on esp8266.
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
* ```void begin(Stream *stream)``` - Initialize the object to use the given stream to read from the RDM6300.
* ```void begin(int rxPin, uint8_t uart_nr=1)``` - Initialize the object to use the given GPIO pin as RX from the RDM6300.
* ```bool update()``` - Updates the internal values by reading from the RDM6300, returns true on tag detection, must be called repeatedly!
* ```uint32_t get_tag_id()``` - Returns the last tag id read by ```update```- can be called only once after ```update```.
* ```bool is_tag_near()``` -  Returns whether a tag is currently held near the antenna- can be called many times after ```update```.
## Enjoy!
A.E.TECH
