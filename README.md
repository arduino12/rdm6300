# rdm6300
A simple library to interface with RDM6300 RFID reader.

## Features
* Fast and single tag reading, even if it held near the antenna for a while.
* Using a single configurable GPIO pin.
* Can tell if the tag is still near the antenna.

## Getting Started

### Hardware
* RDM6300 module with matching antenna.
* 125KHz RFID tags.
* Microcontroller (arduino, esp8266, esp32...).
* Jump-Wires (Resistors of 1K and 2K if you are using 3.3V based microcontroller).

![alt text](https://lh3.googleusercontent.com/xaZxf4aRQCM8teJDho_jtRJmqZsE5Da2rX5pBThctffH6az2lyWtEnTPPcqcCPayg55p8bbobJLVRCW-M5e5ywGp2nlcUq1dZAzxXvzCjm2zXq0e8K1EeNI6WG2G_gApMI90Lkt_rYKPiamGgXwFvjGCE-rI2NmOciinfDIbdIpCHMlPf-0WRTxo7s7hSY86b0BDnNRpCKgZBeJizIM3karj6Y5EwH3xP5heDDr2sunw4YWF5EQCmVrU-ipS8Umzp5EnyQELH0nW9JfAXFHkN4aCVoO2QW2oOMRajGpEW8IGxAQUEmLHmYQEsqV1dUd4aVvkTFnN-H2JoOssspcIpXoHExpLLxOwlk2QjT6KVw7hQaeENwv-my6gEbTOzfg9DoC__j6WMkjewsaph408ALDsqxjm5lOoZDl9ag8nwqXi0SmmCphq7FiwG9pbyrTCbOqh4V8Dep4vXg3lkchZeedWLSU51xIJjN3K2RYG9tSiemIO4TjjNB_fEZiq5dbakNeSjKdyjjCUoKsWTOsrDlkBSpvC7U_2VcrU_4eExXkf8qyY_HPYD-X6QRodMqBzvQJUOlC_UIihDbZd8MUW0WMsGBCKAzVScL4msemZphtI4vhWAwNj4l1ss5P9hCkAdhpWPJkzz8hIYvBt3HLfxPv9NL0S12C1TKVNEGmu_PZkPvrk4j1I0hXwVA=w688-h578-no "rdm6300_connections")

### Software
A simple RFID to serial reciver example can be found at:
[```examples/read_to_serial/read_to_serial.ino```](examples/read_to_serial/read_to_serial.ino)

#### API
* ```void begin(int rxPin)``` - Initialize the object to use the given GPIO pin as RX from the RDM6300.
* ```bool update()``` - Updates the internal values by reading from the RDM6300, returns true on tag detection, must be called repeatedly!
* ```uint32_t get_tag_id()``` - Returns the last tag id read by ```update```- can be called only once after ```update```.
* ```bool is_tag_near()``` -  Returns whether a tag is currently held near the antenna- can be called many times after ```update```.
## enjoy!
A.E.TECH
