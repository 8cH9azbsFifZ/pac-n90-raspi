# pac-n90-raspi
IR remote control for DeLonghi PAC N90 Eco on Raspi.

# Dependencies
- Prepare a raspia W zero 
- Prepare and wire an IR transmitter module ()
- PIN DAT - 18 (GPIO24)
- PIN VCC - PIN 2 (5V)
- PIN GND - 20 (GND)
- Download raspian buster lite and flash it (https://www.raspberrypi.org/downloads/raspbian/)
- make prepare_raspi

sudo apt-get install libpigpio
Install: paho.mqtt.c


# Configuration
- Adjust hostname & co for mqtt
- Adjust topics for mqtt
- make build
- make install

# Starting

sudo systemctl start  pac-n90
sudo systemctl enable pac-n90


# Reverse engineering
cat off_synth |grep "^1"|awk 'NR>1{if($2<700){a=a"0"}if($2>700){a=a"1"}}END{print a}'

mosquitto_pub -h t20 -t pac/toggle/power -m off
 mosquitto_sub -h t20 -t pac/power
mosquitto_pub -h t20 -t pac/toggle/temperature -m 30




## References
- https://blog.bschwind.com/2016/05/29/sending-infrared-commands-from-a-raspberry-pi-without-lirc/
- https://github.com/zeroflow/ESPAircon
- https://github.com/bschwind/ir-slinger
