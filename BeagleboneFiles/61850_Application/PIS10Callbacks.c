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
/*!	\file		PIS1Callbacks.c
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
#include "PIS10Callbacks.h"


/******************************************************************************
*       ReadCallbackHandler Function Definition
******************************************************************************/
/*!  \brief       Callback Function for when a Read has occurred
 *
 *   \ingroup       Callback Handlers
 *
 *   \param[in]     ptUserData       	Pointer to the user data memory declared during creation
 *	 \param[in]	    ptDataAttributeID	Pointer to the data attribute ID that generated the callback
 *	 \param[in]	    ptReturnedValue		Pointer to the new value that generated the callback
 *
 *   \return        Returns an enum IEC61850_CallbackReturnServiceErrorCodes
 ******************************************************************************/
enum IEC61850_CallbackReturnServiceErrorCodes ReadCallbackHandler( void *ptUserData, struct IEC61850_DataAttributeID *ptDataAttributeID, struct IEC61850_DataAttributeData *ptReturnedValue)
{
	enum IEC61850_CallbackReturnServiceErrorCodes ErrorCode = IEC61850_CB_ERROR_NONE; //Create return value and init to No Error

	return  ErrorCode;
}

/******************************************************************************
*       WriteCallbackHandler Function Definition
******************************************************************************/
/*!  \brief         Callback Function for when a Write has occurred
 *
 *    \ingroup       Callback Handlers
 *
 *    \param[in]     ptUserData       	Pointer to the user data memory declared during creation
 *	  \param[in]	 ptDataAttributeID	Pointer to the data attribute ID that generated the callback
 *	  \param[in]	 ptReturnedValue	Pointer to the new value that generated the callback
 *
 *    \return        Returns an enum IEC61850_CallbackReturnServiceErrorCodes
 ******************************************************************************/
enum IEC61850_CallbackReturnServiceErrorCodes   WriteCallbackHandler( void *ptUserData, struct IEC61850_DataAttributeID *ptDataAttributeID, struct IEC61850_DataAttributeData * ptNewValue)
{
	enum IEC61850_CallbackReturnServiceErrorCodes ErrorCode = IEC61850_CB_ERROR_NONE; //Create return value and init to No Error

	return ErrorCode;
}

/******************************************************************************
*       SelectCallbackHandler Function Definition
******************************************************************************/
/*!  \brief         Callback Function for when a Select has occurred
 *
 *   \ingroup       Callback Handlers
 *
 *   \param[in]     ptUserData       		Pointer to the user data memory declared during creation
 *	 \param[in]	    ptControlID			Pointer to the data attribute ID that generated the callback
 *	 \param[in]	    ptSelectValue		Pointer to the selected value that generated the callback
 *	 \param[in]	    iSyncroCheck		If non zero the control is to perform a Syncro check (if supported by control) prior to selection
 *	 \param[in]	    iInterlockCheck		If non zero the control is to perform a Interlock check (if supported by control) prior to selection
 *
 *   \return        Returns an enum IEC61850_CallbackReturnServiceErrorCodes
 ******************************************************************************/
enum eCommandAddCause  SelectCallbackHandler( void *ptUserData, struct IEC61850_DataAttributeID * ptControlID, struct IEC61850_DataAttributeData * ptSelectValue, struct IEC61850_CommandParameters* ptSelectParameters)
{
	enum eCommandAddCause ErrorCode = IEC61850_COMMAND_ERROR_NONE; //Create return value and init to No Error

	return ErrorCode;
}


/******************************************************************************
*       OperateCallbackHandler Function Definition
******************************************************************************/
/*!  \brief         Callback Function for when an Operate has occurred
 *
 *   \ingroup       Callback Handlers
 *
 *   \param[in]     ptUserData       		Pointer to the user data memory declared during creation
 *	 \param[in]	    ptControlID			Pointer to the data attribute ID that generated the callback
 *	 \param[in]	    ptSelectValue		Pointer to the selected value that generated the callback
 *	 \param[in]	    iSyncroCheck		If non zero the control is to perform a Syncro check (if supported by control) prior to selection
 *	 \param[in]	    iInterlockCheck		If non zero the control is to perform a Interlock check (if supported by control) prior to selection
 *
 *   \return                  Returns an enum IEC61850_CallbackReturnServiceErrorCodes
 ******************************************************************************/
enum eCommandAddCause  OperateCallbackHandler( void *ptUserData, struct IEC61850_DataAttributeID * ptControlID, struct IEC61850_DataAttributeData * ptOperateValue, struct IEC61850_CommandParameters* ptOperateParameters)
{
	enum eCommandAddCause ErrorCode = IEC61850_COMMAND_ERROR_NONE; //Create return value and init to No Error

	enum IEC61850_ErrorCodes eErrorCode = IEC61850_ERROR_NONE;
	struct IEC61850_DataAttributeID_Generic *DaidGeneric = (struct IEC61850_DataAttributeID_Generic *)ptControlID;

	struct IEC61850_DataAttributeID_Generic StValDAID = {0}; //We use the Generic version as it is easier to assign the Data Attribute IDs
	struct IEC61850_DataAttributeData StValDAData = {0}; // Always initialize the structure to 0 to avoid Garbage data value
	IEC61850 myIEC61850Object = GetMyServerClient(); //Get the IEC61850 Structure
	Integer8 dbPosValue = DBPOS_OFF;
	Boolean boolValue = FALSE;

	/* ==== IMPORTANT NOTE ====
	 * We update the StVal related to the control in this callback
	 * This StVal should represent the hardware state and should be
	 * updated only when the hardware state changes
	 *
	 * This example does not have hardware attached so the StVal
	 * is updated when the operate happens to "emulate" a hardware change
	 * ========================
	 */

	if( (DaidGeneric->uiField1 == 1) && (DaidGeneric->uiField2 == 1) && (DaidGeneric->uiField3 == 0) && (DaidGeneric->uiField4 == 0) && (DaidGeneric->uiField5 == 0) )
	{/* CSWI */

		/* Setup the Data Attribute ID (DAID) Struct - We set this to the CtlVal of the Control*/
		StValDAID.Generic_type = IEC61850_DAID_GENERIC;
		StValDAID.uiField1 = 1;
		StValDAID.uiField2 = 0;
		StValDAID.uiField3 = 0;
		StValDAID.uiField4 = 0;
		StValDAID.uiField5 = 0;

		/* Setup the Data Attribute Data Struct */
		StValDAData.uiBitLength = IEC61850_DBPOS_BITSIZE; //sizeof(dbPosValue) * 8; //Set the size of the data in bits
		StValDAData.ucType = IEC61850_DATATYPE_DBPOS; //set the type of the data
		StValDAData.pvData = &dbPosValue; //Set a pointer to the data

		/* Set the DBPos Value based on the Operate's value to emulate hardware change */
		if( (*((Integer8*)ptOperateValue->pvData)) == FALSE )
		{
			dbPosValue = DBPOS_OFF;
		}
		else
		{
			dbPosValue = DBPOS_ON;
		}

		/* Perform the Update, note the IEC61850_DataAttributeID_Generic needs to be typecast to a standard struct IEC61850_DataAttributeID */
		eErrorCode = IEC61850_Update(myIEC61850Object, (struct IEC61850_DataAttributeID*)&StValDAID, &StValDAData, 1); //count of 1 as only 1 element is being updated
		if(eErrorCode == IEC61850_ERROR_NONE)
		{
			/* If the Update Succeeded Update the local value for the gui*/
			SetCSWIStVal((enum DbPosValues)dbPosValue);
			SetCSWICtlVal((*((Integer8*)ptOperateValue->pvData)));
			/*Redraw Server GUI  */
			PrintServerFullView();
		}
		else
		{
			/* If the Update failed alert the user */
			char strError[SIZE_OF_ERROR_STRING] = {0};
			snprintf(strError, SIZE_OF_ERROR_STRING, "CSWI StVal: Update failed:(%i) %s\n", eErrorCode, IEC61850_ErrorString(eErrorCode));
			SetErrorString(strError, SIZE_OF_ERROR_STRING);
		}
	}
	else if( (DaidGeneric->uiField1 == 2) && (DaidGeneric->uiField2 == 1) && (DaidGeneric->uiField3 == 0) && (DaidGeneric->uiField4 == 0) && (DaidGeneric->uiField5 == 0) )
	{/* GGIO */

		/* Setup the Data Attribute ID (DAID) Struct - We set this to the CtlVal of the Control*/
		StValDAID.Generic_type = IEC61850_DAID_GENERIC;
		StValDAID.uiField1 = 2;
		StValDAID.uiField2 = 0;
		StValDAID.uiField3 = 0;
		StValDAID.uiField4 = 0;
		StValDAID.uiField5 = 0;

		/* Setup the Data Attribute Data Struct */
		StValDAData.uiBitLength = sizeof(boolValue) * 8; //Set the size of the data in bits
		StValDAData.ucType = IEC61850_DATATYPE_BOOLEAN; //set the type of the data
		StValDAData.pvData = &boolValue; //Set a pointer to the data

		/* Set the DBPos Value based on the Operate's value to emulate hardware change */
		boolValue = (*((Integer8*)ptOperateValue->pvData)) ;

		/* Perform the Update, note the IEC61850_DataAttributeID_Generic needs to be typecast to a standard struct IEC61850_DataAttributeID */
		eErrorCode = IEC61850_Update(myIEC61850Object, (struct IEC61850_DataAttributeID*)&StValDAID, &StValDAData, 1); //count of 1 as only 1 element is being updated

		if(eErrorCode == IEC61850_ERROR_NONE)
		{
			/* If the Update Succeeded Update the local value for the gui*/
			SetGGIOStVal(boolValue);
			SetGGIOCtlVal(boolValue);
			/*Redraw Server GUI  */
			PrintServerFullView();
		}
		else
		{
			/* If the Update failed alert the user */
			char strError[SIZE_OF_ERROR_STRING] = {0};
			snprintf(strError, SIZE_OF_ERROR_STRING, "GGIO StVal: Update failed:(%i) %s\n", eErrorCode, IEC61850_ErrorString(eErrorCode));
			SetErrorString(strError, SIZE_OF_ERROR_STRING);
			/*Redraw Server GUI  */
			PrintServerFullView();
		}
	}
    else if( (DaidGeneric->uiField1 == 1) && (DaidGeneric->uiField2 == 1) && (DaidGeneric->uiField3 == 0) && (DaidGeneric->uiField4 == 0) && (DaidGeneric->uiField5 == 0) )
	{/* GGIO2 */

		/* Setup the Data Attribute ID (DAID) Struct - We set this to the CtlVal of the Control*/
		StValDAID.Generic_type = IEC61850_DAID_GENERIC;
		StValDAID.uiField1 = 1;
		StValDAID.uiField2 = 0;
		StValDAID.uiField3 = 0;
		StValDAID.uiField4 = 0;
		StValDAID.uiField5 = 0;

		/* Setup the Data Attribute Data Struct */
		StValDAData.uiBitLength = sizeof(boolValue) * 8; //Set the size of the data in bits
		StValDAData.ucType = IEC61850_DATATYPE_BOOLEAN; //set the type of the data
		StValDAData.pvData = &boolValue; //Set a pointer to the data

		/* Set the DBPos Value based on the Operate's value to emulate hardware change */
		boolValue = (*((Integer8*)ptOperateValue->pvData)) ;

		/* Perform the Update, note the IEC61850_DataAttributeID_Generic needs to be typecast to a standard struct IEC61850_DataAttributeID */
		eErrorCode = IEC61850_Update(myIEC61850Object, (struct IEC61850_DataAttributeID*)&StValDAID, &StValDAData, 1); //count of 1 as only 1 element is being updated

		if(eErrorCode == IEC61850_ERROR_NONE)
		{
			/* If the Update Succeeded Update the local value for the gui*/
			SetGGIO2StVal(boolValue);
			SetGGIO2CtlVal(boolValue);
			/*Redraw Server GUI  */
			PrintServerFullView();
		}
		else
		{
			/* If the Update failed alert the user */
			char strError[SIZE_OF_ERROR_STRING] = {0};
			snprintf(strError, SIZE_OF_ERROR_STRING, "GGIO2 StVal: Update failed:(%i) %s\n", eErrorCode, IEC61850_ErrorString(eErrorCode));
			SetErrorString(strError, SIZE_OF_ERROR_STRING);
			/*Redraw Server GUI  */
			PrintServerFullView();
		}
	}


	return ErrorCode;
}

/******************************************************************************
*       CancelCallbackHandler Function Definition
******************************************************************************/
/*!  \brief       Callback Function for when a Cancel has occurred
 *
 *   \ingroup      Callback Handlers
 *
 *   \param[in]     ptUserData       	Pointer to the user data memory declared during creation
 *	 \param[in]	    ptControlID			Pointer to the data attribute ID that generated the callback
 *
 *   \return        Returns an enum IEC61850_CallbackReturnServiceErrorCodes
 ******************************************************************************/
enum eCommandAddCause  CancelCallbackHandler( void *ptUserData, struct IEC61850_DataAttributeID * ptControlID, struct IEC61850_CommandParameters* ptCancelParameters)
{
	enum eCommandAddCause ErrorCode = IEC61850_COMMAND_ERROR_NONE; //Create return value and init to No Error

	return ErrorCode;
}


/******************************************************************************
*       UpdateCallbackHandler Function Definition
******************************************************************************/
/*!  \brief      Callback Function for when an Update has occured
 *
 *   \ingroup       Callback Handlers
 *
 *   \param[in]     ptUserData       	Pointer to the user data memory declared during creation
 *	 \param[in]	    ptControlID			Pointer to the data attribute ID that generated the callback
 *	 \param[in]	    ptSelectValue		Pointer to the new value that has been sent
 *
 *   \return        None
 ******************************************************************************/
void  UpdateCallbackHandler( void *ptUserData, struct IEC61850_DataAttributeID * ptUpdateID, const struct IEC61850_DataAttributeData *ptNewValue)
{
	struct IEC61850_DataAttributeID_Generic *DaidGeneric = (struct IEC61850_DataAttributeID_Generic *)ptUpdateID; //Type cast ptUpdateID to a generic ID

	Integer32 int32LocalVal =0; //Temporary variable to hold the current Local value
	Integer32 int32NewVal =0; //Temporary variable to hold the current Local value
	Float32 f32LocalVal =0.0; //temp variable to hold the current local value
	Float32 f32NewVal =0.0; //temp variable to hold the current local value
	enum DbPosValues dbPosLocalValue = DBPOS_OFF;
	enum DbPosValues dbPosNewValue = DBPOS_OFF;
	enum IEC61850DbPosValues dbPoseNetworkValue = IEC61850_DB_POS_OFF; // The value from the callback uses IEC61850DbPosValues (hex values)

	Boolean boolLocalValue = FALSE;
	Boolean boolNewValue = FALSE;
	CrvPts crvPtsLocalValue = {0};
	Float32 XYValNewValue = 0;

	//printf("updatecallback\n");


	if( (DaidGeneric->uiField1 == 3) && (DaidGeneric->uiField2 == 0) && (DaidGeneric->uiField3 == 0) && (DaidGeneric->uiField4 == 0) && (DaidGeneric->uiField5 == 0) )
	{/* MMXU PhsV PhsA Mag f */

		f32LocalVal = GetMMXUPhsAMagi();
		f32NewVal = (*((Float32*)ptNewValue->pvData));

		/* Check if the value has changed and if so update the local value and the GUI */
		if(f32LocalVal != f32NewVal)
		{
			SetMMXUPhsAMagi(f32NewVal);
			//PrintClientFullView();
		}
	}
	else if( (DaidGeneric->uiField1 == 4) && (DaidGeneric->uiField2 == 0) && (DaidGeneric->uiField3 == 0) && (DaidGeneric->uiField4 == 0) && (DaidGeneric->uiField5 == 0) )
	{/* MMXU PhsV PhsB Mag f */
		f32LocalVal = GetMMXUPhsBMagi();
		f32NewVal = (*((Float32*)ptNewValue->pvData));

		/* Check if the value has changed and if so update the local value and the GUI */
		if(f32LocalVal != f32NewVal)
		{
			SetMMXUPhsBMagi(f32NewVal);
			//PrintClientFullView();
		}
	}
	else if( (DaidGeneric->uiField1 == 5) && (DaidGeneric->uiField2 == 0) && (DaidGeneric->uiField3 == 0) && (DaidGeneric->uiField4 == 0) && (DaidGeneric->uiField5 == 0) )
	{/* MMXU PhsV PhsC Mag f */
		f32LocalVal = GetMMXUPhsCMagi();
		f32NewVal = (*((Float32*)ptNewValue->pvData));

		/* Check if the value has changed and if so update the local value and the GUI */
		if(f32LocalVal != f32NewVal)
		{
			SetMMXUPhsCMagi(f32NewVal);
			//PrintClientFullView();
		}
	}
	else if( (DaidGeneric->uiField1 == 6) && (DaidGeneric->uiField2 == 0) && (DaidGeneric->uiField3 == 0) && (DaidGeneric->uiField4 == 0) && (DaidGeneric->uiField5 == 0) )
	{/* MMXU A PhsA mag f  */
        f32LocalVal = GetMMXUAAMagf();
		f32NewVal = (*((Float32*)ptNewValue->pvData));

		/* Check if the value has changed and if so update the local value and the GUI */
		if(f32LocalVal != f32NewVal)
		{
			SetMMXUAAMagf(f32NewVal);
			//PrintClientFullView();
		}
	}
    else if( (DaidGeneric->uiField1 == 7) && (DaidGeneric->uiField2 == 0) && (DaidGeneric->uiField3 == 0) && (DaidGeneric->uiField4 == 0) && (DaidGeneric->uiField5 == 0) )
	{/* MMXU A PhsB mag f  */
        f32LocalVal = GetMMXUABMagf();
		f32NewVal = (*((Float32*)ptNewValue->pvData));

		/* Check if the value has changed and if so update the local value and the GUI */
		if(f32LocalVal != f32NewVal)
		{
			SetMMXUABMagf(f32NewVal);
			//PrintClientFullView();
		}
	}
    else if( (DaidGeneric->uiField1 == 8) && (DaidGeneric->uiField2 == 0) && (DaidGeneric->uiField3 == 0) && (DaidGeneric->uiField4 == 0) && (DaidGeneric->uiField5 == 0) )
	{/* MMXU A PhsC mag f  */
        f32LocalVal = GetMMXUACMagf();
		f32NewVal = (*((Float32*)ptNewValue->pvData));

		/* Check if the value has changed and if so update the local value and the GUI */
		if(f32LocalVal != f32NewVal)
		{
			SetMMXUACMagf(f32NewVal);
			//PrintClientFullView();
			//PrintServerFullView();
		}
	}
	else if( (DaidGeneric->uiField1 == 9) && (DaidGeneric->uiField2 == 0) && (DaidGeneric->uiField3 == 0) && (DaidGeneric->uiField4 == 0) && (DaidGeneric->uiField5 == 0) )
	{/* MMXU2 PhsV PhsA Mag f */

		f32LocalVal = GetMMXU2PhsAMagi();
		f32NewVal = (*((Float32*)ptNewValue->pvData));

		/* Check if the value has changed and if so update the local value and the GUI */
		if(f32LocalVal != f32NewVal)
		{
			SetMMXU2PhsAMagi(f32NewVal);
			//PrintClientFullView();
		}
	}
	else if( (DaidGeneric->uiField1 == 10) && (DaidGeneric->uiField2 == 0) && (DaidGeneric->uiField3 == 0) && (DaidGeneric->uiField4 == 0) && (DaidGeneric->uiField5 == 0) )
	{/* MMXU2 PhsV PhsB Mag f */
		f32LocalVal = GetMMXU2PhsBMagi();
		f32NewVal = (*((Float32*)ptNewValue->pvData));

		/* Check if the value has changed and if so update the local value and the GUI */
		if(f32LocalVal != f32NewVal)
		{
			SetMMXU2PhsBMagi(f32NewVal);
			//PrintClientFullView();
		}
	}
	else if( (DaidGeneric->uiField1 == 11) && (DaidGeneric->uiField2 == 0) && (DaidGeneric->uiField3 == 0) && (DaidGeneric->uiField4 == 0) && (DaidGeneric->uiField5 == 0) )
	{/* MMXU2 PhsV PhsC Mag f */
		f32LocalVal = GetMMXU2PhsCMagi();
		f32NewVal = (*((Float32*)ptNewValue->pvData));

		/* Check if the value has changed and if so update the local value and the GUI */
		if(f32LocalVal != f32NewVal)
		{
			SetMMXU2PhsCMagi(f32NewVal);
			//PrintClientFullView();
		}
	}
	else if( (DaidGeneric->uiField1 == 6) && (DaidGeneric->uiField2 == 1) && (DaidGeneric->uiField3 == 0) && (DaidGeneric->uiField4 == 0) && (DaidGeneric->uiField5 == 0) )
	{/* MMXU2 A PhsA mag f  */
        f32LocalVal = GetMMXU2AAMagf();
		f32NewVal = (*((Float32*)ptNewValue->pvData));

		/* Check if the value has changed and if so update the local value and the GUI */
		if(f32LocalVal != f32NewVal)
		{
			SetMMXU2AAMagf(f32NewVal);
			//PrintClientFullView();
		}
	}
	else if( (DaidGeneric->uiField1 == 7) && (DaidGeneric->uiField2 == 1) && (DaidGeneric->uiField3 == 0) && (DaidGeneric->uiField4 == 0) && (DaidGeneric->uiField5 == 0) )
	{/* MMXU2 A PhsB mag f  */
        f32LocalVal = GetMMXU2ABMagf();
		f32NewVal = (*((Float32*)ptNewValue->pvData));

		/* Check if the value has changed and if so update the local value and the GUI */
		if(f32LocalVal != f32NewVal)
		{
			SetMMXU2ABMagf(f32NewVal);
			//PrintClientFullView();
		}
	}
	else if( (DaidGeneric->uiField1 == 8) && (DaidGeneric->uiField2 == 1) && (DaidGeneric->uiField3 == 0) && (DaidGeneric->uiField4 == 0) && (DaidGeneric->uiField5 == 0) )
	{/* MMXU2 A PhsC mag f  */
        f32LocalVal = GetMMXU2ACMagf();
		f32NewVal = (*((Float32*)ptNewValue->pvData));

		/* Check if the value has changed and if so update the local value and the GUI */
		if(f32LocalVal != f32NewVal)
		{
			SetMMXU2ACMagf(f32NewVal);
			PrintClientFullView();
			PrintServerFullView();
		}
	}
}

/******************************************************************************
*       ErrorCallbackHandler Function Definition
******************************************************************************/
/*!  \brief       Function that is called for an error callback
 *
 *   \ingroup       Callback Handlers
 *
 *   \param[in]     ptUserData       	Pointer to the user data memory declared during creation
 *	 \param[in]	    ptControlID			Pointer to the data attribute ID that generated the callback
 *	 \param[in]	    ptErrorParamtrs		Pointer to the struct IEC61850_ErrorParameters
 *
 *   \return        None
 *
 ******************************************************************************/
enum eCommandAddCause ErrorCallbackHandler(void * ptUserData, struct IEC61850_DataAttributeID * ptDataAttributeID,  struct IEC61850_ErrorParameters * ptErrorParamtrs)
{
	enum eCommandAddCause eError = IEC61850_COMMAND_ERROR_NONE;

	//printf("\n========= Error Callback  =======\n");
	//printf("\t The error for %s/%s \n", ptErrorParamtrs->uDomainName, ptErrorParamtrs->uItemName);
	//printf("\t The cause is %s and type %s \n", ptErrorParamtrs->uErrorCause, ptErrorParamtrs->uErrorType);
	//printf("\t The DAID is: ");
	//IEC61850_PrintDataAttributeID(ptDataAttributeID);
	//printf("\n");

	return eError;
};

/******************************************************************************
*       OperativeTestCallbackHandler Function Definition
******************************************************************************/
/*!  \brief     Function that is called for an Operative test  callback
 *
 *   \ingroup       Callback Handlers
 *
 *   \param[in]     ptUserData       	Pointer to the user data memory declared during creation
 *	 \param[in]	    ptControlID			Pointer to the data attribute ID that generated the callback
 *	 \param[in]	    ptOperativeTestParameters		Pointer to the struct IEC61850_CommandParameters
 *
 *   \return  None
 ******************************************************************************/
enum eCommandAddCause OperativeTestCallbackHandler(void * ptUserData, struct IEC61850_DataAttributeID * ptControlID, struct IEC61850_CommandParameters* ptOperativeTestParameters)
{
	enum eCommandAddCause eError = IEC61850_COMMAND_ERROR_NONE;

	//printf("\n========= Operative Test Callback  =======\n");
	//printf("\t The DAID is: ");
	//IEC61850_PrintDataAttributeID(ptControlID);
	//printf("\n");

	return eError;
}


/******************************************************************************
*       QuestionableCallbackHandler Function Definition
******************************************************************************/
/*!  \brief       Function that is called for a Questionable data point callback
 *
 *   \ingroup       Callback Handlers
 *
 *   \param[in]     ptUserData       	Pointer to the user data memory declared during creation
 *	 \param[in]	    ptDataAttributeID	Pointer to the data attribute ID that generated the callback
 *
 *   \return  None
 ******************************************************************************/
void QuestionableCallbackHandler(void * ptUserData, struct IEC61850_DataAttributeID * ptDataAttributeID)
{

	//printf("\n========= Questionable Data Point Callback =======\n");
	//printf("\t The DAID is: ");
	//IEC61850_PrintDataAttributeID(ptDataAttributeID);
	//printf("\n");
	if(ptUserData != NULL)  //If the data is not NULL then this is a server subscriber
	{
		struct IEC61850_DataAttributeID_Generic * ptGeneric = (struct IEC61850_DataAttributeID_Generic *)ptDataAttributeID;

		/* If the Data point is questionable alert the user */
		char strError[SIZE_OF_ERROR_STRING] = {0};
		snprintf(strError, SIZE_OF_ERROR_STRING, "GOOSE Data point is questionable - Private ID: %u, %u, %u, %u, %u", ptGeneric->uiField1, ptGeneric->uiField2, ptGeneric->uiField3, ptGeneric->uiField4, ptGeneric->uiField5);
		SetErrorString(strError, SIZE_OF_ERROR_STRING);

		PrintServerSubscriptionFullView(); // update server subscriber screen
	}
}


/******************************************************************************
*       CommandTerminationCallback Function Definition
******************************************************************************/
/*!  \brief       Function that is called for a Command Termination callback
 *
 *   \ingroup       Callback Handlers
 *
 *   \param[in]     ptUserData       	Pointer to the user data memory declared during creation
 *	 \param[in]	    ptControlID			Pointer to the data attribute ID that generated the callback
 *	 \param[in]	    ptCmdTermValue		Pointer to the value that has been sent
 *
 *   \return  None
 ******************************************************************************/
enum eCommandAddCause CommandTerminationCallback(void * ptUserData, struct IEC61850_DataAttributeID * ptControlID, struct IEC61850_DataAttributeData * ptCmdTermValue)
{
	enum eCommandAddCause eError = IEC61850_COMMAND_ERROR_NONE;

	//printf("\n========= Command Termination Callback =======\n");
	//printf("\t The DAID is: ");
	//IEC61850_PrintDataAttributeID(ptControlID);
	//printf("\n");

	return eError;
}
