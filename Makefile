pac-n90:
	gcc  src/pac-n90.c -lm -lpigpio -pthread -lrt -lpaho-mqtt3c -opac-n90

honeywell:
	gcc  src/pac-n90.c -lm -lpigpio -pthread -lrt -lpaho-mqtt3c -ohoneywell

install:
	sudo install pac-n90 /usr/local/bin
	sudo install src/pac-n90.service /etc/systemd/system/
	sudo install honeywell /usr/local/bin
	sudo install src/honeywell.service /etc/systemd/system/	


uninstall:
	sudo systemctl stop  pac-n90
	sudo systemctl disable pac-n90
	sudo systemctl stop  honeywell
	sudo systemctl disable honeywell
	sudo rm /usr/local/bin/pac-n90
	sudo rm /etc/systemd/system/pac-n90.service
	sudo rm /usr/local/bin/honeywell
	sudo rm /etc/systemd/system/honeywell.service

run:
	sudo ./pac-n90
