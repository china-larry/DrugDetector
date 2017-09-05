#ifndef _IECR_H_
#define _IECR_H_

#ifdef WIN32
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif
#endif

#include "IObjectBase.h"

/// Represents an HL7 ECR message segment. 
/// This segment has the following fields:<ol>
///<li>ECR-1: Command Response (CE)</li>
///<li>ECR-2: Date/Time Completed (TS)</li>
///<li>ECR-3: Command Response Parameters (ST)</li>


struct IECR : public IObjectBase
{
	/*
	*	\brief 命令响应ID,index 0
	*/
	virtual void SetCommandResponseID(const char* commandResponseID) = 0;

	virtual void GetCommandResponseID(char** commandResponseID) = 0;

	/*
	*	\brief 命令响应ID文本描述,index 0
	*/
	virtual void SetCommandResponseText(const char* commandResponseText) = 0;

	virtual void GetCommandResponseText(char** commandResponseText) = 0;

	/*
	*	\brief 完成命令时间,index 1
	*/
	virtual void SetDateTimeCompleted(const char* dateTimeComplete) = 0;

	virtual void GetDateTimeCompleted(char** dateTimeComplete) = 0;


	/*
	*	\brief 命令响应返回消息,index 2
	*/
	virtual void SetCommandResponseParameters(const char* parameter) = 0;

	virtual void GetCommandResponseParameters(char** parameter) = 0;

};

#ifdef WIN32
HL7_ENGINE_DLL_API IECR* CreateECR();
#else
IECR* CreateECR();
#endif

#endif