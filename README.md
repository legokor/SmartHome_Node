# SmartHome Node
The current version is running on esp8266 and based on [Arduino Core](https://github.com/esp8266/Arduino).

## Communication
Nodes are using UDP protocol between each other. The current message format is modified [NMEA](https://en.wikipedia.org/wiki/NMEA_0183).

### Message format
> $[command],[nodeid],[payload]*[checksum]

* command: 4 bytes 
* nodeid: 17 bytes ( node bssid in string format)
* payload: depends on command
* checksum: 2 bytes ( XOR of all bytes between **$** and **\*** represented as 2 digit hex)

### Commands

**READ**  
Read all input value ( current inputnumber is 4). The payload will be 4 bytes. The first byte contains the analog input.    

Sender:

> payload is empty

Receiver:

> payload = [pin1 state][pin2 state][pin3 state][pin4 state]
---  

**WRTE**  
Set the output pin state and mode. Currently there is 2 different mode simple GPIO or PWM.

Sender:
> payload = [pin number][pin mode][pin state]

* [pin number]: 0 to 4
* [pin mode]: 0 (GPIO), 1 (PWM)
* [pin state]: 0 or 1 (if GPIO), 0 to 255 (if PWM)

Receiver:  
No response

---
**LIST**  
It sends back the currently connected devices.

Sender: 
> payload is empty

Receiver: 
> payload = [nodeid]...

* [nodeid]: 6 bytes
