#!/bin/bash

BUILDER=/home/matt/Downloads/arduino-1.8.6/arduino-builder
HARDWARE=/home/matt/Downloads/arduino-1.8.6/hardware
TOOLS=/home/matt/Downloads/arduino-1.8.6/tools-builder

$BUILDER -compile -logger=machine -hardware $HARDWARE -libraries /usr/include -tools $TOOLS -fqbn=teensy:avr:teensy36:usb=serial,speed=120,opt=o2std,keys=en-us -ide-version=10806  -warnings=all -verbose PANode.ino
