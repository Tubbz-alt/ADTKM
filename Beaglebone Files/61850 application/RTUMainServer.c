/******************************************************************************
*
* (c) 2014 	by SystemCORP Embedded Technology Pty Ltd
*
*******************************************************************************
*
* Disclaimer: This program is an example and should be used as such.
*             If you wish to use this program or parts of it in your application,
*             you must validate the code yourself.  SystemCORP Embedded Technology
*             Pty Ltd can not be held responsible for the correct functioning
*			  or coding of this example.
*******************************************************************************/

/*****************************************************************************/
/*!	\file		MainServer.c
 *	\brief 		C Source code file for the Example IEC61850 Server using the PIS10 Stack
 *				This server has a CSWI, GGIO, Phase Measurement and a Cure Description Array
				The server publishes GOOSE packets containing the CSWI and GGIO stVal and q values
				Buffered and UnBuffered reports are available for CSWI, GGIO, Phase Measurement and
				a Cure Description Array to the client to get data from the server.
				All points can also bee read by a client.

 *	\par 		SystemCORP Pty Ltd
 *
 *				15/50 William Street,
 *				Beckenham,
 *				Perth, WA 6107,
 *				Australia.
 *
 *				Phone	: +61 (8) 9258 4683
 *				Fax		: +61 (8) 9258 4673
 *				Email	: support@systemcorp.com.au
 */
/*****************************************************************************/

#if defined (__PARADIGM__)
// for BECK we need the clib.h
#include <clib.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "MainServer.h"

int main ()
{
	char continueMainLoop = 'y'; /*Run loop variable */
	enum UserCommands usrCommand = COMMAND_NONE; /* The User command to execute*/
	enum IEC61850_ErrorCodes IEC61850Error = IEC61850_ERROR_NONE; //Variable to hold the IEC61850 Error Codes
	Integer32 int32Val =0; //Temporary value to hold user input
	Integer8 int8Val =0; //Temporary value to hold user input
	Float32 f32Val = 0.0; //Temporary value to hold user input
	CrvPts CrvPtsVal = {0}; //Temporary value to hold user input
	//float VRMS[6]; //holder for V values
	//float IRMS[3]; //holder for I values

		/* SQLite */
	sqlite3 *conn;
	sqlite3_stmt *res;
	int sqlError = 0;
	// int rec_count = 0;
	const char *errMSG;
	const char *tail;
	char sql_lite[500]=" ";

#if defined (__PARADIGM__)
	// for BECK Set STDIO focus to application
	BIOS_Set_Focus(FOCUS_APPLICATION);
#endif

	IEC61850Error = setupIEC61850Server();

	if(IEC61850Error == IEC61850_ERROR_NONE) //If the server stated successfully
	{
		while(1)
		{

            sqlError = sqlite3_open("db_test.db", &conn);
            if (sqlError)
            {
                printf("Can't open database!");
            }

            sqlError = sqlite3_exec(conn, "pragma journal_mode = WAL", NULL, NULL, NULL);
            if(sqlError != SQLITE_OK)
            {
                printf("Can't start WAL!");
            }

            sqlError = sqlite3_prepare_v2(conn, "SELECT * FROM adc WHERE oid = (SELECT MAX(oid) FROM adc);", -1, &res, NULL);


            if(sqlError != SQLITE_OK)
            {
                printf("We didn't get any data from adc!");
                exit(0);
            }


            while(sqlite3_step(res) == SQLITE_ROW)
            {
                /*Check that the correct values are queried*/
                //printf("%f\n",sqlite3_column_double(res,0));
                //printf("%f\n",sqlite3_column_double(res,1));

                /*Update with client VRMS and IRMS values */
                UpdateMMXUPhsAMagi(sqlite3_column_double(res,0));
                UpdateMMXUPhsBMagi(sqlite3_column_double(res,1));
                UpdateMMXUPhsCMagi(sqlite3_column_double(res,2));
                //UpdateMMXUAAMagf(sqlite3_column_double(res,3));
                //UpdateMMXUABMagf(sqlite3_column_double(res,4));
                //UpdateMMXUACMagf(sqlite3_column_double(res,5));
            }

            sqlError = sqlite3_prepare_v2(conn, "SELECT * FROM adc2 WHERE oid = (SELECT MAX(oid) FROM adc2);", -1, &res, NULL);


            if(sqlError != SQLITE_OK)
            {
                printf("We didn't get any data from adc2!");
                exit(0);
            }


            while(sqlite3_step(res) == SQLITE_ROW)
            {
                /*Check that the correct values are queried*/
                //printf("%f\n",sqlite3_column_double(res,0));
                //printf("%f\n",sqlite3_column_double(res,1));

                /*Update with client VRMS and IRMS values */
                //UpdateMMXUPhsAMagi(sqlite3_column_double(res,0));
                //UpdateMMXUPhsBMagi(sqlite3_column_double(res,1));
                //UpdateMMXUPhsCMagi(sqlite3_column_double(res,2));
                UpdateMMXUAAMagf(sqlite3_column_double(res,3));
                UpdateMMXUABMagf(sqlite3_column_double(res,4));
                UpdateMMXUACMagf(sqlite3_column_double(res,5));
            }


			sqlite3_finalize(res);
			sqlite3_close(conn);

			PrintServerFullView(); /* Initialize the Screen */

            sleep(0.5);

		}
	}

	printf("Stopping IEC61850 Server \n");
	StopMyServerClient(); // Stop server if running
	printf("Freeing IEC61850 Server \n");
	FreeMyServerClient(); // Free server if running

	//Allow the program to pause and let the user see any exit messages
	printf("\nIEC61850 Server Example has ended, press enter to exit\n");
	getchar();

#if defined (__PARADIGM__)
	// for BECK Set STDIO focus back to both for terminal and Applications
	BIOS_Set_Focus(FOCUS_BOTH);
#endif

	return 0;
}

/******************************************************************************
*       setupIEC61850 Function Definition
******************************************************************************/
/*!  \brief         This function Creates, Loads and Starts the IEC61850 Server
 *
 *   \ingroup       Setup Function
 *
 *   \param[in]     Var     DESC
 *
 *   \return        IEC61850_ERROR_NONE on success,
 *   \return        otherwise the relevant enum IEC61850_ErrorCodes
 ******************************************************************************/
enum IEC61850_ErrorCodes setupIEC61850Server()
{
	enum IEC61850_ErrorCodes returnError = IEC61850_ERROR_SERVICE_FAILED;

	if(memcmp(IEC61850_GetLibraryVersion(), PIS10_VERSION, sizeof(PIS10_VERSION)) == 0)
	{
		returnError = CreateServer(IEC61850_OPTION_NONE);

		if(returnError == IEC61850_ERROR_NONE)
		{
			returnError = LoadSCLFile("../cidFiles/RTUserver.cid");

			if(returnError == IEC61850_ERROR_NONE)
			{
				returnError = StartMyServerClient();
			}
			else if(returnError == IEC61850_ERROR_LICENCE_INVALID)
			{
				printf("IEC61850 License for this device is invalid\n");
			}
		}
		else if(returnError == IEC61850_ERROR_LICENCE_INVALID)
		{
            printf("IEC61850 License for this device is invalid\n");
		}
	}
	else
	{
		printf("Version Mismatch: PIS10 IEC61850API.h Version = %s , PIS10 Library Version = %s\n",PIS10_VERSION ,IEC61850_GetLibraryVersion());
	}
	return returnError;
}
