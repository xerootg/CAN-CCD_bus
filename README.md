# CCD-CAN_bus
CAN bus to CCD bus protocol translator for Atmel Cortex ARM on layer 1 and 2 of communication in the network.  This project is in the prototype stages.  The final goal is to be able to place a device between the CAN bus on a modern Engine Control Unit(ECU) and the CCD bus of the old modules.  The old modules include gauge clusters, body control modules, transmission controllers, and air bag modules in Chrysler designed vehicles.  Once everything is fully reverse engineered on both ends this means that modern engine swaps or ECU swaps can be done in these older vehicles without losing any of the existing CCD bus based technology.

![](docs/images/gaugewiggle.gif)

##Required Hardware
###AVR Board
Any AVR with a dedicated hardware serial pins and a CAN pins are supported.  At the moment this is limited to the [Teensy 3.1 and 3.2 boards](https://www.pjrc.com/store/teensy32.html).

###CAN Transceiver
Both the CAN bus and CCD bus will utilize a standard CAN transceiver.  Using 5 volt transceivers is recommend for best compatibility, but 3.3 volt transceivers do work.  If communicating to more than one CCD based module then a 5 volt is highly recommended as CCD is less fault tolerant to voltage differences than CAN.  A [Waveshare SN65HVD230 CAN Transceiver Board](https://www.amazon.com/gp/product/B00KM6XMXO/) is being used for reverse engineering and testing currently.

* 3.3 volt option
 * TI SN65HVD230 series.
* 5 volt options
 * NXP TJA1050T/VM,118
 * Microchip MCP2551

##FAQ
###What vehicles does this device cover?
Early 1980s to early 2000s Chrysler, Dodge, Jeep, and Plymouth models that utilized CCD bus communications.

###Does the device transmit on the CAN bus?
At the moment there are not plans to translate messages on the CCD bus over to the CAN bus.  The current focus is to provide information to devices that mainly recieve only such as gauge clusters.