/*
 * spi.h
 *
 */

#ifndef SPI_H_
#define SPI_H_

#include <linux/spi/spidev.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <pthread.h>

typedef struct {
        int fd;
        int bits_per_word;
        int mode;
        long speed;
        int flags;
} spi_properties;

void *spi_Vrms(void *arg);
void readVrms(float *VRMS);
void *spi_Irms(void *arg);
void readIrms(float *IRMS);


#endif /* SPI_H_ */
