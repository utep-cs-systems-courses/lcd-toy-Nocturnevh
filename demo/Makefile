# makfile configuration
CPU             	= msp430g2553
CFLAGS          	= -mmcu=${CPU} -Os -I../h
LDFLAGS		= -L../lib -L/opt/ti/msp430_gcc/include/

#switch the compiler (for the internal make rules)
CC              = msp430-elf-gcc
AS              = msp430-elf-gcc -mmcu=${CPU} -c

all:demo.elf

#additional rules for files
demo.elf: ${COMMON_OBJECTS} demo.o wdt_handler.o switches.o
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ -lTimer -lLcd

load: demo.elf
	msp430loader.sh $^

#additional rules for files (Assembly)
demo-s.elf: ${COMMON_OBJECTS} demo.o wdt_handler.o switches.o
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ -lTimer -lLcd

load-s: demo-s.elf
	msp430loader.sh $^

clean:
	rm -f *.o *.c~ *.elf Makefile~

demo.o: demo.h switches.h
switches.o: switches.h
