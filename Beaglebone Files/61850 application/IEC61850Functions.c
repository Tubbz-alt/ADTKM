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
/*!	\file		PIS10Handlers.c
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
#include "IEC61850Functions.h"
#include "IEC61850Types.h"
#include "PIS10CreateServerClient.h"

#include <stdio.h>
#include <string.h>

void printServerList(struct ConnectedServerList *connectedServerList);
unsigned int GetServerIndexFromUser();
void GetFileNameFromUser(char * messageString, char * userInput);

/******************************************************************************
*       SelectCSWI Function Definition
******************************************************************************/
/*!  \brief        This function performs an IEC61850 Select on the CSWI with the value passed in
 *
 *   \ingroup       Callback Handlers
 *
 *   \param[in]     inCtrlValue      	A Boolean that holds the value that the control will be Selected with
 *
 *   \return        IEC61850_ERROR_NONE on success
 *   \return        otherwise the relevant enum IEC61850_ErrorCodes on failure
 ******************************************************************************/
enum IEC61850_ErrorCodes SelectCSWI(Boolean inCtrlValue)
{
	enum IEC61850_ErrorCodes eErrorCode = IEC61850_ERROR_NONE;

	struct IEC61850_DataAttributeID_Generic CtrlDAID = {0}; //We use the Generic version as it is easier to assign the Data Attribute IDs
	struct IEC61850_DataAttributeData CtrlDAData = {0}; // Always initialize the structure to 0 to avoid Garbage data value
	struct IEC61850_ControlParameters CtrlControlParams = {0};

	IEC61850 myIEC61850Object = GetMyServerClient(); //Get the IEC61850 Structure

	/* Setup the Data Attribute ID (DAID) Struct - We set this to the CtlVal of the Control*/
	CtrlDAID.Generic_type = IEC61850_DAID_GENERIC;
	CtrlDAID.uiField1 = 1;
	CtrlDAID.uiField2 = 1;
	CtrlDAID.uiField3 = 0;
	CtrlDAID.uiField4 = 0;
	CtrlDAID.uiField5 = 0;

	/* Setup the Data Attribute Data Struct */
	CtrlDAData.uiBitLength = sizeof(Boolean) * 8; //Set the size of the data in bits
	CtrlDAData.ucType = IEC61850_DATATYPE_BOOLEAN; //set the type of the data
	CtrlDAData.pvData = &inCtrlValue; //Set a pointer to the data

	/* Perform the Select, note the IEC61850_DataAttributeID_Generic needs to be typecast to a standard struct IEC61850_DataAttributeID */
	eErrorCode = IEC61850_ControlSelect(myIEC61850Object, (struct IEC61850_DataAttributeID*)&CtrlDAID, &CtrlDAData, CtrlControlParams);

	/* If the Select failed alert the user */
	if(eErrorCode != IEC61850_ERROR_NONE)
	{
		char strError[SIZE_OF_ERROR_STRING] = {0};
		snprintf(strError, SIZE_OF_ERROR_STRING, "CSWI Select failed:(%i) %s\n", eErrorCode, IEC61850_ErrorString(eErrorCode));
		SetErrorString(strError, SIZE_OF_ERROR_STRING);
	}
	else
	{
		/* Set the Error String to NONE */
		char strError[SIZE_OF_ERROR_STRING] = {0};
		SetErrorString(strError, SIZE_OF_ERROR_STRING);
	}

	return eErrorCode;
}

/******************************************************************************
*       OperateCSWI Function Definition
******************************************************************************/
/*!  \brief        This function performs an IEC61850 Operate on the CSWI with the value passed in
 *
 *   \ingroup       Callback Handlers
 *
 *   \param[in]     inCtrlValue      	A Boolean that holds the value that the control will be operated to
 *
 *   \return        IEC61850_ERROR_NONE on success
 *   \return        otherwise the relevant enum IEC61850_ErrorCodes on failure
 ******************************************************************************/
enum IEC61850_ErrorCodes OperateCSWI(Boolean inCtrlValue)
{
	enum IEC61850_ErrorCodes eErrorCode = IEC61850_ERROR_NONE;

	struct IEC61850_DataAttributeID_Generic CtrlDAID = {0}; //We use the Generic version as it is easier to assign the Data Attribute IDs
	struct IEC61850_DataAttributeData CtrlDAData = {0}; // Always initialize the structure to 0 to avoid Garbage data value
	struct IEC61850_ControlParameters CtrlControlParams = {0};

	IEC61850 myIEC61850Object = GetMyServerClient(); //Get the IEC61850 Structure

	/* Setup the Data Attribute ID (DAID) Struct - We set this to the CtlVal of the Control*/
	CtrlDAID.Generic_type = IEC61850_DAID_GENERIC;
	CtrlDAID.uiField1 = 1;
	CtrlDAID.uiField2 = 1;
	CtrlDAID.uiField3 = 0;
	CtrlDAID.uiField4 = 0;
	CtrlDAID.uiField5 = 0;

	/* Setup the Data Attribute Data Struct */
	CtrlDAData.uiBitLength = sizeof(Boolean) * 8; //Set the size of the data in bits
	CtrlDAData.ucType = IEC61850_DATATYPE_BOOLEAN; //set the type of the data
	CtrlDAData.pvData = &inCtrlValue; //Set a pointer to the data

	/* Perform the Operate, note the IEC61850_DataAttributeID_Generic needs to be typecast to a standard struct IEC61850_DataAttributeID */
	eErrorCode = IEC61850_ControlOperate(myIEC61850Object, (struct IEC61850_DataAttributeID*)&CtrlDAID, &CtrlDAData, CtrlControlParams);

	if(eErrorCode == IEC61850_ERROR_NONE)
	{
		/* Set the Error String to NONE */
		char strError[SIZE_OF_ERROR_STRING] = {0};
		SetErrorString(strError, SIZE_OF_ERROR_STRING);

		/* If the Operate Succeeded Update the local value for the gui*/
		SetCSWICtlVal(inCtrlValue);
	}
	else
	{
		/* If the Operate failed alert the user */
		char strError[SIZE_OF_ERROR_STRING] = {0};
		snprintf(strError, SIZE_OF_ERROR_STRING, "CSWI Operate failed:(%i) %s\n", eErrorCode, IEC61850_ErrorString(eErrorCode));
		SetErrorString(strError, SIZE_OF_ERROR_STRING);
	}

	return eErrorCode;
}



/******************************************************************************
*       OperateGGIO Function Definition
******************************************************************************/
/*!  \brief        This function performs an IEC61850 Operate on the GGIO with the value passed in
 *
 *   \ingroup       Callback Handlers
 *
 *   \param[in]     inCtrlValue      	A Boolean that holds the value that the control will be operated to
 *
 *   \return        IEC61850_ERROR_NONE on success
 *   \return        otherwise the relevant enum IEC61850_ErrorCodes on failure
 ******************************************************************************/
enum IEC61850_ErrorCodes OperateGGIO(Boolean inCtrlValue)
{
	enum IEC61850_ErrorCodes eErrorCode = IEC61850_ERROR_NONE;

	struct IEC61850_DataAttributeID_Generic CtrlDAID = {0}; //We use the Generic version as it is easier to assign the Data Attribute IDs
	struct IEC61850_DataAttributeData CtrlDAData = {0}; // Always initialize the structure to 0 to avoid Garbage data value
	struct IEC61850_ControlParameters CtrlControlParams = {0};

	IEC61850 myIEC61850Object = GetMyServerClient(); //Get the IEC61850 Structure

	/* Setup the Data Attribute ID (DAID) Struct - We set this to the CtlVal of the Control*/
	CtrlDAID.Generic_type = IEC61850_DAID_GENERIC;
	CtrlDAID.uiField1 = 2;
	CtrlDAID.uiField2 = 1;
	CtrlDAID.uiField3 = 0;
	CtrlDAID.uiField4 = 0;
	CtrlDAID.uiField5 = 0;

	/* Setup the Data Attribute Data Struct */
	CtrlDAData.uiBitLength = sizeof(Boolean) * 8; //Set the size of the data in bits
	CtrlDAData.ucType = IEC61850_DATATYPE_BOOLEAN; //set the type of the data
	CtrlDAData.pvData = &inCtrlValue; //Set a pointer to the data

	/* Perform the Operate, note the IEC61850_DataAttributeID_Generic needs to be typecast to a standard struct IEC61850_DataAttributeID */
	eErrorCode = IEC61850_ControlOperate(myIEC61850Object, (struct IEC61850_DataAttributeID*)&CtrlDAID, &CtrlDAData, CtrlControlParams);

	if(eErrorCode == IEC61850_ERROR_NONE)
	{
		/* Set the Error String to NONE */
		char strError[SIZE_OF_ERROR_STRING] = {0};
		SetErrorString(strError, SIZE_OF_ERROR_STRING);

		/* If the Operate Succeeded Update the local value for the gui*/
		SetGGIOCtlVal(inCtrlValue);
	}
	else
	{
		/* If the Operate failed alert the user */
		char strError[SIZE_OF_ERROR_STRING] = {0};
		snprintf(strError, SIZE_OF_ERROR_STRING, "GGIO Operate failed:(%i) %s\n", eErrorCode, IEC61850_ErrorString(eErrorCode));
		SetErrorString(strError, SIZE_OF_ERROR_STRING);
	}

	return eErrorCode;
}



/******************************************************************************
*       OperateGGIO Function Definition
******************************************************************************/
/*!  \brief        This function performs an IEC61850 Operate on the GGIO with the value passed in
 *
 *   \ingroup       Callback Handlers
 *
 *   \param[in]     inCtrlValue      	A Boolean that holds the value that the control will be operated to
 *
 *   \return        IEC61850_ERROR_NONE on success
 *   \return        otherwise the relevant enum IEC61850_ErrorCodes on failure
 ******************************************************************************/
enum IEC61850_ErrorCodes OperateGGIO2(Boolean inCtrlValue)
{
	enum IEC61850_ErrorCodes eErrorCode = IEC61850_ERROR_NONE;

	struct IEC61850_DataAttributeID_Generic CtrlDAID = {0}; //We use the Generic version as it is easier to assign the Data Attribute IDs
	struct IEC61850_DataAttributeData CtrlDAData = {0}; // Always initialize the structure to 0 to avoid Garbage data value
	struct IEC61850_ControlParameters CtrlControlParams = {0};

	IEC61850 myIEC61850Object = GetMyServerClient(); //Get the IEC61850 Structure

	/* Setup the Data Attribute ID (DAID) Struct - We set this to the CtlVal of the Control*/
	CtrlDAID.Generic_type = IEC61850_DAID_GENERIC;
	CtrlDAID.uiField1 = 1;
	CtrlDAID.uiField2 = 1;
	CtrlDAID.uiField3 = 0;
	CtrlDAID.uiField4 = 0;
	CtrlDAID.uiField5 = 0;

	/* Setup the Data Attribute Data Struct */
	CtrlDAData.uiBitLength = sizeof(Boolean) * 8; //Set the size of the data in bits
	CtrlDAData.ucType = IEC61850_DATATYPE_BOOLEAN; //set the type of the data
	CtrlDAData.pvData = &inCtrlValue; //Set a pointer to the data

	/* Perform the Operate, note the IEC61850_DataAttributeID_Generic needs to be typecast to a standard struct IEC61850_DataAttributeID */
	eErrorCode = IEC61850_ControlOperate(myIEC61850Object, (struct IEC61850_DataAttributeID*)&CtrlDAID, &CtrlDAData, CtrlControlParams);

	if(eErrorCode == IEC61850_ERROR_NONE)
	{
		/* Set the Error String to NONE */
		char strError[SIZE_OF_ERROR_STRING] = {0};
		SetErrorString(strError, SIZE_OF_ERROR_STRING);

		/* If the Operate Succeeded Update the local value for the gui*/
		SetGGIO2CtlVal(inCtrlValue);
	}
	else
	{
		/* If the Operate failed alert the user */
		char strError[SIZE_OF_ERROR_STRING] = {0};
		snprintf(strError, SIZE_OF_ERROR_STRING, "GGIO2 Operate failed:(%i) %s\n", eErrorCode, IEC61850_ErrorString(eErrorCode));
		SetErrorString(strError, SIZE_OF_ERROR_STRING);
	}

	return eErrorCode;
}


/******************************************************************************
*       UpdateMMXUPhsAMagi Function Definition
******************************************************************************/
/*!  \brief        This function performs an IEC61850 Update on the UpdateMMXUPhsAMagi with the value passed in
 *
 *   \ingroup       Callback Handlers
 *
 *   \param[in]     inUpdateValue      	An Integer32 that holds the value that the UpdateMMXUPhsAMagi will be updated to
 *
 *   \return        IEC61850_ERROR_NONE on success
 *   \return        otherwise the relevant enum IEC61850_ErrorCodes on failure
 ******************************************************************************/
enum IEC61850_ErrorCodes UpdateMMXUPhsAMagi(Float32 inUpdateValue)
{
	enum IEC61850_ErrorCodes eErrorCode = IEC61850_ERROR_NONE;

	struct IEC61850_DataAttributeID_Generic updateDAID = {0}; //We use the Generic version as it is easier to assign the Data Attribute IDs
	struct IEC61850_DataAttributeData updateDAData = {0}; // Always initialize the structure to 0 to avoid Garbage data value

	IEC61850 myIEC61850Object = GetMyServerClient(); //Get the IEC61850 Structure

	/* Setup the Data Attribute ID (DAID) Struct */
	updateDAID.Generic_type = IEC61850_DAID_GENERIC;
	updateDAID.uiField1 = 3;
	updateDAID.uiField2 = 0;
	updateDAID.uiField3 = 0;
	updateDAID.uiField4 = 0;
	updateDAID.uiField5 = 0;

	/* Setup the Data Attribute Data Struct */
	updateDAData.uiBitLength = sizeof(Float32) * 8; //Set the size of the data in bits
	updateDAData.ucType = IEC61850_DATATYPE_FLOAT32; //set the type of the data
	updateDAData.pvData = &inUpdateValue; //Set a pointer to the data

	/* Perform the Update, note the IEC61850_DataAttributeID_Generic needs to be typecast to a standard struct IEC61850_DataAttributeID */
	eErrorCode = IEC61850_Update(myIEC61850Object, (struct IEC61850_DataAttributeID*)&updateDAID, &updateDAData, 1); //count of 1 as only 1 element is being updated


	if(eErrorCode == IEC61850_ERROR_NONE)
	{
		/* Set the Error String to NONE */
		char strError[SIZE_OF_ERROR_STRING] = {0};
		SetErrorString(strError, SIZE_OF_ERROR_STRING);

		/* If the Update Succeeded Update the local value for the gui*/
		SetMMXUPhsAMagi(inUpdateValue);
	}
	else
	{
		/* If the Update failed alert the user */
		char strError[SIZE_OF_ERROR_STRING] = {0};
		snprintf(strError, SIZE_OF_ERROR_STRING, "UpdateMMXUPhsAMagi Update failed:(%i) %s\n", eErrorCode, IEC61850_ErrorString(eErrorCode));
		SetErrorString(strError, SIZE_OF_ERROR_STRING);
	}

	return eErrorCode;
}


/******************************************************************************
*       UpdateMMXUPhsAMagi Function Definition
******************************************************************************/
/*!  \brief        This function performs an IEC61850 Update on the UpdateMMXUPhsAMagi with the value passed in
 *
 *   \ingroup       Callback Handlers
 *
 *   \param[in]     inUpdateValue      	An Integer32 that holds the value that the UpdateMMXUPhsAMagi will be updated to
 *
 *   \return        IEC61850_ERROR_NONE on success
 *   \return        otherwise the relevant enum IEC61850_ErrorCodes on failure
 ******************************************************************************/
enum IEC61850_ErrorCodes UpdateMMXU2PhsAMagi(Float32 inUpdateValue)
{
	enum IEC61850_ErrorCodes eErrorCode = IEC61850_ERROR_NONE;

	struct IEC61850_DataAttributeID_Generic updateDAID = {0}; //We use the Generic version as it is easier to assign the Data Attribute IDs
	struct IEC61850_DataAttributeData updateDAData = {0}; // Always initialize the structure to 0 to avoid Garbage data value

	IEC61850 myIEC61850Object = GetMyServerClient(); //Get the IEC61850 Structure

	/* Setup the Data Attribute ID (DAID) Struct */
	updateDAID.Generic_type = IEC61850_DAID_GENERIC;
	updateDAID.uiField1 = 9;
	updateDAID.uiField2 = 0;
	updateDAID.uiField3 = 0;
	updateDAID.uiField4 = 0;
	updateDAID.uiField5 = 0;

	/* Setup the Data Attribute Data Struct */
	updateDAData.uiBitLength = sizeof(Float32) * 8; //Set the size of the data in bits
	updateDAData.ucType = IEC61850_DATATYPE_FLOAT32; //set the type of the data
	updateDAData.pvData = &inUpdateValue; //Set a pointer to the data

	/* Perform the Update, note the IEC61850_DataAttributeID_Generic needs to be typecast to a standard struct IEC61850_DataAttributeID */
	eErrorCode = IEC61850_Update(myIEC61850Object, (struct IEC61850_DataAttributeID*)&updateDAID, &updateDAData, 1); //count of 1 as only 1 element is being updated


	if(eErrorCode == IEC61850_ERROR_NONE)
	{
		/* Set the Error String to NONE */
		char strError[SIZE_OF_ERROR_STRING] = {0};
		SetErrorString(strError, SIZE_OF_ERROR_STRING);

		/* If the Update Succeeded Update the local value for the gui*/
		SetMMXU2PhsAMagi(inUpdateValue);
	}
	else
	{
		/* If the Update failed alert the user */
		char strError[SIZE_OF_ERROR_STRING] = {0};
		snprintf(strError, SIZE_OF_ERROR_STRING, "UpdateMMXU2PhsAMagi Update failed:(%i) %s\n", eErrorCode, IEC61850_ErrorString(eErrorCode));
		SetErrorString(strError, SIZE_OF_ERROR_STRING);
	}

	return eErrorCode;
}


/******************************************************************************
*       UpdateMMXUPhsBMagi Function Definition
******************************************************************************/
/*!  \brief        This function performs an IEC61850 Update on the UpdateMMXUPhsBMagi with the value passed in
 *
 *   \ingroup       Callback Handlers
 *
 *   \param[in]     inUpdateValue      	An Integer32 that holds the value that the UpdateMMXUPhsBMagi will be updated to
 *
 *   \return        IEC61850_ERROR_NONE on success
 *   \return        otherwise the relevant enum IEC61850_ErrorCodes on failure
 ******************************************************************************/
enum IEC61850_ErrorCodes UpdateMMXUPhsBMagi(Float32 inUpdateValue)
{
	enum IEC61850_ErrorCodes eErrorCode = IEC61850_ERROR_NONE;

	struct IEC61850_DataAttributeID_Generic updateDAID = {0}; //We use the Generic version as it is easier to assign the Data Attribute IDs
	struct IEC61850_DataAttributeData updateDAData = {0}; // Always initialize the structure to 0 to avoid Garbage data value

	IEC61850 myIEC61850Object = GetMyServerClient(); //Get the IEC61850 Structure

	/* Setup the Data Attribute ID (DAID) Struct */
	updateDAID.Generic_type = IEC61850_DAID_GENERIC;
	updateDAID.uiField1 = 4;
	updateDAID.uiField2 = 0;
	updateDAID.uiField3 = 0;
	updateDAID.uiField4 = 0;
	updateDAID.uiField5 = 0;

	/* Setup the Data Attribute Data Struct */
	updateDAData.uiBitLength = sizeof(Float32) * 8; //Set the size of the data in bits
	updateDAData.ucType = IEC61850_DATATYPE_FLOAT32; //set the type of the data
	updateDAData.pvData = &inUpdateValue; //Set a pointer to the data

	/* Perform the Update, note the IEC61850_DataAttributeID_Generic needs to be typecast to a standard struct IEC61850_DataAttributeID */
	eErrorCode = IEC61850_Update(myIEC61850Object, (struct IEC61850_DataAttributeID*)&updateDAID, &updateDAData, 1); //count of 1 as only 1 element is being updated

	if(eErrorCode == IEC61850_ERROR_NONE)
	{
		/* Set the Error String to NONE */
		char strError[SIZE_OF_ERROR_STRING] = {0};
		SetErrorString(strError, SIZE_OF_ERROR_STRING);

		/* If the Update Succeeded Update the local value for the gui*/
		SetMMXUPhsBMagi(inUpdateValue);
	}
	else
	{
		/* If the Update failed alert the user */
		char strError[SIZE_OF_ERROR_STRING] = {0};
		snprintf(strError, SIZE_OF_ERROR_STRING, "MMXUPhsBMagi Update failed:(%i) %s\n", eErrorCode, IEC61850_ErrorString(eErrorCode));
		SetErrorString(strError, SIZE_OF_ERROR_STRING);
	}

	return eErrorCode;
}


/******************************************************************************
*       UpdateMMXUPhsBMagi Function Definition
******************************************************************************/
/*!  \brief        This function performs an IEC61850 Update on the UpdateMMXUPhsBMagi with the value passed in
 *
 *   \ingroup       Callback Handlers
 *
 *   \param[in]     inUpdateValue      	An Integer32 that holds the value that the UpdateMMXUPhsBMagi will be updated to
 *
 *   \return        IEC61850_ERROR_NONE on success
 *   \return        otherwise the relevant enum IEC61850_ErrorCodes on failure
 ******************************************************************************/
enum IEC61850_ErrorCodes UpdateMMXU2PhsBMagi(Float32 inUpdateValue)
{
	enum IEC61850_ErrorCodes eErrorCode = IEC61850_ERROR_NONE;

	struct IEC61850_DataAttributeID_Generic updateDAID = {0}; //We use the Generic version as it is easier to assign the Data Attribute IDs
	struct IEC61850_DataAttributeData updateDAData = {0}; // Always initialize the structure to 0 to avoid Garbage data value

	IEC61850 myIEC61850Object = GetMyServerClient(); //Get the IEC61850 Structure

	/* Setup the Data Attribute ID (DAID) Struct */
	updateDAID.Generic_type = IEC61850_DAID_GENERIC;
	updateDAID.uiField1 = 10;
	updateDAID.uiField2 = 0;
	updateDAID.uiField3 = 0;
	updateDAID.uiField4 = 0;
	updateDAID.uiField5 = 0;

	/* Setup the Data Attribute Data Struct */
	updateDAData.uiBitLength = sizeof(Float32) * 8; //Set the size of the data in bits
	updateDAData.ucType = IEC61850_DATATYPE_FLOAT32; //set the type of the data
	updateDAData.pvData = &inUpdateValue; //Set a pointer to the data

	/* Perform the Update, note the IEC61850_DataAttributeID_Generic needs to be typecast to a standard struct IEC61850_DataAttributeID */
	eErrorCode = IEC61850_Update(myIEC61850Object, (struct IEC61850_DataAttributeID*)&updateDAID, &updateDAData, 1); //count of 1 as only 1 element is being updated

	if(eErrorCode == IEC61850_ERROR_NONE)
	{
		/* Set the Error String to NONE */
		char strError[SIZE_OF_ERROR_STRING] = {0};
		SetErrorString(strError, SIZE_OF_ERROR_STRING);

		/* If the Update Succeeded Update the local value for the gui*/
		SetMMXU2PhsBMagi(inUpdateValue);
	}
	else
	{
		/* If the Update failed alert the user */
		char strError[SIZE_OF_ERROR_STRING] = {0};
		snprintf(strError, SIZE_OF_ERROR_STRING, "MMXU2PhsBMagi Update failed:(%i) %s\n", eErrorCode, IEC61850_ErrorString(eErrorCode));
		SetErrorString(strError, SIZE_OF_ERROR_STRING);
	}

	return eErrorCode;
}


/******************************************************************************
*       UpdateMMXUPhsCMagi Function Definition
******************************************************************************/
/*!  \brief        This function performs an IEC61850 Update on the MMXUPhsCMagi with the value passed in
 *
 *   \ingroup       Callback Handlers
 *
 *   \param[in]     inUpdateValue      	An Integer32 that holds the value that the MMXUPhsCMagi will be updated to
 *
 *   \return        IEC61850_ERROR_NONE on success
 *   \return        otherwise the relevant enum IEC61850_ErrorCodes on failure
 ******************************************************************************/
enum IEC61850_ErrorCodes UpdateMMXUPhsCMagi(Float32 inUpdateValue)
{
	enum IEC61850_ErrorCodes eErrorCode = IEC61850_ERROR_NONE;

	struct IEC61850_DataAttributeID_Generic updateDAID = {0}; //We use the Generic version as it is easier to assign the Data Attribute IDs
	struct IEC61850_DataAttributeData updateDAData = {0}; // Always initialize the structure to 0 to avoid Garbage data value

	IEC61850 myIEC61850Object = GetMyServerClient(); //Get the IEC61850 Structure

	/* Setup the Data Attribute ID (DAID) Struct */
	updateDAID.Generic_type = IEC61850_DAID_GENERIC;
	updateDAID.uiField1 = 5;
	updateDAID.uiField2 = 0;
	updateDAID.uiField3 = 0;
	updateDAID.uiField4 = 0;
	updateDAID.uiField5 = 0;

	/* Setup the Data Attribute Data Struct */
	updateDAData.uiBitLength = sizeof(Float32) * 8; //Set the size of the data in bits
	updateDAData.ucType = IEC61850_DATATYPE_FLOAT32; //set the type of the data
	updateDAData.pvData = &inUpdateValue; //Set a pointer to the data

	/* Perform the Update, note the IEC61850_DataAttributeID_Generic needs to be typecast to a standard struct IEC61850_DataAttributeID */
	eErrorCode = IEC61850_Update(myIEC61850Object, (struct IEC61850_DataAttributeID*)&updateDAID, &updateDAData, 1); //count of 1 as only 1 element is being updated

	if(eErrorCode == IEC61850_ERROR_NONE)
	{
		/* Set the Error String to NONE */
		char strError[SIZE_OF_ERROR_STRING] = {0};
		SetErrorString(strError, SIZE_OF_ERROR_STRING);

		/* If the Update Succeeded Update the local value for the gui*/
		SetMMXUPhsCMagi(inUpdateValue);
	}
	else
	{
		/* If the Update failed alert the user */
		char strError[SIZE_OF_ERROR_STRING] = {0};
		snprintf(strError, SIZE_OF_ERROR_STRING, "MMXUPhsCMagi Update failed:(%i) %s\n", eErrorCode, IEC61850_ErrorString(eErrorCode));
		SetErrorString(strError, SIZE_OF_ERROR_STRING);
	}

	return eErrorCode;
}


/******************************************************************************
*       UpdateMMXUPhsCMagi Function Definition
******************************************************************************/
/*!  \brief        This function performs an IEC61850 Update on the MMXUPhsCMagi with the value passed in
 *
 *   \ingroup       Callback Handlers
 *
 *   \param[in]     inUpdateValue      	An Integer32 that holds the value that the MMXUPhsCMagi will be updated to
 *
 *   \return        IEC61850_ERROR_NONE on success
 *   \return        otherwise the relevant enum IEC61850_ErrorCodes on failure
 ******************************************************************************/
enum IEC61850_ErrorCodes UpdateMMXU2PhsCMagi(Float32 inUpdateValue)
{
	enum IEC61850_ErrorCodes eErrorCode = IEC61850_ERROR_NONE;

	struct IEC61850_DataAttributeID_Generic updateDAID = {0}; //We use the Generic version as it is easier to assign the Data Attribute IDs
	struct IEC61850_DataAttributeData updateDAData = {0}; // Always initialize the structure to 0 to avoid Garbage data value

	IEC61850 myIEC61850Object = GetMyServerClient(); //Get the IEC61850 Structure

	/* Setup the Data Attribute ID (DAID) Struct */
	updateDAID.Generic_type = IEC61850_DAID_GENERIC;
	updateDAID.uiField1 = 11;
	updateDAID.uiField2 = 0;
	updateDAID.uiField3 = 0;
	updateDAID.uiField4 = 0;
	updateDAID.uiField5 = 0;

	/* Setup the Data Attribute Data Struct */
	updateDAData.uiBitLength = sizeof(Float32) * 8; //Set the size of the data in bits
	updateDAData.ucType = IEC61850_DATATYPE_FLOAT32; //set the type of the data
	updateDAData.pvData = &inUpdateValue; //Set a pointer to the data

	/* Perform the Update, note the IEC61850_DataAttributeID_Generic needs to be typecast to a standard struct IEC61850_DataAttributeID */
	eErrorCode = IEC61850_Update(myIEC61850Object, (struct IEC61850_DataAttributeID*)&updateDAID, &updateDAData, 1); //count of 1 as only 1 element is being updated

	if(eErrorCode == IEC61850_ERROR_NONE)
	{
		/* Set the Error String to NONE */
		char strError[SIZE_OF_ERROR_STRING] = {0};
		SetErrorString(strError, SIZE_OF_ERROR_STRING);

		/* If the Update Succeeded Update the local value for the gui*/
		SetMMXU2PhsCMagi(inUpdateValue);
	}
	else
	{
		/* If the Update failed alert the user */
		char strError[SIZE_OF_ERROR_STRING] = {0};
		snprintf(strError, SIZE_OF_ERROR_STRING, "MMXU2PhsCMagi Update failed:(%i) %s\n", eErrorCode, IEC61850_ErrorString(eErrorCode));
		SetErrorString(strError, SIZE_OF_ERROR_STRING);
	}

	return eErrorCode;
}



/******************************************************************************
*       UpdateMMXUAAMagf Function Definition
******************************************************************************/
/*!  \brief        This function performs an IEC61850 Update on the MMXUPhsCMagi with the value passed in
 *
 *   \ingroup       Callback Handlers
 *
 *   \param[in]     inUpdateValue      	An Float32 that holds the value that the MMXUPhsCMagi will be updated to
 *
 *   \return        IEC61850_ERROR_NONE on success
 *   \return        otherwise the relevant enum IEC61850_ErrorCodes on failure
 ******************************************************************************/
enum IEC61850_ErrorCodes UpdateMMXUAAMagf(Float32 inUpdateValue)
{
	enum IEC61850_ErrorCodes eErrorCode = IEC61850_ERROR_NONE;

	struct IEC61850_DataAttributeID_Generic updateDAID = {0}; //We use the Generic version as it is easier to assign the Data Attribute IDs
	struct IEC61850_DataAttributeData updateDAData = {0}; // Always initialize the structure to 0 to avoid Garbage data value

	IEC61850 myIEC61850Object = GetMyServerClient(); //Get the IEC61850 Structure

	/* Setup the Data Attribute ID (DAID) Struct */
	updateDAID.Generic_type = IEC61850_DAID_GENERIC;
	updateDAID.uiField1 = 6;
	updateDAID.uiField2 = 0;
	updateDAID.uiField3 = 0;
	updateDAID.uiField4 = 0;
	updateDAID.uiField5 = 0;

	/* Setup the Data Attribute Data Struct */
	updateDAData.uiBitLength = sizeof(Float32) * 8; //Set the size of the data in bits
	updateDAData.ucType = IEC61850_DATATYPE_FLOAT32; //set the type of the data
	updateDAData.pvData = &inUpdateValue; //Set a pointer to the data

	/* Perform the Update, note the IEC61850_DataAttributeID_Generic needs to be typecast to a standard struct IEC61850_DataAttributeID */
	eErrorCode = IEC61850_Update(myIEC61850Object, (struct IEC61850_DataAttributeID*)&updateDAID, &updateDAData, 1); //count of 1 as only 1 element is being updated

	if(eErrorCode == IEC61850_ERROR_NONE)
	{
		/* Set the Error String to NONE */
		char strError[SIZE_OF_ERROR_STRING] = {0};
		SetErrorString(strError, SIZE_OF_ERROR_STRING);

		/* If the Update Succeeded Update the local value for the gui*/
		SetMMXUAAMagf(inUpdateValue);
	}
	else
	{
		/* If the Update failed alert the user */
		char strError[SIZE_OF_ERROR_STRING] = {0};
		snprintf(strError, SIZE_OF_ERROR_STRING, "MMXUPhsCMagi Update failed:(%i) %s\n", eErrorCode, IEC61850_ErrorString(eErrorCode));
		SetErrorString(strError, SIZE_OF_ERROR_STRING);
	}

	return eErrorCode;
}



/******************************************************************************
*       UpdateMMXUAAMagf Function Definition
******************************************************************************/
/*!  \brief        This function performs an IEC61850 Update on the MMXUPhsCMagi with the value passed in
 *
 *   \ingroup       Callback Handlers
 *
 *   \param[in]     inUpdateValue      	An Float32 that holds the value that the MMXUPhsCMagi will be updated to
 *
 *   \return        IEC61850_ERROR_NONE on success
 *   \return        otherwise the relevant enum IEC61850_ErrorCodes on failure
 ******************************************************************************/
enum IEC61850_ErrorCodes UpdateMMXU2AAMagf(Float32 inUpdateValue)
{
	enum IEC61850_ErrorCodes eErrorCode = IEC61850_ERROR_NONE;

	struct IEC61850_DataAttributeID_Generic updateDAID = {0}; //We use the Generic version as it is easier to assign the Data Attribute IDs
	struct IEC61850_DataAttributeData updateDAData = {0}; // Always initialize the structure to 0 to avoid Garbage data value

	IEC61850 myIEC61850Object = GetMyServerClient(); //Get the IEC61850 Structure

	/* Setup the Data Attribute ID (DAID) Struct */
	updateDAID.Generic_type = IEC61850_DAID_GENERIC;
	updateDAID.uiField1 = 6;
	updateDAID.uiField2 = 1;
	updateDAID.uiField3 = 0;
	updateDAID.uiField4 = 0;
	updateDAID.uiField5 = 0;

	/* Setup the Data Attribute Data Struct */
	updateDAData.uiBitLength = sizeof(Float32) * 8; //Set the size of the data in bits
	updateDAData.ucType = IEC61850_DATATYPE_FLOAT32; //set the type of the data
	updateDAData.pvData = &inUpdateValue; //Set a pointer to the data

	/* Perform the Update, note the IEC61850_DataAttributeID_Generic needs to be typecast to a standard struct IEC61850_DataAttributeID */
	eErrorCode = IEC61850_Update(myIEC61850Object, (struct IEC61850_DataAttributeID*)&updateDAID, &updateDAData, 1); //count of 1 as only 1 element is being updated

	if(eErrorCode == IEC61850_ERROR_NONE)
	{
		/* Set the Error String to NONE */
		char strError[SIZE_OF_ERROR_STRING] = {0};
		SetErrorString(strError, SIZE_OF_ERROR_STRING);

		/* If the Update Succeeded Update the local value for the gui*/
		SetMMXU2AAMagf(inUpdateValue);
	}
	else
	{
		/* If the Update failed alert the user */
		char strError[SIZE_OF_ERROR_STRING] = {0};
		snprintf(strError, SIZE_OF_ERROR_STRING, "MMXU2AAMagi Update failed:(%i) %s\n", eErrorCode, IEC61850_ErrorString(eErrorCode));
		SetErrorString(strError, SIZE_OF_ERROR_STRING);
	}

	return eErrorCode;
}


/******************************************************************************
*       UpdateMMXUABMagf Function Definition
******************************************************************************/
/*!  \brief        This function performs an IEC61850 Update on the MMXUPhsCMagi with the value passed in
 *
 *   \ingroup       Callback Handlers
 *
 *   \param[in]     inUpdateValue      	An Float32 that holds the value that the MMXUPhsCMagi will be updated to
 *
 *   \return        IEC61850_ERROR_NONE on success
 *   \return        otherwise the relevant enum IEC61850_ErrorCodes on failure
 ******************************************************************************/
enum IEC61850_ErrorCodes UpdateMMXUABMagf(Float32 inUpdateValue)
{
	enum IEC61850_ErrorCodes eErrorCode = IEC61850_ERROR_NONE;

	struct IEC61850_DataAttributeID_Generic updateDAID = {0}; //We use the Generic version as it is easier to assign the Data Attribute IDs
	struct IEC61850_DataAttributeData updateDAData = {0}; // Always initialize the structure to 0 to avoid Garbage data value

	IEC61850 myIEC61850Object = GetMyServerClient(); //Get the IEC61850 Structure

	/* Setup the Data Attribute ID (DAID) Struct */
	updateDAID.Generic_type = IEC61850_DAID_GENERIC;
	updateDAID.uiField1 = 7;
	updateDAID.uiField2 = 0;
	updateDAID.uiField3 = 0;
	updateDAID.uiField4 = 0;
	updateDAID.uiField5 = 0;

	/* Setup the Data Attribute Data Struct */
	updateDAData.uiBitLength = sizeof(Float32) * 8; //Set the size of the data in bits
	updateDAData.ucType = IEC61850_DATATYPE_FLOAT32; //set the type of the data
	updateDAData.pvData = &inUpdateValue; //Set a pointer to the data

	/* Perform the Update, note the IEC61850_DataAttributeID_Generic needs to be typecast to a standard struct IEC61850_DataAttributeID */
	eErrorCode = IEC61850_Update(myIEC61850Object, (struct IEC61850_DataAttributeID*)&updateDAID, &updateDAData, 1); //count of 1 as only 1 element is being updated

	if(eErrorCode == IEC61850_ERROR_NONE)
	{
		/* Set the Error String to NONE */
		char strError[SIZE_OF_ERROR_STRING] = {0};
		SetErrorString(strError, SIZE_OF_ERROR_STRING);

		/* If the Update Succeeded Update the local value for the gui*/
		SetMMXUABMagf(inUpdateValue);
	}
	else
	{
		/* If the Update failed alert the user */
		char strError[SIZE_OF_ERROR_STRING] = {0};
		snprintf(strError, SIZE_OF_ERROR_STRING, "MMXUPhsCMagi Update failed:(%i) %s\n", eErrorCode, IEC61850_ErrorString(eErrorCode));
		SetErrorString(strError, SIZE_OF_ERROR_STRING);
	}

	return eErrorCode;
}

/******************************************************************************
*       UpdateMMXUABMagf Function Definition
******************************************************************************/
/*!  \brief        This function performs an IEC61850 Update on the MMXUPhsCMagi with the value passed in
 *
 *   \ingroup       Callback Handlers
 *
 *   \param[in]     inUpdateValue      	An Float32 that holds the value that the MMXUPhsCMagi will be updated to
 *
 *   \return        IEC61850_ERROR_NONE on success
 *   \return        otherwise the relevant enum IEC61850_ErrorCodes on failure
 ******************************************************************************/
enum IEC61850_ErrorCodes UpdateMMXU2ABMagf(Float32 inUpdateValue)
{
	enum IEC61850_ErrorCodes eErrorCode = IEC61850_ERROR_NONE;

	struct IEC61850_DataAttributeID_Generic updateDAID = {0}; //We use the Generic version as it is easier to assign the Data Attribute IDs
	struct IEC61850_DataAttributeData updateDAData = {0}; // Always initialize the structure to 0 to avoid Garbage data value

	IEC61850 myIEC61850Object = GetMyServerClient(); //Get the IEC61850 Structure

	/* Setup the Data Attribute ID (DAID) Struct */
	updateDAID.Generic_type = IEC61850_DAID_GENERIC;
	updateDAID.uiField1 = 7;
	updateDAID.uiField2 = 1;
	updateDAID.uiField3 = 0;
	updateDAID.uiField4 = 0;
	updateDAID.uiField5 = 0;

	/* Setup the Data Attribute Data Struct */
	updateDAData.uiBitLength = sizeof(Float32) * 8; //Set the size of the data in bits
	updateDAData.ucType = IEC61850_DATATYPE_FLOAT32; //set the type of the data
	updateDAData.pvData = &inUpdateValue; //Set a pointer to the data

	/* Perform the Update, note the IEC61850_DataAttributeID_Generic needs to be typecast to a standard struct IEC61850_DataAttributeID */
	eErrorCode = IEC61850_Update(myIEC61850Object, (struct IEC61850_DataAttributeID*)&updateDAID, &updateDAData, 1); //count of 1 as only 1 element is being updated

	if(eErrorCode == IEC61850_ERROR_NONE)
	{
		/* Set the Error String to NONE */
		char strError[SIZE_OF_ERROR_STRING] = {0};
		SetErrorString(strError, SIZE_OF_ERROR_STRING);

		/* If the Update Succeeded Update the local value for the gui*/
		SetMMXU2ABMagf(inUpdateValue);
	}
	else
	{
		/* If the Update failed alert the user */
		char strError[SIZE_OF_ERROR_STRING] = {0};
		snprintf(strError, SIZE_OF_ERROR_STRING, "MMXU2ABMagi Update failed:(%i) %s\n", eErrorCode, IEC61850_ErrorString(eErrorCode));
		SetErrorString(strError, SIZE_OF_ERROR_STRING);
	}

	return eErrorCode;
}


/******************************************************************************
*       UpdateMMXUACMagf Function Definition
******************************************************************************/
/*!  \brief        This function performs an IEC61850 Update on the MMXUPhsCMagi with the value passed in
 *
 *   \ingroup       Callback Handlers
 *
 *   \param[in]     inUpdateValue      	An Float32 that holds the value that the MMXUPhsCMagi will be updated to
 *
 *   \return        IEC61850_ERROR_NONE on success
 *   \return        otherwise the relevant enum IEC61850_ErrorCodes on failure
 ******************************************************************************/
enum IEC61850_ErrorCodes UpdateMMXUACMagf(Float32 inUpdateValue)
{
	enum IEC61850_ErrorCodes eErrorCode = IEC61850_ERROR_NONE;

	struct IEC61850_DataAttributeID_Generic updateDAID = {0}; //We use the Generic version as it is easier to assign the Data Attribute IDs
	struct IEC61850_DataAttributeData updateDAData = {0}; // Always initialize the structure to 0 to avoid Garbage data value

	IEC61850 myIEC61850Object = GetMyServerClient(); //Get the IEC61850 Structure

	/* Setup the Data Attribute ID (DAID) Struct */
	updateDAID.Generic_type = IEC61850_DAID_GENERIC;
	updateDAID.uiField1 = 8;
	updateDAID.uiField2 = 0;
	updateDAID.uiField3 = 0;
	updateDAID.uiField4 = 0;
	updateDAID.uiField5 = 0;

	/* Setup the Data Attribute Data Struct */
	updateDAData.uiBitLength = sizeof(Float32) * 8; //Set the size of the data in bits
	updateDAData.ucType = IEC61850_DATATYPE_FLOAT32; //set the type of the data
	updateDAData.pvData = &inUpdateValue; //Set a pointer to the data

	/* Perform the Update, note the IEC61850_DataAttributeID_Generic needs to be typecast to a standard struct IEC61850_DataAttributeID */
	eErrorCode = IEC61850_Update(myIEC61850Object, (struct IEC61850_DataAttributeID*)&updateDAID, &updateDAData, 1); //count of 1 as only 1 element is being updated

	if(eErrorCode == IEC61850_ERROR_NONE)
	{
		/* Set the Error String to NONE */
		char strError[SIZE_OF_ERROR_STRING] = {0};
		SetErrorString(strError, SIZE_OF_ERROR_STRING);

		/* If the Update Succeeded Update the local value for the gui*/
		SetMMXUACMagf(inUpdateValue);
	}
	else
	{
		/* If the Update failed alert the user */
		char strError[SIZE_OF_ERROR_STRING] = {0};
		snprintf(strError, SIZE_OF_ERROR_STRING, "MMXUPhsCMagi Update failed:(%i) %s\n", eErrorCode, IEC61850_ErrorString(eErrorCode));
		SetErrorString(strError, SIZE_OF_ERROR_STRING);
	}

	return eErrorCode;
}


/******************************************************************************
*       UpdateMMXUACMagf Function Definition
******************************************************************************/
/*!  \brief        This function performs an IEC61850 Update on the MMXUPhsCMagi with the value passed in
 *
 *   \ingroup       Callback Handlers
 *
 *   \param[in]     inUpdateValue      	An Float32 that holds the value that the MMXUPhsCMagi will be updated to
 *
 *   \return        IEC61850_ERROR_NONE on success
 *   \return        otherwise the relevant enum IEC61850_ErrorCodes on failure
 ******************************************************************************/
enum IEC61850_ErrorCodes UpdateMMXU2ACMagf(Float32 inUpdateValue)
{
	enum IEC61850_ErrorCodes eErrorCode = IEC61850_ERROR_NONE;

	struct IEC61850_DataAttributeID_Generic updateDAID = {0}; //We use the Generic version as it is easier to assign the Data Attribute IDs
	struct IEC61850_DataAttributeData updateDAData = {0}; // Always initialize the structure to 0 to avoid Garbage data value

	IEC61850 myIEC61850Object = GetMyServerClient(); //Get the IEC61850 Structure

	/* Setup the Data Attribute ID (DAID) Struct */
	updateDAID.Generic_type = IEC61850_DAID_GENERIC;
	updateDAID.uiField1 = 8;
	updateDAID.uiField2 = 1;
	updateDAID.uiField3 = 0;
	updateDAID.uiField4 = 0;
	updateDAID.uiField5 = 0;

	/* Setup the Data Attribute Data Struct */
	updateDAData.uiBitLength = sizeof(Float32) * 8; //Set the size of the data in bits
	updateDAData.ucType = IEC61850_DATATYPE_FLOAT32; //set the type of the data
	updateDAData.pvData = &inUpdateValue; //Set a pointer to the data

	/* Perform the Update, note the IEC61850_DataAttributeID_Generic needs to be typecast to a standard struct IEC61850_DataAttributeID */
	eErrorCode = IEC61850_Update(myIEC61850Object, (struct IEC61850_DataAttributeID*)&updateDAID, &updateDAData, 1); //count of 1 as only 1 element is being updated

	if(eErrorCode == IEC61850_ERROR_NONE)
	{
		/* Set the Error String to NONE */
		char strError[SIZE_OF_ERROR_STRING] = {0};
		SetErrorString(strError, SIZE_OF_ERROR_STRING);

		/* If the Update Succeeded Update the local value for the gui*/
		SetMMXU2ACMagf(inUpdateValue);
	}
	else
	{
		/* If the Update failed alert the user */
		char strError[SIZE_OF_ERROR_STRING] = {0};
		snprintf(strError, SIZE_OF_ERROR_STRING, "MMXU2ACMagi Update failed:(%i) %s\n", eErrorCode, IEC61850_ErrorString(eErrorCode));
		SetErrorString(strError, SIZE_OF_ERROR_STRING);
	}

	return eErrorCode;
}


/******************************************************************************
*       UpdatePDIFCrvPtsYVal Function Definition
******************************************************************************/
/*!  \brief        This function performs an IEC61850 Update on the PDIFCrvPtsYVal with the value passed in
 *
 *   \ingroup       Callback Handlers
 *
 *   \param[in]     inUpdateValue   A Float32 that holds the value that the element in PDIFCrvPtsYVal will be updated to
 *   \param[in]     inIndex	      	An Integer8 that holds the index of the element in the array to be updates
 *
 *   \return        IEC61850_ERROR_NONE on success
 *   \return        otherwise the relevant enum IEC61850_ErrorCodes on failure
 ******************************************************************************/
enum IEC61850_ErrorCodes UpdatePDIFCrvPts(CrvPts inUpdateValue, Integer8 inIndex)
{
	enum IEC61850_ErrorCodes eErrorCode = IEC61850_ERROR_NONE;

	struct IEC61850_DataAttributeID_Generic crvPtsArrayDAID; //We use the Generic version as it is easier to assign the Data Attribute IDs
	struct IEC61850_DataAttributeData CrvPtsArrayDAData; // Always initialize the structure to 0 to avoid Garbage data value
	struct IEC61850_DataAttributeData XYValDAData[NUM_CRVPTS_STRUCT_ELEMENTS]; // Always initialize the structure to 0 to avoid Garbage data value

	IEC61850 myIEC61850Object = GetMyServerClient(); //Get the IEC61850 Structure

   memset(&crvPtsArrayDAID, 0, sizeof(struct IEC61850_DataAttributeID_Generic));
   memset(&CrvPtsArrayDAData, 0, sizeof(struct IEC61850_DataAttributeData));
   memset(XYValDAData, 0, NUM_CRVPTS_STRUCT_ELEMENTS * sizeof(struct IEC61850_DataAttributeData));

	/* Setup the Data Attribute ID (DAID) Struct */
	crvPtsArrayDAID.Generic_type = IEC61850_DAID_GENERIC;
	crvPtsArrayDAID.uiField1 = 6;
	crvPtsArrayDAID.uiField2 = 0;
	crvPtsArrayDAID.uiField3 = 0;
	crvPtsArrayDAID.uiField4 = 0;
	crvPtsArrayDAID.uiField5 = 0;

	/* Setup the Data Attribute Data Struct */
	CrvPtsArrayDAData.iArrayIndex = inIndex; //Set the index to be updated
	CrvPtsArrayDAData.uiBitLength = NUM_CRVPTS_STRUCT_ELEMENTS; //Set the size of the Array being passed in. 2 elements in this case Xval and YVal
	CrvPtsArrayDAData.ucType = IEC61850_DATATYPE_ARRAY; //set the type of the data
	CrvPtsArrayDAData.pvData = XYValDAData; //Set a pointer to the data

	/* Setup the XVal */
	XYValDAData[0].ucType = IEC61850_DATATYPE_FLOAT32;
	XYValDAData[0].uiBitLength = sizeof(Float32)*8;
	XYValDAData[0].pvData = &(inUpdateValue.xVal);

	/* Setup the YVal */
	XYValDAData[1].ucType = IEC61850_DATATYPE_FLOAT32;
	XYValDAData[1].uiBitLength = sizeof(Float32)*8;
	XYValDAData[1].pvData = &(inUpdateValue.yVal);

	/* Perform the Update, note the IEC61850_DataAttributeID_Generic needs to be typecast to a standard struct IEC61850_DataAttributeID */
	eErrorCode = IEC61850_Update(myIEC61850Object, (struct IEC61850_DataAttributeID*)&crvPtsArrayDAID, &CrvPtsArrayDAData, 1); //count of 1 as only 1 element is being updated

	if(eErrorCode == IEC61850_ERROR_NONE)
	{
		/* Set the Error String to NONE */
		char strError[SIZE_OF_ERROR_STRING] = {0};
		SetErrorString(strError, SIZE_OF_ERROR_STRING);

		/* If the Update Succeeded Update the local value for the gui*/
		SetPDIFCrvPts(inIndex, inUpdateValue);
	}
	else
	{
		/* If the Update failed alert the user */
		char strError[SIZE_OF_ERROR_STRING] = {0};
		snprintf(strError, SIZE_OF_ERROR_STRING, "PDIFCrvPtsYVal[%d] Update failed:(%i) %s\n", inIndex, eErrorCode, IEC61850_ErrorString(eErrorCode));
		SetErrorString(strError, SIZE_OF_ERROR_STRING);
	}

	return eErrorCode;
}



/******************************************************************************
*       MMSGetConnectedServerList Function Definition
******************************************************************************/
/*!  \brief        This function gets the connected server list from IEC61850 and prints it
 *
 *   \ingroup       Callback Handlers
 *
 *   \return        IEC61850_ERROR_NONE on success
 *   \return        otherwise the relevant enum IEC61850_ErrorCodes on failure
 ******************************************************************************/
enum IEC61850_ErrorCodes MMSGetConnectedServerList()
{
	enum IEC61850_ErrorCodes eErrorCode = IEC61850_ERROR_NONE;
	IEC61850 myIEC61850Object = GetMyServerClient(); //Get the IEC61850 Structure
	struct ConnectedServerList *connectedServerList = NULL; //create a structure pointer for the Connected Server List

	/* Request the list from the server, passing in a pointer to the above pointer. */
	eErrorCode = IEC61850_GetConnectedServerList(myIEC61850Object, &connectedServerList);
	if(eErrorCode == IEC61850_ERROR_NONE)
	{
		/* Set the Error String to NONE */
		char strError[SIZE_OF_ERROR_STRING] = {0};
		Unsigned32 u32Count = 0;
		/* Set error message to show success */
		snprintf(strError, SIZE_OF_ERROR_STRING, "Get connected server list success.");
		SetErrorString(strError, SIZE_OF_ERROR_STRING);

		/* Print the list */
		printf("\n\tConnected Servers (%u):\n", connectedServerList->u16NumberOfConnectedServers);
		printf("\tIndex\tDomain Name\t\tIP Address\tConnection Status\n");
		for(u32Count = 0; u32Count < connectedServerList->u16NumberOfConnectedServers; u32Count++)
		{
			printf("\t%u\t%s\t%s\t%s\n",
			connectedServerList->ptConnectedServers[u32Count].uiAAIndex,
			connectedServerList->ptConnectedServers[u32Count].ptDomainName,
			connectedServerList->ptConnectedServers[u32Count].ptIPAddress,
			connectedServerList->ptConnectedServers[u32Count].bConnectionState ? "Connected" : "Disconnected"); //status 2 is connected
		}
		/* Wait for user */
		printf("\nPress enter to continue.");
		getchar();
	}
	else
	{
		/* If the function failed alert the user */
		char strError[SIZE_OF_ERROR_STRING] = {0};
		snprintf(strError, SIZE_OF_ERROR_STRING, "Get connected server list failed: %i: %s\n", eErrorCode, IEC61850_ErrorString(eErrorCode)==NULL?"MMS Error":IEC61850_ErrorString(eErrorCode));
		SetErrorString(strError, SIZE_OF_ERROR_STRING);
	}
	return eErrorCode;
}

/******************************************************************************
*       MMSGetFileAttribs Function Definition
******************************************************************************/
/*!  \brief			This function get the connected server list from IEC61850 and prints it
 *					The function then prompts to selected a server index and files name.
 *					The function the gets the file attributes from the server.
 *
 *   \ingroup       Callback Handlers
 *
 *   \return        IEC61850_ERROR_NONE on success
 *   \return        otherwise the relevant enum IEC61850_ErrorCodes on failure
 ******************************************************************************/
enum IEC61850_ErrorCodes MMSGetFileAttribs()
{
	enum IEC61850_ErrorCodes eErrorCode = IEC61850_ERROR_NONE;
	struct tFileAttr ptFileAttributes = {0}; //tFileAttr to hold the returned attributes
	struct tFile *sourceFile = (struct tFile *) calloc(1, sizeof(struct tFile )); //tFile to pass to server to select file or directory
	IEC61850 myIEC61850Object = GetMyServerClient(); //Get the IEC61850 Structure
	struct ConnectedServerList *connectedServerList = NULL; //create a structure pointer for the Connected Server List
	unsigned int fileCount = 0; //file count
	unsigned int uiServerIndex = 0; //selected server index

	eErrorCode = IEC61850_GetConnectedServerList(myIEC61850Object, &connectedServerList); //get the connected server list
	if(eErrorCode == IEC61850_ERROR_NONE)
	{
		printServerList(connectedServerList); //print the connected server list
		uiServerIndex = GetServerIndexFromUser(); //prompt to select index
		/* prompt for file or directory */
		GetFileNameFromUser("Enter a file name or directory (blank will list the current directory): ", sourceFile->cFileName);

		/* populate requested file structure  */
		ptFileAttributes.ptArrayofDirectoryEntries = sourceFile;
		ptFileAttributes.uiSizeOfArray = 0;
		ptFileAttributes.u32NumOfDirectoryEntries = 0;
		/* request the attributes */
		eErrorCode = IEC61850_GetFileAttributeValues(myIEC61850Object, uiServerIndex, &ptFileAttributes);

		if(eErrorCode == IEC61850_ERROR_NONE)
		{
			/* Set the Error String to NONE */
			char strError[SIZE_OF_ERROR_STRING] = {0};
			snprintf(strError, SIZE_OF_ERROR_STRING, "Get File attributes success.");
			SetErrorString(strError, SIZE_OF_ERROR_STRING);

			/* Print list of files and attributes */
			printf("\n\n\tReceived attributes for %d file(s):\n", ptFileAttributes.u32NumOfDirectoryEntries);
			printf("\tFile\tTime Stamp\tSize [Bytes]\tFile Name\n");
			for(fileCount = 0; fileCount < ptFileAttributes.u32NumOfDirectoryEntries; fileCount++)
			{
				printf("\t%d:\t%s\t%d\t\t%s\n", fileCount + 1, ptFileAttributes.ptArrayofDirectoryEntries[fileCount].ctLastModified, ptFileAttributes.ptArrayofDirectoryEntries[fileCount].iFileSize, ptFileAttributes.ptArrayofDirectoryEntries[fileCount].cFileName);
			}

			/* Wait for user */
			printf("\nPress enter to continue.");
			getchar();
		}
		else
		{
			/* If the function failed alert the user */
			char strError[SIZE_OF_ERROR_STRING] = {0};
			snprintf(strError, SIZE_OF_ERROR_STRING, "Get File attributes failed: %i: %s\n", eErrorCode, IEC61850_ErrorString(eErrorCode)==NULL?"MMS Error":IEC61850_ErrorString(eErrorCode));
			SetErrorString(strError, SIZE_OF_ERROR_STRING);
		}
	}
	else
	{
		/* If the function failed alert the user */
		char strError[SIZE_OF_ERROR_STRING] = {0};
		snprintf(strError, SIZE_OF_ERROR_STRING, "Unable to get connected server list: %i: %s\n", eErrorCode, IEC61850_ErrorString(eErrorCode)==NULL?"MMS Error":IEC61850_ErrorString(eErrorCode));
		SetErrorString(strError, SIZE_OF_ERROR_STRING);
	}
	free(sourceFile);
	return eErrorCode;
}

/******************************************************************************
*       GetServerIndexFromUser Function Definition
******************************************************************************/
/*!  \brief         This function Gets an unsigned int index from the user
 *
 *   \ingroup       User Input
 *
 *
 *   \return        The unsigned int value of the input supplied by the user
 ******************************************************************************/
unsigned int GetServerIndexFromUser()
{
	unsigned int returnValue = 0xFFFF;

	printf("Enter index of the connected server: ");
	scanf("%u", &returnValue);

	return returnValue;
}

/******************************************************************************
*       GetFileNameFromUser Function Definition
******************************************************************************/
/*!  \brief         This function gets a string from the user
 *
 *   \ingroup       User Input
 *
 *   \param[in]     messageString	A pointer to the char array containing the message to be displayed
 *   \param[in]     userInput		A pointer to the char array to be populated by the file name. Should have 256 elements.
 *
 *   \return        The unsigned int value of the input supplied by the user
 ******************************************************************************/
void GetFileNameFromUser(char * messageString, char * userInput)
{
	char line[256] = {0};

	printf("%s", messageString); //print the message

	fflush(stdin); //clear any input
	fgets(line, 256, stdin); //get use input string
	sscanf(line, "%255s", userInput); //get string from input
}

/******************************************************************************
*       PrintServerList Function Definition
******************************************************************************/
/*!  \brief         This function prints the data in a ConnectedServerList struct
 *
 *   \ingroup
 *
 *   \param[in]     connectedServerList   A pointer to the populated ConnectedServerList struct
 *
 *   \return        void
 ******************************************************************************/
void printServerList(struct ConnectedServerList *connectedServerList)
{
	unsigned int u32Count = 0;
	printf("\n\tConnected Servers (%u):\n", connectedServerList->u16NumberOfConnectedServers);
	printf("\tIndex\tDomain Name\t\tIP Address\tConnection Status\n");
	for(u32Count = 0; u32Count < connectedServerList->u16NumberOfConnectedServers; u32Count++)
	{
		printf("\t%u\t%s\t%s\t%s\n",
			connectedServerList->ptConnectedServers[u32Count].uiAAIndex,
			connectedServerList->ptConnectedServers[u32Count].ptDomainName,
			connectedServerList->ptConnectedServers[u32Count].ptIPAddress,
			connectedServerList->ptConnectedServers[u32Count].bConnectionState ? "Connected" : "Disconnected"); //status 2 is connected
	}
	printf("\n");
}



/******************************************************************************
*       MMSGetFile Function Definition
******************************************************************************/
/*!  \brief			This function downloads a file from a connected server
 *					The function prompts to selected a server index, remote file name and local file name.
 *
 *   \ingroup       Callback Handlers
 *
 *   \return        IEC61850_ERROR_NONE on success
 *   \return        otherwise the relevant enum IEC61850_ErrorCodes on failure
 ******************************************************************************/
enum IEC61850_ErrorCodes MMSGetFile()
{
	enum IEC61850_ErrorCodes eErrorCode = IEC61850_ERROR_NONE;
	IEC61850 myIEC61850Object = GetMyServerClient(); //Get the IEC61850 Structure
	struct ConnectedServerList *connectedServerList = NULL;
	unsigned int uiServerIndex = 0;
	char remoteFileName[256] = {0};
	char localFileName[256] = {0};
	eErrorCode = IEC61850_GetConnectedServerList(myIEC61850Object, &connectedServerList);
	if(eErrorCode == IEC61850_ERROR_NONE)
	{
		printServerList(connectedServerList);
		uiServerIndex = GetServerIndexFromUser();
		GetFileNameFromUser("Enter the remote source file name: ", remoteFileName);
		GetFileNameFromUser("Enter the local destination file name: ", localFileName);

		eErrorCode = IEC61850_GetFile(myIEC61850Object, uiServerIndex, remoteFileName, localFileName);
		if(eErrorCode == IEC61850_ERROR_NONE)
		{
			/* Set the Error String to NONE */
			char strError[SIZE_OF_ERROR_STRING] = {0};
			snprintf(strError, SIZE_OF_ERROR_STRING, "Get File success.");
			SetErrorString(strError, SIZE_OF_ERROR_STRING);
		}
	}
	if(eErrorCode != IEC61850_ERROR_NONE)
	{
		/* If the function failed alert the user */
		char strError[SIZE_OF_ERROR_STRING] = {0};
		snprintf(strError, SIZE_OF_ERROR_STRING, "Get File failed: %i: %s\n", eErrorCode, IEC61850_ErrorString(eErrorCode)==NULL?"MMS Error":IEC61850_ErrorString(eErrorCode));
		SetErrorString(strError, SIZE_OF_ERROR_STRING);
	}
	return eErrorCode;
}



/******************************************************************************
*       MMSDeleteFile Function Definition
******************************************************************************/
/*!  \brief			This function deletes a file from a connected server.
 *					The function prompts to selected a server index and remote file name for deletion.
 *
 *   \ingroup       Callback Handlers
 *
 *   \return        IEC61850_ERROR_NONE on success
 *   \return        otherwise the relevant enum IEC61850_ErrorCodes on failure
 ******************************************************************************/
enum IEC61850_ErrorCodes MMSDeleteFile()
{
	enum IEC61850_ErrorCodes eErrorCode = IEC61850_ERROR_NONE;
	IEC61850 myIEC61850Object = GetMyServerClient(); //Get the IEC61850 Structure
	struct ConnectedServerList *connectedServerList = NULL;
	unsigned int uiServerIndex = 0;
	char remoteFileName[256] = {0};
	eErrorCode = IEC61850_GetConnectedServerList(myIEC61850Object, &connectedServerList);
	if(eErrorCode == IEC61850_ERROR_NONE)
	{
		printServerList(connectedServerList);
		uiServerIndex = GetServerIndexFromUser();
		GetFileNameFromUser("Enter the name of the remote file you wish to delete: ", remoteFileName);

		/* send delete file command */
		eErrorCode = IEC61850_DeleteFile(myIEC61850Object, uiServerIndex, remoteFileName);
		if(eErrorCode == IEC61850_ERROR_NONE)
		{
			/* Set the Error String to NONE */
			char strError[SIZE_OF_ERROR_STRING] = {0};
			snprintf(strError, SIZE_OF_ERROR_STRING, "Delete File success.");
			SetErrorString(strError, SIZE_OF_ERROR_STRING);
		}
	}
	if(eErrorCode != IEC61850_ERROR_NONE)
	{
		/* If the function failed alert the user */
		char strError[SIZE_OF_ERROR_STRING] = {0};
		snprintf(strError, SIZE_OF_ERROR_STRING, "Delete File failed: %i: %s\n", eErrorCode, IEC61850_ErrorString(eErrorCode)==NULL?"MMS Error":IEC61850_ErrorString(eErrorCode));
		SetErrorString(strError, SIZE_OF_ERROR_STRING);
	}
	return eErrorCode;
}
