#! /bin/bash

make &&sudo insmod arnamodule.ko
TMP=`cat /proc/devices | grep ARNA | cut -d" " -f 1`
echo $TMP
sudo mknod /dev/arnanode c $TMP $RANDOM


# RUN TEST

sudo python3 pytest.py

sudo rm -rf /dev/arnanode &&sudo rmmod arnamodule && make clean

echo "$(tput setaf 0) $(tput setab 2) have a nice time. bye bye! $(tput sgr 0)"

