build:
	gcc src/pac-n90.c -lm -lpigpio -pthread -lrt -lpaho-mqtt3c -opac-n90

install:
	sudo install pac-n90 /usr/local/bin
	sudo install src/pac-n90.service /etc/systemd/system/
	sudo systemctl start  pac-n90
	sudo systemctl enable pac-n90

prepare_raspi:
	touch /Volumes/boot/ssh
	cp lib/wpa_supplicant.conf /Volumes/boot/
	vim /Volumes/boot/wpa_supplicant.conf

run:
	sudo ./pac-n90
