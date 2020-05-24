pac-n90:
	gcc  src/pac-n90.c -lm -lpigpio -pthread -lrt -lpaho-mqtt3c -opac-n90

honeywell:
	gcc  src/pac-n90.c -lm -lpigpio -pthread -lrt -lpaho-mqtt3c -ohoneywell

install:
	sudo install pac-n90 /usr/local/bin
	sudo install src/pac-n90.service /etc/systemd/system/
	sudo systemctl start  pac-n90
	sudo systemctl enable pac-n90


uninstall:
	sudo systemctl stop pac-n90
	sudo systemctl disable pac-n90
	sudo rm /usr/local/bin/pac-n90
	sudo rm /etc/systemd/system/pac-n90.service

run:
	sudo ./pac-n90
