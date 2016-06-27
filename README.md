# CCD-CAN_bus
CAN bus to CCD bus protocol translator for AVR on layer 1 and 2 of communication in the network.  This project is in the prototype stages.  The final goal is to be able to place a device between the CAN bus on a modern Engine Control Unit(ECU) and the CCD bus of the old modules.  The old modules include gauge clusters, body control modules, transmission controllers, and air bag modules in Chrysler designed vehicles.  Once everything is fully reverse engineered on both ends this means that modern engine swaps or ECU swaps can be done in these older vehicles without losing any of the existing CCD bus based technology.

##FAQ
###What vehicles does this device cover?
Early 1980s to early 2000s Chrysler, Dodge, Jeep, and Plymouth models that utilized CCD bus communications.

###Does the device transmit on the CAN bus?
At the moment there are not plans to translate messages on the CCD bus over to the CAN bus.  The current focus is to provide information to devices that mainly recieve only such as gauge clusters.