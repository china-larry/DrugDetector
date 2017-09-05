/*
* IEAR_U08_COMMAND_RESPONSE.h
*
*  Created on: 2015年11月9日
*      Author: maggie
*/
///Represents the EAR_U08_COMMAND_RESPONSE Group.  A Group is an ordered collection of message 
/// segments that can repeat together or be optionally in/excluded together.
/// This Group contains the following elements: 
///<li>0: ECD (Equipment Command) </li>
///<li>1: ECR (Equipment Command Response) </li>

#ifndef _IEAR_U08_COMMAND_RESPONSE_H_
#define _IEAR_U08_COMMAND_RESPONSE_H_

#include "IECD.h"
#include "IECR.h"
#include "IObjectList.h"

struct IEAR_U08_COMMAND_RESPONSE : public IObjectBase
{
	virtual bool GetECD(IECD* ecd) = 0;

	virtual void SetECD(IECD* ecd) = 0;

	virtual bool GetECR(IECR* ecr) = 0;

	virtual void SetECR(IECR* ecr) = 0;

	virtual void GetString(char** messageString) = 0;
};

#ifdef WIN32
HL7_ENGINE_DLL_API IEAR_U08_COMMAND_RESPONSE* CreateEarU08CommandResponse();
#else
IEAR_U08_COMMAND_RESPONSE* CreateEarU08CommandResponse();
#endif

#endif /* _IEAR_U08_COMMAND_RESPONSE_H_ */
