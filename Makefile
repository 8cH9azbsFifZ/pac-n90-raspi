build:
	gcc pac-n90.c -lm -lpigpio -pthread -lrt -lpaho-mqtt3c



run:
	sudo ./a.out 
