#!/bin/bash
sudo apt-get update
sudo apt-get -y upgrade

# Install essential stuff
sudo apt-get -y install vim git build-essential
sudo apt-get -y install libpigpio-dev libpigpio1 

# Install MQTT stuff
sudo apt-get -y install build-essential gcc make cmake cmake-gui cmake-curses-gui
sudo apt-get -y install fakeroot fakeroot devscripts dh-make lsb-release
sudo apt-get -y install libssl-dev
sudo apt-get -y install doxygen graphviz
cd
git clone https://github.com/eclipse/paho.mqtt.c.git
cd paho.mqtt.c
make
sudo make install


# Install PAC remote control stuff
cd
git clone https://github.com/8cH9azbsFifZ/pac-n90-raspi.git
cd pac-n90-raspi
make build
