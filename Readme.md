## What

(2026-04)

ATmega328 hardware I2C interface.

Implemented only what I required: Master mode and no timeouts.

## Stuff

* Interface
  * [Frontend][Interface_front]
  * [Backend][Interface_back]
* Implementation
  * [Frontend][Implementation_front]
  * [Backend][Implementation_back]
* [Example][Example]
* [Notes][Notes]

## Requirements

  * arduino-cli
  * bash

## Install/remove

Clone [GetLibs][GetLibs] repo and run it's code to get all my stuff.

Low-level implementation is self-sufficient. External interface uses
streams.

## See also

* [My other embedded C++ libraries][Embedded]
* [My other repositories][Repos]

[Interface_front]: src/me_I2C.h
[Implementation_front]: src/me_I2C.cpp
[Interface_back]: src/I2C.Bare.ATmega328.h
[Implementation_back]: src/I2C_Master.Bare.ATmega328.cpp
[Example]: examples/me_I2C/me_I2C.ino
[Notes]: extras/Notes.txt

[GetLibs]: https://github.com/martin-eden/Embedded-Framework-GetLibs

[Embedded]: https://github.com/martin-eden/Embedded_Crafts/tree/master/Parts
[Repos]: https://github.com/martin-eden/contents
