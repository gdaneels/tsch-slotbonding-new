#!/usr/bin/env bash

LOG_DIRECTORY="/users/gdaneels/logs"
# make directory with logs if it does not exist yet
mkdir -p $LOG_DIRECTORY

LOG_FILE="${LOG_DIRECTORY}/success.log"
touch ${LOG_FILE}
ERROR_FILE="${LOG_DIRECTORY}/error.log"
touch ${ERROR_FILE}

sudo apt-get -y update 1>>${LOG_FILE} 2>>${ERROR_FILE}
echo '* libraries/restart-without-asking boolean true' | sudo debconf-set-selections
sudo apt-get -y install build-essential doxygen curl python-serial python3-pip srecord rlwrap 1>>${LOG_FILE} 2>>${ERROR_FILE}

# install pyserial for python3
python3 -m pip install pyserial 1>>${LOG_FILE} 2>>${ERROR_FILE}

# untargz the experiments
tar -xvzf /users/gdaneels/experiments.tar.gz 1>>${LOG_FILE} 2>>${ERROR_FILE}

# install ARM
wget https://launchpad.net/gcc-arm-embedded/5.0/5-2015-q4-major/+download/gcc-arm-none-eabi-5_2-2015q4-20151219-linux.tar.bz2 1>>${LOG_FILE} 2>>${ERROR_FILE}
tar xjf gcc-arm-none-eabi-5_2-2015q4-20151219-linux.tar.bz2 1>>${LOG_FILE} 2>>${ERROR_FILE}

# install package to make sure the arm-none-eabi-gcc is found
# https://answers.launchpad.net/gcc-arm-embedded/+question/403082
sudo apt-get -y install gcc-multilib 1>>${LOG_FILE} 2>>${ERROR_FILE}

# make the arm toolchain to the PATH variable
# export PATH=$PATH:/users/gdaneels/gcc-arm-none-eabi-5_2-2015q4/bin
echo 'export PATH=$PATH:/users/gdaneels/gcc-arm-none-eabi-5_2-2015q4/bin' >>~/.profile
source ~/.profile

# add users to plugdev group
sudo usermod -a -G dialout gdaneels 1>>${LOG_FILE} 2>>${ERROR_FILE}
sudo usermod -a -G plugdev gdaneels 1>>${LOG_FILE} 2>>${ERROR_FILE}

# give the run-experiment file the correct permissions
chmod 755 run-experiment.sh 1>>${LOG_FILE} 2>>${ERROR_FILE}

# clone the git repository with the firmware
git clone https://gdaneels:straat93@github.com/gdaneels/tsch-slotbonding-new 1>>${LOG_FILE} 2>>${ERROR_FILE}
cd tsch-slotbonding-new
git submodule update --init --recursive 1>>${LOG_FILE} 2>>${ERROR_FILE}

