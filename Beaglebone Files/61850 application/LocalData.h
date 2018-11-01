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
/*!	\file		LocalData.h
 *	\brief 		Header file for the PIS10 Stack Example
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

/******************************************************************************
*	Defines
******************************************************************************/

#ifndef LOCAL_DATA_INCLUDED
#define LOCAL_DATA_INCLUDED 1

#define SIZE_OF_PDIF_ARRAY 6
#define SIZE_OF_ERROR_STRING 255

/* Needed to make snprintf work in Visual Studio */
#if _MSC_VER
   #define snprintf _snprintf
#endif

/******************************************************************************
*	Includes
******************************************************************************/

#include <stdio.h>
#include "IEC61850Types.h"
#include "sysctype.h"

#include "ExampleTypes.h"

/******************************************************************************
*	Prototypes
******************************************************************************/
/* CSWI */
Boolean GetCSWICtlVal();
Boolean SetCSWICtlVal(Boolean inCtlVal);
enum DbPosValues GetCSWIStVal();
enum DbPosValues SetCSWIStVal(enum DbPosValues inStVal);

/* GGIO */
Boolean GetGGIOCtlVal();
Boolean SetGGIOCtlVal(Boolean inCtlVal);
Boolean GetGGIOStVal();
Boolean SetGGIOStVal(Boolean inStVal);

/* MMXU */
Float32 GetMMXUPhsAMagi();
Float32 SetMMXUPhsAMagi(Float32 inPhsMagi);
Float32 GetMMXUPhsBMagi();
Float32 SetMMXUPhsBMagi(Float32 inPhsMagi);
Float32 GetMMXUPhsCMagi();
Float32 SetMMXUPhsCMagi(Float32 inPhsMagi);
Float32 GetMMXUAAMagf();
Float32 SetMMXUAAMagf(Float32 inMagf);
Float32 GetMMXUABMagf();
Float32 SetMMXUABMagf(Float32 inMagf);
Float32 GetMMXUACMagf();
Float32 SetMMXU2ACMagf(Float32 inMagf);
Float32 GetMMXU2PhsAMagi();
Float32 SetMMXU2PhsAMagi(Float32 inPhsMagi);
Float32 GetMMXU2PhsBMagi();
Float32 SetMMXU2PhsBMagi(Float32 inPhsMagi);
Float32 GetMMXU2PhsCMagi();
Float32 SetMMXU2PhsCMagi(Float32 inPhsMagi);
Float32 GetMMXU2AAMagf();
Float32 SetMMXU2AAMagf(Float32 inMagf);
Float32 GetMMXU2ABMagf();
Float32 SetMMXU2ABMagf(Float32 inMagf);
Float32 GetMMXU2ACMagf();
Float32 SetMMXU2ACMagf(Float32 inMagf);

/* PDIF */
CrvPts GetPDIFCrvPts(Integer8 inIndex);
CrvPts SetPDIFCrvPts(Integer8 inIndex, CrvPts inCrvPts);

/* ErrorString*/
char* GetErrorString();
char* SetErrorString(char* inErrorString, Unsigned8 inLength);

#endif
