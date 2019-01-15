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
/*!	\file		LocalData.c
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <sqlite3.h>

#include "LocalData.h"

	/* CSWI */
	Boolean CSWICtlVal = FALSE;
	enum DbPosValues CSWIStVal = DBPOS_OFF;

	/* GGIO */
	Boolean GGIOCtlVal = FALSE;
	Boolean GGIOStVal = FALSE;
    Boolean GGIO2CtlVal = FALSE;
	Boolean GGIO2StVal = FALSE;

	/* MMXU */
	Float32 MMXUPhsAMagi = 0.0;
	Float32 MMXUPhsBMagi = 0.0;
	Float32 MMXUPhsCMagi = 0.0;
	Float32 MMXUAAMagf = 0.0;
	Float32 MMXUABMagf = 0.0;
	Float32 MMXUACMagf = 0.0;
    Float32 MMXU2PhsAMagi = 0.0;
	Float32 MMXU2PhsBMagi = 0.0;
	Float32 MMXU2PhsCMagi = 0.0;
	Float32 MMXU2AAMagf = 0.0;
	Float32 MMXU2ABMagf = 0.0;
	Float32 MMXU2ACMagf = 0.0;

	/* PDIF */
	CrvPts PDIFCrvPts[SIZE_OF_PDIF_ARRAY] ={{0}};

	/*Error String */
	char strError[SIZE_OF_ERROR_STRING] = {0};


/******************************************************************************
*       GetCSWICtlVal Function Definition
******************************************************************************/
/*!  \brief         This function gets the current value of the CSWICtlVal
 *
 *   \ingroup       View Function
 *
 *   \param[in]     Var     DESC
 *
 *   \return        The current value of the CSWICtlVal
 ******************************************************************************/
Boolean GetCSWICtlVal()
{
	return CSWICtlVal;
}

/******************************************************************************
*       SetCSWICtlVal Function Definition
******************************************************************************/
/*!  \brief         This function sets the value of the CSWICtlVal
 *
 *   \ingroup       Mutator
 *
 *   \param[in]     inCtlVal     a Boolean that will be used the value of the CSWICtlVal
 *
 *   \return        The current value of the CSWICtlVal
 ******************************************************************************/
Boolean SetCSWICtlVal(Boolean inCtlVal)
{
	//Validate input and ensure the value is always either True or False
	if(inCtlVal == FALSE)
	{
		CSWICtlVal = FALSE;
	}
	else
	{
		CSWICtlVal = TRUE;
	}

	return CSWICtlVal;
}

/******************************************************************************
*       GetCSWIStlVal Function Definition
******************************************************************************/
/*!  \brief         This function gets the current value of the CSWIStlVal
 *
 *   \ingroup       View Function
 *
 *   \param[in]     Var     DESC
 *
 *   \return        The current value of the CSWIStlVal
 ******************************************************************************/
enum DbPosValues GetCSWIStVal()
{
	return CSWIStVal;
}

/******************************************************************************
*       SetCSWIStlVal Function Definition
******************************************************************************/
/*!  \brief         This function sets the value of the CSWIStlVal
 *
 *   \ingroup       Mutator
 *
 *   \param[in]     inStVal     an enum DbPosValues that will be used the value of the CSWIStlVal
 *
 *   \return        The current value of the CSWIStlVal
 ******************************************************************************/
enum DbPosValues SetCSWIStVal(enum DbPosValues inStVal)
{
	//Validate input to make sure it is a valid value for a DBPos
	switch(inStVal)
	{
		case DBPOS_INTERMEDIATE:
			CSWIStVal = DBPOS_INTERMEDIATE;
			break;
		case DBPOS_OFF:
			CSWIStVal = DBPOS_OFF;
			break;
		case DBPOS_ON:
			CSWIStVal = DBPOS_ON;
			break;
		case DBPOS_BAD:
			CSWIStVal = DBPOS_BAD;
			break;
	}

	return CSWIStVal;
}




/******************************************************************************
*       GetGGIOCtlVal Function Definition
******************************************************************************/
/*!  \brief         This function gets the current value of the GGIOCtlVal
 *
 *   \ingroup       View Function
 *
 *   \param[in]     Var     DESC
 *
 *   \return        The current value of the GGIOCtlVal
 ******************************************************************************/
Boolean GetGGIOCtlVal()
{
	return GGIOCtlVal;
}

/******************************************************************************
*       SetGGIOCtlVal Function Definition
******************************************************************************/
/*!  \brief         This function sets the value of the GGIOCtlVal
 *
 *   \ingroup       Mutator
 *
 *   \param[in]     inCtlVal     a Boolean that will be used the value of the GGIOCtlVal
 *
 *   \return        The current value of the GGIOCtlVal
 ******************************************************************************/
Boolean SetGGIOCtlVal(Boolean inCtlVal)
{
	//Validate input and ensure the value is always either True or False
	if(inCtlVal == FALSE)
	{
		GGIOCtlVal = FALSE;
	}
	else
	{
		GGIOCtlVal = TRUE;
	}

	return GGIOCtlVal;
}

/******************************************************************************
*       GetGGIO2CtlVal Function Definition
******************************************************************************/
/*!  \brief         This function gets the current value of the GGIOCtlVal
 *
 *   \ingroup       View Function
 *
 *   \param[in]     Var     DESC
 *
 *   \return        The current value of the GGIOCtlVal
 ******************************************************************************/
Boolean GetGGIO2CtlVal()
{
	return GGIO2CtlVal;
}

/******************************************************************************
*       SetGGIO2CtlVal Function Definition
******************************************************************************/
/*!  \brief         This function sets the value of the GGIOCtlVal
 *
 *   \ingroup       Mutator
 *
 *   \param[in]     inCtlVal     a Boolean that will be used the value of the GGIOCtlVal
 *
 *   \return        The current value of the GGIOCtlVal
 ******************************************************************************/
Boolean SetGGIO2CtlVal(Boolean inCtlVal)
{
	//Validate input and ensure the value is always either True or False
	if(inCtlVal == FALSE)
	{
		GGIO2CtlVal = FALSE;
	}
	else
	{
		GGIO2CtlVal = TRUE;
	}

	return GGIO2CtlVal;
}

/******************************************************************************
*       GetGGIOStVal Function Definition
******************************************************************************/
/*!  \brief         This function gets the current value of the GGIOStVal
 *
 *   \ingroup       Accessor
 *
 *   \param[in]     Var     DESC
 *
 *   \return        The current value of the GGIOCtlVal
 ******************************************************************************/
Boolean GetGGIOStVal()
{
	return GGIOStVal;
}

/******************************************************************************
*       SetGGIOStVal Function Definition
******************************************************************************/
/*!  \brief         This function sets the value of the GGIOCtlVal
 *
 *   \ingroup       Mutator
 *
 *   \param[in]     inStVal     a Boolean that will be used the value of the GGIOCtlVal
 *
 *   \return        The current value of the GGIOCtlVal
 ******************************************************************************/
Boolean SetGGIOStVal(Boolean inStVal)
{
	//Validate input and ensure the value is always either True or False
	if(inStVal == FALSE)
	{
		GGIOStVal = FALSE;
	}
	else
	{
		GGIOStVal = TRUE;
	}

	return GGIOStVal;
}

/******************************************************************************
*       GetGGIO2StVal Function Definition
******************************************************************************/
/*!  \brief         This function gets the current value of the GGIOStVal
 *
 *   \ingroup       Accessor
 *
 *   \param[in]     Var     DESC
 *
 *   \return        The current value of the GGIOCtlVal
 ******************************************************************************/
Boolean GetGGIO2StVal()
{
	return GGIO2StVal;
}

/******************************************************************************
*       SetGGIOStVal Function Definition
******************************************************************************/
/*!  \brief         This function sets the value of the GGIOCtlVal
 *
 *   \ingroup       Mutator
 *
 *   \param[in]     inStVal     a Boolean that will be used the value of the GGIOCtlVal
 *
 *   \return        The current value of the GGIOCtlVal
 ******************************************************************************/
Boolean SetGGIO2StVal(Boolean inStVal)
{
	//Validate input and ensure the value is always either True or False
	if(inStVal == FALSE)
	{
		GGIO2StVal = FALSE;
	}
	else
	{
		GGIO2StVal = TRUE;
	}

	return GGIO2StVal;
}

/******************************************************************************
*       GetMMXUPhsAMagi Function Definition
******************************************************************************/
/*!  \brief         This function gets the current value of the MMXUPhsAMagi
 *
 *   \ingroup       Accessor
 *
 *   \param[in]     Var     DESC
 *
 *   \return        The current value of the MMXUPhsAMagi
 ******************************************************************************/
Float32 GetMMXUPhsAMagi()
{
	return MMXUPhsAMagi;
}

/******************************************************************************
*       SetMMXUPhsAMagi Function Definition
******************************************************************************/
/*!  \brief         This function sets the value of the MMXUPhsAMagi
 *
 *   \ingroup       Mutator
 *
 *   \param[in]     inPhsMagi     an Integer32 that will be used the value of the MMXUPhsAMagi
 *
 *   \return        The current value of the MMXUPhsAMagi
 ******************************************************************************/
Float32 SetMMXUPhsAMagi(Float32 inPhsMagi)
{
	MMXUPhsAMagi = inPhsMagi;

	/* sqlError = sqlite3_open("db_test.db", &conn);
    if (sqlError)
    {
        printf("Can't open database for f1!");
    }

	sprintf(sql_lite, "INSERT INTO opal (f1) values ('%f');",MMXUPhsAMagi);
	sqlError = sqlite3_exec(conn, sql_lite, 0, 0, 0);

	if (sqlError != SQLITE_OK)
    {
        printf("We did not get any data for f1!");
        exit(0);
    }

    sqlite3_close(conn); */

	return MMXUPhsAMagi;
}


/******************************************************************************
*       GetMMXU2PhsAMagi Function Definition
******************************************************************************/
/*!  \brief         This function gets the current value of the MMXUPhsAMagi
 *
 *   \ingroup       Accessor
 *
 *   \param[in]     Var     DESC
 *
 *   \return        The current value of the MMXUPhsAMagi
 ******************************************************************************/
Float32 GetMMXU2PhsAMagi()
{
	return MMXU2PhsAMagi;
}

/******************************************************************************
*       SetMMXU2PhsAMagi Function Definition
******************************************************************************/
/*!  \brief         This function sets the value of the MMXUPhsAMagi
 *
 *   \ingroup       Mutator
 *
 *   \param[in]     inPhsMagi     an Integer32 that will be used the value of the MMXUPhsAMagi
 *
 *   \return        The current value of the MMXUPhsAMagi
 ******************************************************************************/
Float32 SetMMXU2PhsAMagi(Float32 inPhsMagi)
{
	MMXU2PhsAMagi = inPhsMagi;

	/* sqlError = sqlite3_open("db_test.db", &conn);
    if (sqlError)
    {
        printf("Can't open database for f1!");
    }

	sprintf(sql_lite, "INSERT INTO opal (f1) values ('%f');",MMXUPhsAMagi);
	sqlError = sqlite3_exec(conn, sql_lite, 0, 0, 0);

	if (sqlError != SQLITE_OK)
    {
        printf("We did not get any data for f1!");
        exit(0);
    }

    sqlite3_close(conn); */

	return MMXU2PhsAMagi;
}

/******************************************************************************
*       GetMMXUPhsBMagi Function Definition
******************************************************************************/
/*!  \brief         This function gets the current value of the MMXUPhsBMagi
 *
 *   \ingroup       Accessor
 *
 *   \param[in]     Var     DESC
 *
 *   \return        The current value of the MMXUPhsBMagi
 ******************************************************************************/
Float32 GetMMXUPhsBMagi()
{
	return MMXUPhsBMagi;
}

/******************************************************************************
*       SetMMXUPhsAMagi Function Definition
******************************************************************************/
/*!  \brief         This function sets the value of the MMXUPhsBMagi
 *
 *   \ingroup       Mutator
 *
 *   \param[in]     inPhsMagi     an Integer32 that will be used the value of the MMXUPhsBMagi
 *
 *   \return        The current value of the MMXUPhsBMagi
 ******************************************************************************/
Float32 SetMMXUPhsBMagi(Float32 inPhsMagi)
{
	MMXUPhsBMagi = inPhsMagi;

	/* sqlError = sqlite3_open("db_test.db", &conn);
    if (sqlError)
    {
        printf("Can't open database for f2!");
    }

	sprintf(sql_lite, "INSERT INTO opal (f2) values ('%f');",MMXUPhsBMagi);
	sqlError = sqlite3_exec(conn, sql_lite, 0, 0, 0);

	if (sqlError != SQLITE_OK)
    {
        printf("We did not get any data for f1!");
        exit(0);
    }

    sqlite3_close(conn); */

	return MMXUPhsBMagi;
}

/******************************************************************************
*       GetMMXU2PhsBMagi Function Definition
******************************************************************************/
/*!  \brief         This function gets the current value of the MMXUPhsBMagi
 *
 *   \ingroup       Accessor
 *
 *   \param[in]     Var     DESC
 *
 *   \return        The current value of the MMXUPhsBMagi
 ******************************************************************************/
Float32 GetMMXU2PhsBMagi()
{
	return MMXU2PhsBMagi;
}

/******************************************************************************
*       SetMMXUPhsAMagi Function Definition
******************************************************************************/
/*!  \brief         This function sets the value of the MMXUPhsBMagi
 *
 *   \ingroup       Mutator
 *
 *   \param[in]     inPhsMagi     an Integer32 that will be used the value of the MMXUPhsBMagi
 *
 *   \return        The current value of the MMXUPhsBMagi
 ******************************************************************************/
Float32 SetMMXU2PhsBMagi(Float32 inPhsMagi)
{
	MMXU2PhsBMagi = inPhsMagi;

	/* sqlError = sqlite3_open("db_test.db", &conn);
    if (sqlError)
    {
        printf("Can't open database for f2!");
    }

	sprintf(sql_lite, "INSERT INTO opal (f2) values ('%f');",MMXUPhsBMagi);
	sqlError = sqlite3_exec(conn, sql_lite, 0, 0, 0);

	if (sqlError != SQLITE_OK)
    {
        printf("We did not get any data for f1!");
        exit(0);
    }

    sqlite3_close(conn); */

	return MMXU2PhsBMagi;
}

/******************************************************************************
*       GetMMXUPhsCMagi Function Definition
******************************************************************************/
/*!  \brief         This function gets the current value of the MMXUPhsCMagi
 *
 *   \ingroup       Accessor
 *
 *   \param[in]     Var     DESC
 *
 *   \return        The current value of the MMXUPhsCMagi
 ******************************************************************************/
Float32 GetMMXUPhsCMagi()
{
	return MMXUPhsCMagi;
}

/******************************************************************************
*       SetMMXUPhsAMagi Function Definition
******************************************************************************/
/*!  \brief         This function sets the value of the MMXUPhsCMagi
 *
 *   \ingroup       Mutator
 *
 *   \param[in]     inPhsMagi     an Integer32 that the value of the MMXUPhsCMagi will be set to
 *
 *   \return        The current value of the MMXUPhsCMagi
 ******************************************************************************/
Float32 SetMMXUPhsCMagi(Float32 inPhsMagi)
{
	MMXUPhsCMagi = inPhsMagi;

	return MMXUPhsCMagi;
}

/******************************************************************************
*       GetMMXU2PhsCMagi Function Definition
******************************************************************************/
/*!  \brief         This function gets the current value of the MMXUPhsCMagi
 *
 *   \ingroup       Accessor
 *
 *   \param[in]     Var     DESC
 *
 *   \return        The current value of the MMXUPhsCMagi
 ******************************************************************************/
Float32 GetMMXU2PhsCMagi()
{
	return MMXU2PhsCMagi;
}

/******************************************************************************
*       SetMMXU2PhsAMagi Function Definition
******************************************************************************/
/*!  \brief         This function sets the value of the MMXUPhsCMagi
 *
 *   \ingroup       Mutator
 *
 *   \param[in]     inPhsMagi     an Integer32 that the value of the MMXUPhsCMagi will be set to
 *
 *   \return        The current value of the MMXUPhsCMagi
 ******************************************************************************/
Float32 SetMMXU2PhsCMagi(Float32 inPhsMagi)
{
	MMXU2PhsCMagi = inPhsMagi;

	return MMXU2PhsCMagi;
}

/******************************************************************************
*       GetMMXUAAMagf Function Definition
******************************************************************************/
/*!  \brief         This function gets the current value of the MMXUPhsCMagi
 *
 *   \ingroup       Accessor
 *
 *   \param[in]     Var     DESC
 *
 *   \return        The current value of the MMXUPhsCMagi
 ******************************************************************************/
Float32 GetMMXUAAMagf()
{
	return MMXUAAMagf;
}

/******************************************************************************
*       SetMMXUAAMagf Function Definition
******************************************************************************/
/*!  \brief         This function sets the value of the MMXUPhsCMagi
 *
 *   \ingroup       Mutator
 *
 *   \param[in]     inPhsMagi     an Integer32 that the value of the MMXUPhsCMagi will be set to
 *
 *   \return        The current value of the MMXUPhsCMagi
 ******************************************************************************/
Float32 SetMMXUAAMagf(Float32 inMagf)
{
	MMXUAAMagf = inMagf;

	return MMXUAAMagf;
}

/******************************************************************************
*       GetMMXU2AAMagf Function Definition
******************************************************************************/
/*!  \brief         This function gets the current value of the MMXUPhsCMagi
 *
 *   \ingroup       Accessor
 *
 *   \param[in]     Var     DESC
 *
 *   \return        The current value of the MMXUPhsCMagi
 ******************************************************************************/
Float32 GetMMXU2AAMagf()
{
	return MMXU2AAMagf;
}

/******************************************************************************
*       SetMMXU2AAMagf Function Definition
******************************************************************************/
/*!  \brief         This function sets the value of the MMXUPhsCMagi
 *
 *   \ingroup       Mutator
 *
 *   \param[in]     inPhsMagi     an Integer32 that the value of the MMXUPhsCMagi will be set to
 *
 *   \return        The current value of the MMXUPhsCMagi
 ******************************************************************************/
Float32 SetMMXU2AAMagf(Float32 inMagf)
{
	MMXU2AAMagf = inMagf;

	return MMXU2AAMagf;
}

/******************************************************************************
*       GetMMXUABMagf Function Definition
******************************************************************************/
/*!  \brief         This function gets the current value of the MMXUPhsCMagi
 *
 *   \ingroup       Accessor
 *
 *   \param[in]     Var     DESC
 *
 *   \return        The current value of the MMXUPhsCMagi
 ******************************************************************************/
Float32 GetMMXUABMagf()
{
	return MMXUABMagf;
}

/******************************************************************************
*       SetMMXUABMagf Function Definition
******************************************************************************/
/*!  \brief         This function sets the value of the MMXUPhsCMagi
 *
 *   \ingroup       Mutator
 *
 *   \param[in]     inPhsMagi     an Integer32 that the value of the MMXUPhsCMagi will be set to
 *
 *   \return        The current value of the MMXUPhsCMagi
 ******************************************************************************/
Float32 SetMMXUABMagf(Float32 inMagf)
{
	MMXUABMagf = inMagf;

	return MMXUABMagf;
}


/******************************************************************************
*       GetMMXU2ABMagf Function Definition
******************************************************************************/
/*!  \brief         This function gets the current value of the MMXUPhsCMagi
 *
 *   \ingroup       Accessor
 *
 *   \param[in]     Var     DESC
 *
 *   \return        The current value of the MMXUPhsCMagi
 ******************************************************************************/
Float32 GetMMXU2ABMagf()
{
	return MMXU2ABMagf;
}

/******************************************************************************
*       SetMMXU2ABMagf Function Definition
******************************************************************************/
/*!  \brief         This function sets the value of the MMXUPhsCMagi
 *
 *   \ingroup       Mutator
 *
 *   \param[in]     inPhsMagi     an Integer32 that the value of the MMXUPhsCMagi will be set to
 *
 *   \return        The current value of the MMXUPhsCMagi
 ******************************************************************************/
Float32 SetMMXU2ABMagf(Float32 inMagf)
{
	MMXU2ABMagf = inMagf;

	return MMXU2ABMagf;
}

/******************************************************************************
*       GetMMXUACMagf Function Definition
******************************************************************************/
/*!  \brief         This function gets the current value of the MMXUPhsCMagi
 *
 *   \ingroup       Accessor
 *
 *   \param[in]     Var     DESC
 *
 *   \return        The current value of the MMXUPhsCMagi
 ******************************************************************************/
Float32 GetMMXUACMagf()
{
	return MMXUACMagf;
}

/******************************************************************************
*       SetMMXUAAMagf Function Definition
******************************************************************************/
/*!  \brief         This function sets the value of the MMXUPhsCMagi
 *
 *   \ingroup       Mutator
 *
 *   \param[in]     inPhsMagi     an Integer32 that the value of the MMXUPhsCMagi will be set to
 *
 *   \return        The current value of the MMXUPhsCMagi
 ******************************************************************************/
Float32 SetMMXUACMagf(Float32 inMagf)
{
	MMXUACMagf = inMagf;

	return MMXUACMagf;
}


/******************************************************************************
*       GetMMXU2ACMagf Function Definition
******************************************************************************/
/*!  \brief         This function gets the current value of the MMXUPhsCMagi
 *
 *   \ingroup       Accessor
 *
 *   \param[in]     Var     DESC
 *
 *   \return        The current value of the MMXUPhsCMagi
 ******************************************************************************/
Float32 GetMMXU2ACMagf()
{
	return MMXU2ACMagf;
}

/******************************************************************************
*       SetMMXU2ACMagf Function Definition
******************************************************************************/
/*!  \brief         This function sets the value of the MMXUPhsCMagi
 *
 *   \ingroup       Mutator
 *
 *   \param[in]     inPhsMagi     an Integer32 that the value of the MMXUPhsCMagi will be set to
 *
 *   \return        The current value of the MMXUPhsCMagi
 ******************************************************************************/
Float32 SetMMXU2ACMagf(Float32 inMagf)
{
	MMXU2ACMagf = inMagf;

	return MMXU2ACMagf;
}

/******************************************************************************
*       GetPDIFCrvPts Function Definition
******************************************************************************/
/*!  \brief         This function gets the current value of the PDIFCrvPts at the specified index
 *
 *   \ingroup       Accessor
 *
 *   \param[in]     inIndex     An Integer8 that holds the index to get, between 0 and SIZE_OF_PDIF_ARRAY-1
 *
 *   \return        The current value of the PDIFCrvPts at the specified index
 ******************************************************************************/
CrvPts GetPDIFCrvPts(Integer8 inIndex)
{
	CrvPts returnVal = {0};

	//Make sure the index is not out of range
	if((inIndex >= 0) && (inIndex < SIZE_OF_PDIF_ARRAY))
	{
		returnVal.xVal = PDIFCrvPts[inIndex].xVal;
		returnVal.yVal = PDIFCrvPts[inIndex].yVal;
	}

	return returnVal;
}


/******************************************************************************
*       SetPDIFCrvPts Function Definition
******************************************************************************/
/*!  \brief         This function gets the current value of the PDIFCrvPtsYVal at the specified index
 *
 *   \ingroup       Mutator
 *
 *   \param[in]     inIndex     An Integer8 that holds the index to get, between 0 and SIZE_OF_PDIF_ARRAY-1
 *   \param[in]     inCrvPts    A crvPts structure that holds the new Xval and Yval for the PDIFCrvPts
 *
 *   \return        The current value of the PDIFCrvPts
 ******************************************************************************/
CrvPts SetPDIFCrvPts(Integer8 inIndex, CrvPts inCrvPts)
{
	CrvPts returnVal = {0};

	//Make sure the index is not out of range
	if((inIndex >= 0) && (inIndex < SIZE_OF_PDIF_ARRAY))
	{
		PDIFCrvPts[inIndex].xVal = inCrvPts.xVal;
		PDIFCrvPts[inIndex].yVal = inCrvPts.yVal;

		returnVal.xVal = PDIFCrvPts[inIndex].xVal;
		returnVal.yVal = PDIFCrvPts[inIndex].yVal;
	}

	return returnVal;
}



/******************************************************************************
*       GetErrorString Function Definition
******************************************************************************/
/*!  \brief         This function gets the current Error String
 *
 *   \ingroup       Accessor
 *
 *   \param[in]     Var     DESC
 *
 *   \return        The current value of the Error String
 ******************************************************************************/
char* GetErrorString()
{
	return strError;
}

/******************************************************************************
*       SetErrorString Function Definition
******************************************************************************/
/*!  \brief         This function sets the value of the MMXUPhsCMagi
 *
 *   \ingroup       Mutator
 *
 *   \param[in]     inErrorString     A C string that contains the new error string
 *   \param[in]     inLength		  An Unsigned8 that holds the length of the new error string, must be less than SIZE_OF_ERROR_STRING
 *
 *   \return        The current value of the MMXUPhsCMagi
 ******************************************************************************/
char* SetErrorString(char* inErrorString, Unsigned8 inLength)
{
	char * returnVal = NULL;

	/* Validate inputs */
	if((inErrorString != NULL) && ((inLength <= SIZE_OF_ERROR_STRING) && (inLength >= 0)))
	{
		memset(strError, 0, SIZE_OF_ERROR_STRING); //Set the string to 0

		memcpy(strError, inErrorString, inLength); //Copy in the new Error String
	}

	return returnVal;
}



