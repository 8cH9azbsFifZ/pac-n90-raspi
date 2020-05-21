build:
	gcc test.c -lm -lpigpio -pthread -lrt


run:
	sudo ./a.out 

cat off_synth |grep "^1"|awk 'NR>1{if($2<700){a=a"0"}if($2>700){a=a"1"}}END{print a}'

