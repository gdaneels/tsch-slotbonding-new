#!/usr/bin/env bash

LOG_DIRECTORY="/users/gdaneels/logs"
# make directory with logs if it does not exist yet
mkdir -p $LOG_DIRECTORY

LOG_FILE="${LOG_DIRECTORY}/success.log"
touch ${LOG_FILE}
ERROR_FILE="${LOG_DIRECTORY}/error.log"
touch ${ERROR_FILE}

sudo apt-get -y update
echo '* libraries/restart-without-asking boolean true' | sudo debconf-set-selections
sudo apt-get -y install build-essential doxygen curl python-serial srecord rlwrap

# install ARM
wget https://launchpad.net/gcc-arm-embedded/5.0/5-2015-q4-major/+download/gcc-arm-none-eabi-5_2-2015q4-20151219-linux.tar.bz2
tar xjf gcc-arm-none-eabi-5_2-2015q4-20151219-linux.tar.bz2

# install package to make sure the arm-none-eabi-gcc is found
# https://answers.launchpad.net/gcc-arm-embedded/+question/403082
sudo apt-get -y install gcc-multilib

# make the arm toolchain to the PATH variable
# export PATH=$PATH:/users/gdaneels/gcc-arm-none-eabi-5_2-2015q4/bin
echo 'export PATH=$PATH:/users/gdaneels/gcc-arm-none-eabi-5_2-2015q4/bin' >>~/.profile
source ~/.profile

# add users to plugdev group
sudo usermod -a -G dialout gdaneels
sudo usermod -a -G plugdev gdaneels

# clone the git repository with the firmware
git clone https://gdaneels:straat93@github.com/gdaneels/tsch-slotbonding

# enter the slotbonding example
cd tsch-slotbonding/examples/6tisch/multi-phy-slotbonding/make-experiment

HOSTNAME=$(hostname)
echo "hostname of this node resolves to \"$HOSTNAME\"\n" > ${LOG_FILE}

NODEID=$(python3 hostnames.py $HOSTNAME 2>>${ERROR_FILE})
echo "nodeid of this node resolves to \"$NODEID\"\n" >> ${LOG_FILE}

# which experiment should I execute?
ITERATION_NAME="1.json"
EXPERIMENT_NAME="initial-test"
EXPERIMENT_PATH="../experiments/${EXPERIMENT_NAME}/${ITERATION_NAME}"

python3 prepare_firmware.py ${EXPERIMENT_PATH} ${NODEID}  2>>${ERROR_FILE} 1>>${LOG_FILE}

# go back to the example main directory
cd ../

# build the software
if [ ! -s ${ERROR_FILE} ]; then
	make TARGET=zoul BOARD=remote-revb PORT=/dev/ttyUSB0 NODEID=${NODEID} node.upload 2>>${ERROR_FILE} 1>>${LOG_FILE}
else
	echo "Did not run make command because there was an error before." >> ${ERROR_FILE}
fi

