#!/usr/bin/env bash

EXPERIMENT_NAME="lolo"

# this is the general log directory
LOG_DIRECTORY="/users/gdaneels/logs"
# make it if it does not exist for some reason
mkdir -p $LOG_DIRECTORY

ITERATION=$1
RUNTIME=$2

LOG_FILE="${LOG_DIRECTORY}/log-${ITERATION}.log"
touch $LOG_FILE
ERROR_FILE="${LOG_DIRECTORY}/error-${ITERATION}.log"
touch $ERROR_FILE
OUTPUT_FILE="${LOG_DIRECTORY}/output-${ITERATION}.log"
touch $OUTPUT_FILE

echo "Executing iteration ${ITERATION}." >> $LOG_FILE

EXPERIMENT_PATH="/users/gdaneels/experiments/"

# export the path for this iteration run
echo 'export PATH=$PATH:/users/gdaneels/gcc-arm-none-eabi-5_2-2015q4/bin' >>~/.profile
source ~/.profile

# reinitialize the tsch-topology.c and node.c files
cp /users/gdaneels/tsch-slotbonding-new/os/net/mac/tsch/tsch-topology.c.orig /users/gdaneels/tsch-slotbonding-new/os/net/mac/tsch/tsch-topology.c
cp /users/gdaneels/tsch-slotbonding-new/examples/6tisch/slotbonding/node.c.orig /users/gdaneels/tsch-slotbonding-new/examples/6tisch/slotbonding/node.c
cp /users/gdaneels/tsch-slotbonding-new/examples/6tisch/slotbonding/project-conf.h.orig /users/gdaneels/tsch-slotbonding-new/examples/6tisch/slotbonding/project-conf.h

# enter the make experiment part
cd ~/tsch-slotbonding-new/examples/6tisch/slotbonding/make-experiment

# get the hostname of this node and translate it to a node id
HOSTNAME=$(hostname)
echo "hostname of this node resolves to \"$HOSTNAME\"\n" > ${LOG_FILE}

ITERATION_PATH="${EXPERIMENT_PATH}/${ITERATION}.json"

# prepare the firmware for this iteration by changing the tsch-topology.c and node.c
python3 prepare_firmware.py ${ITERATION_PATH} ${HOSTNAME} 2>>${ERROR_FILE} 1>>${LOG_FILE}

# go back to the example main directory
cd ~/tsch-slotbonding-new/examples/6tisch/slotbonding

# build the software
if [ ! -s ${ERROR_FILE} ]; then
        make TARGET=zoul BOARD=remote-revb node 1>>${LOG_FILE} 2>>${ERROR_FILE}
        make TARGET=zoul BOARD=remote-revb PORT=/dev/ttyUSB0 node.upload 1>>${LOG_FILE} 2>>${ERROR_FILE}
else
        echo "Did not run make command because there was an error before." >> ${ERROR_FILE}
fi

cd ~

# start logging the contiki node
# this has to be a SIGINT
# make PORT=/dev/ttyUSB0 serialview >> ${OUTPUT_FILE}
timeout ${RUNTIME}s python3 read_serial.py ${OUTPUT_FILE} 1>>${LOG_FILE} 2>>${ERROR_FILE}

# go back to the example main directory
cd ~/tsch-slotbonding-new/examples/hello-world

# build the software
if [ ! -s ${ERROR_FILE} ]; then
        make TARGET=zoul BOARD=remote-revb hello-world 1>>${LOG_FILE} 2>>${ERROR_FILE}
        make TARGET=zoul BOARD=remote-revb PORT=/dev/ttyUSB0 hello-world.upload 1>>${LOG_FILE} 2>>${ERROR_FILE}
else
        echo "Did not run make hello-world command because there was an error before." >> ${ERROR_FILE}
fi

cd ~

NFS=/groups/ilabt-imec-be/tsch-slotbonding/exp/${EXPERIMENT_NAME}/datastore/${HOSTNAME%%.*}/${ITERATION}
mkdir -p ${NFS}
cp ${LOG_FILE} ${NFS}
rm ${LOG_FILE}
cp ${ERROR_FILE} ${NFS}
rm ${ERROR_FILE}
cp ${OUTPUT_FILE} ${NFS}
rm ${OUTPUT_FILE}

