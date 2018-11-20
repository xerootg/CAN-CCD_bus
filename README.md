CAN->CCD Bus Translator V2
===========================

I am designing an interface for my current engine swap project in my Jeep. I am starting with a 1998 Jeep Cherokee, 4.0L I6, AX15 manual transmission, and a NP242 transfer case, and I am replacing the engine with a VW TDI from a 2005 VW Passat (2.0L 8v, code:                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            BHW) and a 6 speed manual from a late model TJ Rubicon (NSG370).


This project is made up of two parts:
* a board designed to interface with the Jeep CCD protocol and VW CAN v2.0
* a event driven application to handle the needed events to translate between buses

Things needed to get started:
----------------------------
* KiCAD
* PlatformIO
* MinGW or GCC installed and on your $PATH (for tests)

Hardware Concept
----------------
* Teensy 3.2 at the core - extra I/O available for further expansion
* dedicated CCD interface IC - CDP68HC68S1
* CAN handled by teensy/MPC2551
* I/O for all circuits missing from engine/Jeep
* [BOM at Mouser](https://www.mouser.com/ProjectManager/ProjectDetail.aspx?AccessID=2fd919600a)

The VW ECU expects a handful of things the Jeep does not handled
* fuel level (leters)
* speed at 8200 pulses/mile (need to confirm voltage EDC expects)
* cruise control is array of switches
* alternator is monitored by gauge cluster

Likewise, the Jeep expects a couple things the VW ECU does not provide
* speed at 8000 pulses/mile from VSS, CCD expects 8388 bits/mile for odometer
* oil pressure (PSI)
* fuel level (%)
* cruise control is resistor bus
* glow plug lamp needs CAN->physical output translation

Software Concept
----------------
* Unit tests are written in unity syntax
* Events are handled by Eventually
* Arduino API does hardware interface, with TeensyDuino additions
* Each interface gets its own class: CCD, CAN, distance/speed, fuel level... etc.

To run tests, run `pio test -e native`
To compile ELF for teensy, run `pio run`

I'm a web developer so this is gonna be MVCish. 
- Model: Holds some state
- View: Puts state, I.E. does the actual interface with HW
- Controller: translates between model and view, and viceversa

The counter class implements logic to read the hardware model, so by this definition it is a Controller.
The VehicleState class is just a holder of state, so it is squarely a Model.
The CCD hardware interface gets/sets values on the HardwareSerial interface, so it is a view.

This will likely morph.

TODO
----

* implement CAN class
* implement fuel level
* translate cruise buttons
* rewrite CCD class to respect bus status (currently it blindly transmits)
* write tests so they can run on teensy
