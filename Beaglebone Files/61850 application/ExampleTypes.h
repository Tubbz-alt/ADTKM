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
/*!	\file		PrintView.h
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

#ifndef EXAMPLE_TYPES_INCLUDED
#define EXAMPLE_TYPES_INCLUDED 1

#include "sysctype.h"

/******************************************************************************
*	Defines
******************************************************************************/

enum ExampleErrorCodes{
	EXAMPLE_NO_ERROR 		 		 =  0,
	EXAMPLE_INVALID_PARAMETERS 		 = -1,
	EXAMPLE_INVALID_USER_INPUT 		 = -2,
	EXAMPLE_INDEX_OUT_OF_BOUNDS		 = -2,
	EXAMPLE_CALLOC_FAILED			 = -3,
};

enum UserCommands {
	COMMAND_NONE	 = 0,
	COMMAND_EXIT	 = 1,
	COMMAND_READ_ALL = 2,
	COMMAND_OPERATE_CSWI	 = 3,
	COMMAND_OPERATE_GGIO	 = 4,
	COMMAND_UPDATE_MMXU_PHSA	 = 5,
	COMMAND_UPDATE_MMXU_PHSB	 = 6,
	COMMAND_UPDATE_MMXU_PHSC	 = 7,
	COMMAND_UPDATE_PDIF_XVAL	 = 8,
	COMMAND_UPDATE_PDIF_YVAL	 = 9,
	COMMAND_MMS_GET_CONNECTED_SERVER_LIST = 10,
	COMMAND_MMS_GET_FILE_ATTR	 = 11,
	COMMAND_MMS_GET_FILE		 = 12,
	COMMAND_MMS_DELETE_FILE		 = 13,
	COMMAND_UPDATE_MMXU_AA      = 14,
	COMMAND_UPDATE_MMXU_AB      = 15,
	COMMAND_UPDATE_MMXU_AC      =16,
	COMMAND_REFRESH             =17,
};


/* Define Structure to user for the CrvPts array */
typedef struct{
	Float32 xVal;
	Float32 yVal;
}CrvPts;

#define NUM_CRVPTS_STRUCT_ELEMENTS 2  //The Number of Elements in the Structure holding the CrvPts
/******************************************************************************
*	Includes
******************************************************************************/

#include <stdio.h>

/******************************************************************************
*	Prototypes
******************************************************************************/



#endif
