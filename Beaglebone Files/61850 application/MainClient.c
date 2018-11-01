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
/*!	\file		MainClient.c
 *	\brief 		C Source code file for the Example IEC61850 Client using the PIS10 Stack
 *				This Client receives data for a CSWI, GGIO, Phase Measurement and a Cure Description Array
 *				via GOOSE and Reporting.
 *				The Client can Direct Operate the GGIO and use Select before Operate to control the CSWI which
 *				is and enhanced security control.
 *
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

#include "MainClient.h"

int main ()
{
	char continueMainLoop = 'y'; /*Run loop variable */
	enum UserCommands usrCommand = COMMAND_NONE; /* The User command to execute*/
	enum IEC61850_ErrorCodes IEC61850Error = IEC61850_ERROR_NONE; //Variable to hold the IEC61850 Error Codes
	Boolean boolVal = FALSE; //Temporary value to hold user input
    //float VRMS[4]; //holder for V values
	//float IRMS[4]; //holder for I values

	/* SQLite */
	//Not needed for baseline
//	sqlite3 *conn;
//	// sqlite3_stmt *res;
//	int sqlError = 0;
//	// int rec_count = 0;
//	const char *errMSG;
//	const char *tail;
//	char sql_lite[500]=" ";

#if defined (__PARADIGM__)
	// for BECK Set STDIO focus to application
	BIOS_Set_Focus(FOCUS_APPLICATION);
#endif

	IEC61850Error = setupIEC61850Client();

	if(IEC61850Error == IEC61850_ERROR_NONE) //If the Client stated successfully
	{

//        sqlError = sqlite3_open("db_test.db", &conn);
//        if (sqlError)
//        {
//            printf("Can't open database!");
//        }
//
//        sqlError = sqlite3_exec(conn, "pragma journal_mode = WAL", NULL, NULL, NULL);
//        if(sqlError != SQLITE_OK)
//        {
//            printf("Can't start WAL!");
//        }

		while(1)
		{

//            UpdateMMXUPhsAMagi(1.0);
//			UpdateMMXUPhsBMagi(2);
//			UpdateMMXUPhsCMagi(3);
//			UpdateMMXUAAMagf(4);
//			UpdateMMXUABMagf(5);
//			UpdateMMXUACMagf(6);

			PrintClientFullView(); /* Initialize the Screen */

            sleep(0.5);
		}

//        sqlite3_close(conn);
	}

	printf("Stopping IEC61850 Client \n");
	StopMyServerClient(); // Stop Client if running
	printf("Freeing IEC61850 Client \n");
	FreeMyServerClient(); // Free Client if running

	//Allow the program to pause and let the user see any exit messages
	printf("\nIEC61850 Client Example has ended, press enter to exit\n");
	getchar();

#if defined (__PARADIGM__)
	// for BECK Set STDIO focus back to both for terminal and Applications
	BIOS_Set_Focus(FOCUS_BOTH);
#endif

	return 0;
}

/******************************************************************************
*       setupIEC61850Client Function Definition
******************************************************************************/
/*!  \brief         This function Creates, Loads and Starts the IEC61850 Client
 *
 *   \ingroup       Setup Function
 *
 *   \param[in]     Var     DESC
 *
 *   \return        IEC61850_ERROR_NONE on success,
 *   \return        otherwise the relevant enum IEC61850_ErrorCodes
 ******************************************************************************/
enum IEC61850_ErrorCodes setupIEC61850Client()
{
	enum IEC61850_ErrorCodes returnError = IEC61850_ERROR_SERVICE_FAILED;

	if(memcmp(IEC61850_GetLibraryVersion(), PIS10_VERSION, sizeof(PIS10_VERSION)) == 0)  //Make sure the API Header version is the same as the Library
	{
		returnError = CreateClient();

		if(returnError == IEC61850_ERROR_NONE)
		{
			returnError = LoadSCLFile("../cidFiles/client.cid");

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
