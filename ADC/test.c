
#include "spi_thread.h"

int main()
{ 
	float VRMS[4];	
	float IRMS[4];
	int i = 0;
		
	readVrms(VRMS);
	readIrms(IRMS);
	
	for(i = 0; i < 4; i++)
	{
		syslog(LOG_INFO, "%0.2fV is VoltageRMS, %0.2fA is CurrentRMS.", VRMS[i], IRMS[i]);
	}

	syslog(LOG_INFO, "%s", "Finished spi example.");

	return 0;	
}
