/*
 * spi_thread.c
 *
 */

#include "spi_thread.h"
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <math.h>
#include <time.h>


//initialize variables and ADC for Voltage RMS calculations
void *spi_Vrms(void *arg)
{
	// allocate and initialize spi properties
        spi_properties *spi = malloc(sizeof(spi_properties));
        spi->bits_per_word = 8;
        spi->mode = 0;
        spi->speed = 1000000;
        spi->flags = O_RDWR;

	int i = 0;

        spi->fd = open("/dev/spidev1.0", spi->flags);
        if(spi->fd < 0)
        {
                perror("could not open spi.");
		return; 
        }

        if(ioctl(spi->fd, SPI_IOC_WR_MODE, &spi->mode)==-1)
        {
                perror("SPI: Can't set SPI mode.");
		return;
        }

        if (ioctl(spi->fd, SPI_IOC_WR_BITS_PER_WORD, &spi->bits_per_word)==-1)
        {
                perror("SPI: Can't set bits per word.");
                return;
        }

        if (ioctl(spi->fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi->speed)==-1)
        {
                perror("SPI: Can't set max speed HZ");
                return;
        }

	unsigned char tx[4][3];
        unsigned char rx[3];

        // initialize transmission to setup 1 channels of ADC
        //Voltages
	tx[0][0] = 0b00000110;
	tx[0][1] = 0b00000000;
	tx[0][2] = 0b00000000;
        tx[1][0] = 0b00000110;
	tx[1][1] = 0b01000000;
	tx[1][2] = 0b00000000;
	tx[2][0] = 0b00000110;
	tx[2][1] = 0b10000000;
	tx[2][2] = 0b00000000;
	tx[3][0] = 0b00000110;
	tx[3][1] = 0b11000000;
	tx[3][2] = 0b00000000;

        struct spi_ioc_transfer transfer;
        transfer.tx_buf = (unsigned long)tx[0];
        transfer.rx_buf = (unsigned long)rx;
        transfer.len = 3;
        transfer.delay_usecs = 0;
        transfer.speed_hz = spi->speed;
        transfer.bits_per_word = spi->bits_per_word;
        transfer.tx_nbits = 0;
        transfer.rx_nbits = 0;
        transfer.cs_change = 0;

        time_t start,end;

        float *VRMS;
        int status = 0;
        float samples;
        
        int samps[4000];

        VRMS=(float*)arg;

        float sqSum[4];
        int count[4];

        start = clock();

        for(i = 0; i < 4000; i++)
        {
                transfer.tx_buf = (unsigned long)tx[i%4];
                status = ioctl(spi->fd, SPI_IOC_MESSAGE(1), &transfer);

                samps[i] = (int)(((rx[1]&0b00001111)<<8)|rx[2]);
        }
        end = clock();

        for(i = 0; i < 4000; i++)
        {
                samples = (((float)samps[i]/4095.0)*10)-5;

                sqSum[i%4] = pow(samples, 2) + sqSum[i%4];
                count[i%4] = count[i%4] + 1;
        }

        float msec = (float)(end - start)/CLOCKS_PER_SEC;
	
	for(i = 0; i < 4; i++)
	{
        	VRMS[i] = sqrt(sqSum[i]/count[i]);
	//	syslog(LOG_INFO, "VRMS: %f, %d, count: %d", VRMS[i], i, count[i]);
	}
        
        syslog(LOG_INFO, "msecs: %f", msec);
        syslog(LOG_INFO, "spi closed - spi:%d", spi->fd);
        close(spi->fd);

       	return;
}
         
void readVrms(float *VRMS)
{
	pthread_t pth;

	pthread_create(&pth, NULL, spi_Vrms, VRMS);

	pthread_join(pth, NULL);			
}

void *spi_Irms(void *arg)
{
	// allocate and initialize spi properties
        spi_properties *spi = malloc(sizeof(spi_properties));
        spi->bits_per_word = 8;
        spi->mode = 0;
        spi->speed = 1000000;
        spi->flags = O_RDWR;

        int i = 0;

        spi->fd = open("/dev/spidev1.0", spi->flags);
        if(spi->fd < 0)
        {
                perror("could not open spi.");
                return;
        }

        if(ioctl(spi->fd, SPI_IOC_WR_MODE, &spi->mode)==-1)
        {
                perror("SPI: Can't set SPI mode.");
                return;
        }

        if (ioctl(spi->fd, SPI_IOC_WR_BITS_PER_WORD, &spi->bits_per_word)==-1)
        {
                perror("SPI: Can't set bits per word.");
                return;
        }

        if (ioctl(spi->fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi->speed)==-1)
        {
                perror("SPI: Can't set max speed HZ");
                return;
        }	

        unsigned char tx[4][3];
        unsigned char rx[3];

        // initialize transmission to setup 1 channels of ADC
        //Voltages
        tx[0][0] = 0b00000111;
        tx[0][1] = 0b00000000;
        tx[0][2] = 0b00000000;
        tx[1][0] = 0b00000111;
        tx[1][1] = 0b01000000;
        tx[1][2] = 0b00000000;
        tx[2][0] = 0b00000111;
        tx[2][1] = 0b10000000;
        tx[2][2] = 0b00000000;
        tx[3][0] = 0b00000111;
        tx[3][1] = 0b11000000;
        tx[3][2] = 0b00000000;

        struct spi_ioc_transfer transfer;
        transfer.tx_buf = (unsigned long)tx[0];
        transfer.rx_buf = (unsigned long)rx;
        transfer.len = 3;
        transfer.delay_usecs = 0;
        transfer.speed_hz = spi->speed;
        transfer.bits_per_word = spi->bits_per_word;
        transfer.tx_nbits = 0;
        transfer.rx_nbits = 0;
        transfer.cs_change = 0;

        time_t start,end;

        float *IRMS;
        int status = 0;
        float samples;

        int samps[4000];

        IRMS=(float*)arg;

        float sqSum[4];
        int count[4];

        start = clock();

        for(i = 0; i < 4000; i++)
        {
                transfer.tx_buf = (unsigned long)tx[i%4];
                status = ioctl(spi->fd, SPI_IOC_MESSAGE(1), &transfer);

                samps[i] = (int)(((rx[1]&0b00001111)<<8)|rx[2]);
        }
        end = clock();

        for(i = 0; i < 4000; i++)
        {
                samples = (((float)samps[i]/4095.0)*10)-5;

                sqSum[i%4] = pow(samples, 2) + sqSum[i%4];
                count[i%4] = count[i%4] + 1;
        }

        float msec = (float)(end - start)/CLOCKS_PER_SEC;

        for(i = 0; i < 4; i++)
        {
                IRMS[i] = sqrt(sqSum[i]/count[i]);
        //      syslog(LOG_INFO, "VRMS: %f, %d, count: %d", VRMS[i], i, count[i]);
        }

        syslog(LOG_INFO, "msecs: %f", msec);
        syslog(LOG_INFO, "spi closed - spi:%d", spi->fd);
        close(spi->fd);
	
	return;	
}

void readIrms(float *IRMS)
{
	pthread_t pth;
	
	pthread_create(&pth, NULL, spi_Irms, IRMS);

	pthread_join(pth, NULL);
}
