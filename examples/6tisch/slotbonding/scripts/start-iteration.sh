#!/bin/bash

LOG_DIRECTORY="logs"
# make directory with logs if it does not exist yet
mkdir -p LOG_DIRECTORY

LOG_FILE="${LOG_DIRECTORY}/success.log"
ERROR_FILE="${LOG_DIRECTORY}/error.log"

# enter the slotbonding example
cd tsch-slotbonding/examples/6tisch/multi-phy-slotbonding/make-experiment

# prepare the Contiki firmware for this node
python prepare_firmware.py 2>${ERROR_FILE} 1>${LOG_FILE}

# build the software
# make TARGET=zoul BOARD=remote-revb PORT=/dev/ttyUSB0 node
