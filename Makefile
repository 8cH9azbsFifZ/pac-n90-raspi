build:
	gcc pac-n90.c -lm -lpigpio -pthread -lrt


run:
	sudo ./a.out 
