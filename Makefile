build:
	gcc pac-n90.c -lm -lpigpio -pthread -lrt -lpaho-mqtt3c -opac-n90

install:
	sudo install pac-n90 /usr/local/bin
	sudo install pac-n90.service /etc/systemd/system/



run:
	sudo ./pac-n90
