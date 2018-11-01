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
/*!	\file		IEC61850Functions.h
*	\brief 		C header file for the PIS10 Stack Example
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


#ifndef IEC61850_FUNCTIONS_INCLUDED
#define IEC61850_FUNCTIONS_INCLUDED 1



/******************************************************************************
*	Includes
******************************************************************************/

#include <stdio.h>

#include "IEC61850Types.h"
#include "IEC61850API.h"
#include "sysctype.h"

#include "LocalData.h"
#include "PIS10CreateServerClient.h"

/******************************************************************************
*	Prototypes
******************************************************************************/
enum IEC61850_ErrorCodes SelectCSWI(Boolean inCtrlValue);
enum IEC61850_ErrorCodes OperateCSWI(Boolean inCtrlValue);

enum IEC61850_ErrorCodes SelectGGIO(Boolean inCtrlValue);
enum IEC61850_ErrorCodes OperateGGIO(Boolean inCtrlValue);

enum IEC61850_ErrorCodes UpdateMMXUPhsAMagi(Float32 inUpdateValue);
enum IEC61850_ErrorCodes UpdateMMXUPhsBMagi(Float32 inUpdateValue);
enum IEC61850_ErrorCodes UpdateMMXUPhsCMagi(Float32 inUpdateValue);
enum IEC61850_ErrorCodes UpdateMMXUAAMagf(Float32 inUpdateValue);
enum IEC61850_ErrorCodes UpdateMMXUABMagf(Float32 inUpdateValue);
enum IEC61850_ErrorCodes UpdateMMXU2ACMagf(Float32 inUpdateValue);
enum IEC61850_ErrorCodes UpdateMMXU2PhsAMagi(Float32 inUpdateValue);
enum IEC61850_ErrorCodes UpdateMMXU2PhsBMagi(Float32 inUpdateValue);
enum IEC61850_ErrorCodes UpdateMMXU2PhsCMagi(Float32 inUpdateValue);
enum IEC61850_ErrorCodes UpdateMMXU2AAMagf(Float32 inUpdateValue);
enum IEC61850_ErrorCodes UpdateMMXU2ABMagf(Float32 inUpdateValue);
enum IEC61850_ErrorCodes UpdateMMXU2ACMagf(Float32 inUpdateValue);

enum IEC61850_ErrorCodes UpdatePDIFCrvPts(CrvPts inUpdateValue, Integer8 inIndex);

enum IEC61850_ErrorCodes MMSGetConnectedServerList();
enum IEC61850_ErrorCodes MMSGetFileAttribs();
enum IEC61850_ErrorCodes MMSGetFile();
enum IEC61850_ErrorCodes MMSDeleteFile();

#endif
