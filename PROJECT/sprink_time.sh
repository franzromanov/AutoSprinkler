avr-gcc -Os -mmcu=attiny13 -DF_CPU=9600000UL -c -o sprink_time.o sprink_time.c
avr-gcc -mmcu=attiny13 -o sprink_time sprink_time.o
avr-objcopy -O ihex -R .eeprom sprink_time sprink_time.hex

scp -P 22 sprink_time.hex neocrackers@192.168.1.10:/home/neocrackers
