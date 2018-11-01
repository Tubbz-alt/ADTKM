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
/*!	\file		PrintView.c
 *	\brief 		C Source code file for the PIS10 Stack Example
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

#include <stdlib.h>
#include <sqlite3.h>
#include "PrintView.h"

	/* SQLite */
	sqlite3 *conn;
	// sqlite3_stmt *res;
	int sqlError = 0;
	// int rec_count = 0;
	const char *errMSG;
	const char *tail;
	char sql_lite[500]=" ";
	int i=0;

	void PrintDataView2();
	void PrintFullDataView();


/******************************************************************************
*       PrintServerFullView Function Definition
******************************************************************************/
/*!  \brief         This function prints the Server Menu and Data View to the screen
 *
 *   \ingroup       View Function
 *
 *   \param[in]     Var     DESC
 *
 *   \return        N/A
 ******************************************************************************/
void PrintServerFullView()
{
	ClearScreen();
	PrintServerHeader();
	PrintFullDataView();
	//PrintErrorString();
	//PrintServerMenuView();
	//printf("Select a Command: ");
    fflush(stdout); // Will now print everything in the stdout buffer
}


/******************************************************************************
*       PrintClientFullView Function Definition
******************************************************************************/
/*!  \brief         This function prints the Client Menu and Data View to the screen
 *
 *   \ingroup       View Function
 *
 *   \param[in]     Var     DESC
 *
 *   \return        N/A
 ******************************************************************************/
void PrintClientFullView()
{
        ClearScreen();
        PrintClientHeader();
        PrintDataView();
        //PrintErrorString();
        //PrintClientMenuView();
        //printf("Select a Command: ");
        fflush(stdout); // Will now print everything in the stdout buffer

        //not needed for baseline RTU
//        sqlError = -1;
//
//
//        while(sqlError != SQLITE_OK)
//        {
//
//            sprintf(sql_lite, "INSERT INTO adc (f1, f2, f3, f4, f5, f6) values ('%f', '%f', '%f', '%f', '%f', '%f');",GetMMXUPhsAMagi(), GetMMXUPhsBMagi(), GetMMXUPhsCMagi(), GetMMXUAAMagf(), GetMMXUABMagf(), GetMMXUACMagf());
//            sqlError = sqlite3_exec(conn, sql_lite, 0, 0, 0);
//
//            if (sqlError != SQLITE_OK)
//            {
//
//                printf("We did not get any data from adc! %d", sqlError);
//            }
//        }

}

/******************************************************************************
*       PrintClient2FullView Function Definition
******************************************************************************/
/*!  \brief         This function prints the Client Menu and Data View to the screen
 *
 *   \ingroup       View Function
 *
 *   \param[in]     Var     DESC
 *
 *   \return        N/A
 ******************************************************************************/
void PrintClient2FullView()
{
	ClearScreen();
	PrintClientHeader();
	PrintDataView2();
	//PrintErrorString();
	//PrintClientMenuView();
	//printf("Select a Command: ");
	fflush(stdout); // Will now print everything in the stdout buffer

	//not needed for baseline RTU
//	sqlError = -1;
//
//	while(sqlError != SQLITE_OK)
//    {
//
//        sprintf(sql_lite, "INSERT INTO adc2 (f1, f2, f3, f4, f5, f6) values ('%f', '%f', '%f', '%f', '%f', '%f');",GetMMXUPhsAMagi(), GetMMXUPhsBMagi(), GetMMXUPhsCMagi(), GetMMXUAAMagf(), GetMMXUABMagf(), GetMMXUACMagf());
//        sqlError = sqlite3_exec(conn, sql_lite, 0, 0, 0);
//
//        if (sqlError != SQLITE_OK)
//        {
//            printf("We did not get any data from adc2! %d", sqlError);
//        }
//    }

}

/******************************************************************************
*       PrintServerSubscriptionFullView Function Definition
******************************************************************************/
/*!  \brief         This function prints the Server Subscription Menu and Data View to the screen
 *
 *   \ingroup       View Function
 *
 *   \param[in]     Var     DESC
 *
 *   \return        N/A
 ******************************************************************************/
void PrintServerSubscriptionFullView()
{
	ClearScreen();
	PrintServerSubscriptionHeader();
	PrintServerSubscriptionDataView();
	PrintErrorString();
	PrintServerSubscriptionMenuView();
	printf("Select a Command: ");
    fflush(stdout); // Will now print everything in the stdout buffer
}


/******************************************************************************
*       PrintServerHeader Function Definition
******************************************************************************/
/*!  \brief         This function prints the Server Header to the screen
 *
 *   \ingroup       View Function
 *
 *   \param[in]     Var     DESC
 *
 *   \return        N/A
 ******************************************************************************/
void PrintServerHeader()
{
	printf("----------------------------------------------------------------------------\n");
	printf("IEC61850 Server Example from SystemCORP Embedded Technology Pty Ltd \nPIS10: %s\n", IEC61850_GetLibraryVersion());
	printf("----------------------------------------------------------------------------\n");
}

/******************************************************************************
*       PrintClientHeader Function Definition
******************************************************************************/
/*!  \brief         This function prints the Client Header to the screen
 *
 *   \ingroup       View Function
 *
 *   \param[in]     Var     DESC
 *
 *   \return        N/A
 ******************************************************************************/
void PrintClientHeader()
{
	printf("----------------------------------------------------------------------------\n");
	printf("IEC61850 Client Example from SystemCORP Embedded Technology Pty Ltd \nPIS10: %s\n", IEC61850_GetLibraryVersion());
	printf("----------------------------------------------------------------------------\n");
}

/******************************************************************************
*       PrintServerSubscriptionHeader Function Definition
******************************************************************************/
/*!  \brief         This function prints the Server Subscription Header to the screen
 *
 *   \ingroup       View Function
 *
 *   \param[in]     Var     DESC
 *
 *   \return        N/A
 ******************************************************************************/
void PrintServerSubscriptionHeader()
{
	printf("----------------------------------------------------------------------------\n");
	printf("IEC61850 Server Subscription Example from SystemCORP Embedded Technology Pty Ltd \nPIS10: %s\n", IEC61850_GetLibraryVersion());
	printf("----------------------------------------------------------------------------\n");
}

/******************************************************************************
*       PrintDataView Function Definition
******************************************************************************/
/*!  \brief         This function prints the Data View to the screen
 *
 *   \ingroup       View Function
 *
 *   \param[in]     Var     DESC
 *
 *   \return        N/A
 ******************************************************************************/
void PrintDataView()
{
	Integer8 i = 0; //Iterator Variable for the for loops

	printf("=================== IEC61850 Data ==========================================\n");

	/* printf("CSWI:\t CtlVal: %s\t StVal: %s\n",BooleanToString(GetCSWICtlVal()), DBPosToString(GetCSWIStVal()));    //Print CSWI Data
	 * printf("GGIO:\t CtlVal: %s\t StVal: %s\n",BooleanToString(GetGGIOCtlVal()), BooleanToString(GetGGIOCtlVal())); //Print GGIO Data
	 * printf("\n"); //new line for formating
	 */


#ifdef __PARADIGM__

	//Print MMXU Data
	printf("MMXU:\t PhsA$Mag$i: %ld\t ", GetMMXUPhsAMagi());  //Integer32 on beck is a long int so %ld is needed
    printf("PhsB$Mag$i: %ld\t ", GetMMXUPhsBMagi());
    printf("PhsC$Mag$i: %ld\n",GetMMXUPhsCMagi());
	printf("\n"); //new line for formating

	/* We do not support Floating point on BECK devices
	 * Therefore we only add the CrvPts array if this
	 * is not being compiled for BECK devices
	 * */
#else

	//Print MMXU Data
	printf("PhsV$PhsA$Mag$f: %.2f\t ", GetMMXUPhsAMagi());
	printf("PhsV$PhsB$Mag$f: %.2f\t ", GetMMXUPhsBMagi());
	printf("PhsV$PhsC$Mag$f: %.2f\n",GetMMXUPhsCMagi());
	printf("A$PhsA$Mag$f: %.2f\t ", GetMMXUAAMagf());
	printf("A$PhsB$Mag$f: %.2f\t ", GetMMXUABMagf());
	printf("A$PhsC$Mag$f: %.2f\n", GetMMXUACMagf());
    printf("GGIO:\t CtlVal: %s\t StVal: %s\n",BooleanToString(GetGGIOCtlVal()), BooleanToString(GetGGIOStVal())); //Print GGIO Data
	printf("\n"); //new line for formating

	/* Print PDIF DATA xVal array */
	/* printf("PDIF:\t xVal = { ");

	for(i=0; i <SIZE_OF_PDIF_ARRAY-1; i++)
	{
		printf("%2.2f, ", GetPDIFCrvPts(i).xVal);
	}
	printf("%3.2f }\n", GetPDIFCrvPts(SIZE_OF_PDIF_ARRAY-1).xVal); //Print the last element in the array with the closing bracket and new line

	/* Print PDIF DATA yVal array */
	/* printf("PDIF:\t yVal = { ");

	/* for(i=0; i <SIZE_OF_PDIF_ARRAY-1; i++)
	/* {
		printf("%3.2f, ", GetPDIFCrvPts(i).yVal);
	/* }
	/* printf("%2.2f }\n", GetPDIFCrvPts(SIZE_OF_PDIF_ARRAY-1).yVal); //Print the last element in the array with the closing bracket and new line

	/* printf("\n"); */ //new line for formating

#endif
}

/******************************************************************************
*       PrintDataView Function Definition
******************************************************************************/
/*!  \brief         This function prints the Data View to the screen
 *
 *   \ingroup       View Function
 *
 *   \param[in]     Var     DESC
 *
 *   \return        N/A
 ******************************************************************************/
void PrintDataView2()
{
	//Integer8 i = 0; //Iterator Variable for the for loops

	printf("=================== IEC61850 Data ==========================================\n");

	/* printf("CSWI:\t CtlVal: %s\t StVal: %s\n",BooleanToString(GetCSWICtlVal()), DBPosToString(GetCSWIStVal()));    //Print CSWI Data
	 * printf("GGIO:\t CtlVal: %s\t StVal: %s\n",BooleanToString(GetGGIOCtlVal()), BooleanToString(GetGGIOCtlVal())); //Print GGIO Data
	 * printf("\n"); //new line for formating
	 */


#ifdef __PARADIGM__

	//Print MMXU Data
	printf("MMXU:\t PhsA$Mag$i: %ld\t ", GetMMXUPhsAMagi());  //Integer32 on beck is a long int so %ld is needed
    printf("PhsB$Mag$i: %ld\t ", GetMMXUPhsBMagi());
    printf("PhsC$Mag$i: %ld\n",GetMMXUPhsCMagi());
	printf("\n"); //new line for formating

	/* We do not support Floating point on BECK devices
	 * Therefore we only add the CrvPts array if this
	 * is not being compiled for BECK devices
	 * */
#else

	//Print MMXU Data
	printf("PhsV$PhsA$Mag$f: %.2f\t ", GetMMXU2PhsAMagi());
	printf("PhsV$PhsB$Mag$f: %.2f\t ", GetMMXU2PhsBMagi());
	printf("PhsV$PhsC$Mag$f: %.2f\n",GetMMXU2PhsCMagi());
	printf("A$PhsA$Mag$f: %.2f\t ", GetMMXU2AAMagf());
	printf("A$PhsB$Mag$f: %.2f\t ", GetMMXU2ABMagf());
	printf("A$PhsC$Mag$f: %.2f\n", GetMMXU2ACMagf());
    printf("GGIO:\t CtlVal: %s\t StVal: %s\n",BooleanToString(GetGGIO2CtlVal()), BooleanToString(GetGGIO2StVal())); //Print GGIO Data
	printf("\n"); //new line for formating

	/* Print PDIF DATA xVal array */
	/* printf("PDIF:\t xVal = { ");

	for(i=0; i <SIZE_OF_PDIF_ARRAY-1; i++)
	{
		printf("%2.2f, ", GetPDIFCrvPts(i).xVal);
	}
	printf("%3.2f }\n", GetPDIFCrvPts(SIZE_OF_PDIF_ARRAY-1).xVal); //Print the last element in the array with the closing bracket and new line

	/* Print PDIF DATA yVal array */
	/* printf("PDIF:\t yVal = { ");

	/* for(i=0; i <SIZE_OF_PDIF_ARRAY-1; i++)
	/* {
		printf("%3.2f, ", GetPDIFCrvPts(i).yVal);
	/* }
	/* printf("%2.2f }\n", GetPDIFCrvPts(SIZE_OF_PDIF_ARRAY-1).yVal); //Print the last element in the array with the closing bracket and new line

	/* printf("\n"); */ //new line for formating

#endif
}

/******************************************************************************
*       PrintDataView Function Definition
******************************************************************************/
/*!  \brief         This function prints the Data View to the screen
 *
 *   \ingroup       View Function
 *
 *   \param[in]     Var     DESC
 *
 *   \return        N/A
 ******************************************************************************/
void PrintFullDataView()
{
	//Integer8 i = 0; //Iterator Variable for the for loops

	printf("=================== IEC61850 Data ==========================================\n");

	/* printf("CSWI:\t CtlVal: %s\t StVal: %s\n",BooleanToString(GetCSWICtlVal()), DBPosToString(GetCSWIStVal()));    //Print CSWI Data
	 * printf("GGIO:\t CtlVal: %s\t StVal: %s\n",BooleanToString(GetGGIOCtlVal()), BooleanToString(GetGGIOCtlVal())); //Print GGIO Data
	 * printf("\n"); //new line for formating
	 */


#ifdef __PARADIGM__

	//Print MMXU Data
	printf("MMXU:\t PhsA$Mag$i: %ld\t ", GetMMXUPhsAMagi());  //Integer32 on beck is a long int so %ld is needed
    printf("PhsB$Mag$i: %ld\t ", GetMMXUPhsBMagi());
    printf("PhsC$Mag$i: %ld\n",GetMMXUPhsCMagi());
	printf("\n"); //new line for formating

	/* We do not support Floating point on BECK devices
	 * Therefore we only add the CrvPts array if this
	 * is not being compiled for BECK devices
	 * */
#else

	//Print MMXU Data and GGIO Data
	printf("PhsV$PhsA$Mag$f: %.2f\t ", GetMMXUPhsAMagi());
	printf("PhsV$PhsB$Mag$f: %.2f\t ", GetMMXUPhsBMagi());
	printf("PhsV$PhsC$Mag$f: %.2f\n",GetMMXUPhsCMagi());
	printf("A$PhsA$Mag$f: %.2f\t ", GetMMXUAAMagf());
	printf("A$PhsB$Mag$f: %.2f\t ", GetMMXUABMagf());
	printf("A$PhsC$Mag$f: %.2f\n", GetMMXUACMagf());
    printf("GGIO:\t CtlVal: %s\t StVal: %s\n",BooleanToString(GetGGIOCtlVal()), BooleanToString(GetGGIOStVal())); //Print GGIO Data

	//Print MMXU2 Data and GGIO2 Data
	printf("PhsV$PhsA$Mag$f: %.2f\t ", GetMMXU2PhsAMagi());
	printf("PhsV$PhsB$Mag$f: %.2f\t ", GetMMXU2PhsBMagi());
	printf("PhsV$PhsC$Mag$f: %.2f\n",GetMMXU2PhsCMagi());
	printf("A$PhsA$Mag$f: %.2f\t ", GetMMXU2AAMagf());
	printf("A$PhsB$Mag$f: %.2f\t ", GetMMXU2ABMagf());
	printf("A$PhsC$Mag$f: %.2f\n", GetMMXU2ACMagf());
    printf("GGIO:\t CtlVal: %s\t StVal: %s\n",BooleanToString(GetGGIO2CtlVal()), BooleanToString(GetGGIO2StVal())); //Print GGIO Data
	printf("\n"); //new line for formating

	/* Print PDIF DATA xVal array */
	/* printf("PDIF:\t xVal = { ");

	for(i=0; i <SIZE_OF_PDIF_ARRAY-1; i++)
	{
		printf("%2.2f, ", GetPDIFCrvPts(i).xVal);
	}
	printf("%3.2f }\n", GetPDIFCrvPts(SIZE_OF_PDIF_ARRAY-1).xVal); //Print the last element in the array with the closing bracket and new line

	/* Print PDIF DATA yVal array */
	/* printf("PDIF:\t yVal = { ");

	/* for(i=0; i <SIZE_OF_PDIF_ARRAY-1; i++)
	/* {
		printf("%3.2f, ", GetPDIFCrvPts(i).yVal);
	/* }
	/* printf("%2.2f }\n", GetPDIFCrvPts(SIZE_OF_PDIF_ARRAY-1).yVal); //Print the last element in the array with the closing bracket and new line

	/* printf("\n"); */ //new line for formating

#endif
}

/******************************************************************************
*       PrintServerSubscriptionDataView Function Definition
******************************************************************************/
/*!  \brief         This function prints the Data View for the server subscription to the screen, this is cut down from the main view for clarity
 *
 *   \ingroup       View Function
 *
 *   \param[in]     Var     DESC
 *
 *   \return        N/A
 ******************************************************************************/
void PrintServerSubscriptionDataView()
{
	printf("=================== IEC61850 Data ==========================================\n");

	printf("CSWI:\t StVal: %s\n", DBPosToString(GetCSWIStVal()));    //Print CSWI Data
	printf("GGIO:\t StVal: %s\n", BooleanToString(GetGGIOStVal())); //Print GGIO Data
	printf("\n"); //new line for formating
}

/******************************************************************************
*       PrintErrorSrtring Function Definition
******************************************************************************/
/*!  \brief         This function prints the Menu View to the screen
 *
 *   \ingroup       View Function
 *
 *   \param[in]     Var     DESC
 *
 *   \return        N/A
 ******************************************************************************/
void PrintErrorString()
{
	printf("=================== Last Error =============================================\n");
	if(GetErrorString()[0] == 0)
	{
		printf("NONE\n");
	}
	else
	{
		printf("%s\n", GetErrorString());
	}

}


/******************************************************************************
*       PrintServerMenuView Function Definition
******************************************************************************/
/*!  \brief         This function prints the Server Menu View to the screen
 *
 *   \ingroup       View Function
 *
 *   \param[in]     Var     DESC
 *
 *   \return        N/A
 ******************************************************************************/
void PrintServerMenuView()
{
	printf("=================== IEC61850 Command Menu ==================================\n");
	printf("1) Update PhsV$A Mag f \t DAID: {3,0,0,0,0}\n");
	printf("2) Update PhsV$B Mag f \t DAID: {4,0,0,0,0}\n");
	printf("3) Update PhsV$C Mag f \t DAID: {5,0,0,0,0}\n");
    printf("4) Update A$A Mag f \t DAID {6,0,0,0,0}\n");
    printf("5) Update A$B Mag f \t DAID {7,0,0,0,0}\n");
    printf("6) Update A$C Mag f \t DAID {8,0,0,0,0}\n");
	/* We do not support Floating point on BECK devices
	 * Therefore we only add the CrvPts array if this
	 * is not being compiled for BECK devices
	 * */
/* #ifndef __PARADIGM__
 *	printf("4) Update crvPts Xval \t DAID: {6,0,0,0,0} \n");
 *	printf("5) Update crvPts Yval\n");
* #endif */

	printf("X) To Exit\n");
	printf("============================================================================\n");

}

/******************************************************************************
*       PrintClientMenuView Function Definition
******************************************************************************/
/*!  \brief         This function prints the Client Menu View to the screen
 *
 *   \ingroup       View Function
 *
 *   \param[in]     Var     DESC
 *
 *   \return        N/A
 ******************************************************************************/
void PrintClientMenuView()
{
	printf("=================== IEC61850 Command Menu ==================================\n");
	printf("1) Select & Operate CSWI  DAID: {1,1,0,0,0}\n");
	printf("2) Operate GGIO \t  DAID: {2,1,0,0,0}\n");
	printf("3) Get Connected Server List\n");
	printf("4) MMS Get File Attributes\n");
	printf("5) MMS Get File\n");
	printf("6) MMS Delete File\n");
	printf("X) To Exit\n");
	printf("============================================================================\n");

}


/******************************************************************************
*       PrintServerSubscriptionMenuView Function Definition
******************************************************************************/
/*!  \brief         This function prints the Server Subscription Menu View to the screen
 *
 *   \ingroup       View Function
 *
 *   \param[in]     Var     DESC
 *
 *   \return        N/A
 ******************************************************************************/
void PrintServerSubscriptionMenuView()
{
	printf("=================== IEC61850 Command Menu ==================================\n");
	printf("X) To Exit\n");
	printf("============================================================================\n");

}

/******************************************************************************
*       BooleanToString Function Definition
******************************************************************************/
/*!  \brief         This function Returns a Cstring representation of a boolean (True or False)
 *
 *   \ingroup       View Function
 *
 *   \param[in]     Var     DESC
 *
 *   \return        False if inBool is 0,
 *   \return        otherwise True
 ******************************************************************************/
const char* BooleanToString(Boolean inBool)
{
	const char * returnString;
	static const char * const boolStrings[] ={"True", "False" };

	if(inBool == FALSE)
	{
		returnString = boolStrings[1]; //Set returnString to False
	}
	else
	{
		returnString = boolStrings[0]; //Set returnString to True
	}

	return returnString;
}


/******************************************************************************
*       DBPosToString Function Definition
******************************************************************************/
/*!  \brief         This function Returns a Cstring representation of an enum DbPosValues
 *
 *   \ingroup       View Function
 *
 *   \param[in]     inDBPosVal     An enum DbPosValues that will be used to generate the string value
 *
 *   \return        "INTERMEDIATE" if inDBPosVal is DBPOS_INTERMEDIATE
 *   \return        "OFF" if inDBPosVal is DBPOS_OFF,
 *   \return        "ON" if inDBPosVal is DBPOS_ON,
 *   \return        "BAD" if inDBPosVal is DBPOS_BAD,
 *   \return        otherwise NULL pointer
 ******************************************************************************/
const char* DBPosToString(enum DbPosValues inDBPosVal)
{
	const char * returnString;
	static const char * const dbPosStrings[] ={"INTERMEDIATE", "OFF", "ON", "BAD"};

	switch(inDBPosVal)
	{
	case DBPOS_INTERMEDIATE:
		returnString = dbPosStrings[0];
		break;
	case DBPOS_OFF:
		returnString = dbPosStrings[1];
		break;
	case DBPOS_ON:
		returnString = dbPosStrings[2];
		break;
	case DBPOS_BAD:
		returnString = dbPosStrings[3];
		break;
	default:
		returnString = NULL;
		break;
	}

	return returnString;
}


/******************************************************************************
*       ClearScreen Function Definition
******************************************************************************/
/*!  \brief         This function Clears the screen
 *
 *   \ingroup       View Function
 *
 *   \param[in]     N/A
 *
 *   \return        N/A
 ******************************************************************************/
void ClearScreen()
{
	/* There is no CLS or clear command on beck RTOS so only execute if this is not a BECK Device*/

#ifndef __PARADIGM__
	//Simple way to clear the screen
	//if (system("CLS")) system("clear");
	system("clear");
#else
	printf("\n\n\n\n\n\n\n\n\n\n"); //on Beck just push the old screen up by 10 lines
#endif

#ifndef WIN32
   printf("\033[2J\033[1;1H");  //Clear Screen ANSI (not supported on windows)
#endif


}
