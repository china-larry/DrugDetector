#ifndef _IECD_H_
#define _IECD_H_

#ifdef WIN32
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif
#endif

#include "IObjectBase.h"

/// Represents an HL7 ECD message segment. 
/// This segment has the following fields:<ol>
///<li>ECD-1: Reference Command Number (NM)</li>
///<li>ECD-2: Remote Control Command (CE)</li>
///<li>ECD-3: Response Required (ID)</li>
///<li>ECD-4: Requested Completion Time (TQ)</li>
///<li>ECD-5: Parameters (ST)</li>
///</ol>


struct IECD : public IObjectBase
{
	/*
	*	\brief 参考命令号,index 0
	*/
	virtual void SetRefCommandNum(const char* refCommandNum) = 0;

	virtual void GetRefCommandNum(char** refCommandNum) = 0;

	/*
	*	\brief 远程控制命令ID,index 1
	*/
	virtual void SetRemoteControlCommandID(const char* remoteControlCommandID) = 0;

	virtual void GetRemoteControlCommandID(char** remoteControlCommandID) = 0;

	/*
	*	\brief 远程控制命令Text,index 1
	*/
	virtual void SetRemoteControlCommandText(const char* remoteControlCommandText) = 0;

	virtual void GetRemoteControlCommandText(char** remoteControlCommandText) = 0;


	/*
	*	\brief 必要响应,index 2
	*/
	virtual void SetResponseRequire(const char* responseRequire) = 0;

	virtual void GetResponseRequire(char** responseRequire) = 0;

	/*
	*	\brief 要求完成时间，开始日期,index 3
	*/
	virtual void SetRequestedCompletionStartTime(const char* startTime) = 0;

	virtual void GetRequestedCompletionStartTime(char** startTime) = 0;

	/*
	*	\brief 要求完成时间，结束日期,index 3
	*/
	virtual void SetRequestedCompletionEndTime(const char* endTime) = 0;

	virtual void GetRequestedCompletionEndTime(char** endTime) = 0;

	/*
	*	\brief 远程控制命令参数,index 4
	*/
	virtual void SetParameters(const char* parameters) = 0;

	virtual void GetParameters(char** parameters) = 0;

};

#ifdef WIN32
HL7_ENGINE_DLL_API IECD* CreateECD();
#else
IECD* CreateECD();
#endif

#endif