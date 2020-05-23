# pac-n90-raspi
IR remote control for DeLonghi PAC AirConditioners on Raspi with MQTT support for integration in homeautomation systems (i.e. OpenHAB).

| Device Name   | Status        |
| ------------- |:-------------:| 
| PAC N81       | Working       |
| PAC N90 Eco   | Working       |
| PAC NK76      | Not tested    |


# Dependencies
- Prepare a Raspi W Zero 
- Prepare and wire an IR transmitter module (i.e. [IR Transceiver](https://www.amazon.de/HALJIA-Digital-Infrarot-IR-Empf%C3%A4nger-Sensor-Modul-Transmitter/dp/B07BFNGF53))

| PIN GPIO          | PIN IR Transmitter  |
| ----------------- |:-------------------:| 
| 18 (GPIO24)       | PIN DAT             |
| PIN 2 (5V)        | PIN VCC             |
| 20 (GND)          | PIN GND             |

![Raspi GPIO Pins][gpio]


- Download [raspian](https://www.raspberrypi.org/downloads/raspbian/) buster lite and flash it (i.e. [Belena Etcher](https://www.balena.io/etcher/)).
- make prepare_raspi
- Boot it, and adjust hostname (and fixed IP on your router?) password raspberry (default)
```
ssh -lpi <your_ip>
sudo su
echo klima-raspi > /etc/hostname
reboot
```
- Run install script for preparation  `/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/8cH9azbsFifZ/pac-n90-raspi/master/lib/install.sh)"` or clone this repo and run `./lib/install.sh`.
- Enable IR Mode for PINS by uncommenting the following lines:
```
vim /boot/config.txt 
dtoverlay=gpio-ir,gpio_pin=17
dtoverlay=gpio-ir-tx,gpio_pin=18
```

# Configuration
- Configure MQTT variables
```
vim src/pac-n90.c

#define MQTT_ADDRESS              "t20:1883"     
#define MQTT_TOPIC_ROOT           "n90/"          
#define PAC_NAME                  "DeLonghi PAC N90 Eco"        
```

- Build and install the software
```
make build
make install
sudo systemctl start  pac-n90
```

# Reverse engineering a new remote
- use the bits.c			identify.py in lib
- extract the bit patterns using stuff liek: cat off_synth |grep "^1"|awk 'NR>1{if($2<700){a=a"0"}if($2>700){a=a"1"}}END{print a}'

# Testing the installation
```
mosquitto_pub -h t20 -t pac/toggle/power -m off
mosquitto_sub -h t20 -t pac/power
mosquitto_pub -h t20 -t pac/toggle/temperature -m 30
```

## References
- https://blog.bschwind.com/2016/05/29/sending-infrared-commands-from-a-raspberry-pi-without-lirc/
- https://github.com/zeroflow/ESPAircon
- https://github.com/bschwind/ir-slinger
- https://github.com/QuadrifoglioVerde/DL_PAC_NK76

[gpio]: https://raw.githubusercontent.com/8cH9azbsFifZ/pac-n90-raspi/master/doc/pi-gpio-768x576.png "Raspi GPIO Pins"
