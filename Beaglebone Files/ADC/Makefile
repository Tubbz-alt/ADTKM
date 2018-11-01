CC=gcc

PROGS=adc

.DUMMY: all
all: $(PROGS)

adc: spi_thread.h spi_thread.c test.c
	$(CC) -o adc test.c spi_thread.c -lm -pthread

.DUMMY: clean
clean:
	$(RM) $(PROGS)
