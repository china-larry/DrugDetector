#ifndef _IEQP_H_
#define _IEQP_H_

#ifdef WIN32
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif
#endif

#include "IObjectBase.h"

/// Represents an HL7 EQP message segment. 
/// This segment has the following fields:
///<li>EQP-1: Event type (CE)</li>
///<li>EQP-2: File Name (ST)</li>
///<li>EQP-3: Start Date/Time (ST)</li>
///<li>EQP-4: End Date/Time (ST)</li>
///<li>EQP-5: Transaction Data (ST)</li>


struct IEQP : public IObjectBase
{
	/*
	*	\brief 事件类型ID,index 0
	*/
	virtual void SetEventTypeID(const char* eventTypeID) = 0;

	virtual void GetEventTypeID(char** eventTypeID) = 0;

	/*
	*	\brief 事件类型文本描述,index 0
	*/
	virtual void SetEventTypeText(const char* eventTypeText) = 0;

	virtual void GetEventTypeText(char** eventTypeText) = 0;

	/*
	*	\brief 事件ID,index 1
	*/
	virtual void SetEventID(const char* eventID) = 0;

	virtual void GetEventID(char** eventID) = 0;


	/*
	*	\brief 事件开始时间,index 2
	*/
	virtual void SetEventStartTime(const char* startTime) = 0;

	virtual void GetEventStartTime(char** startTime) = 0;


	/*
	*	\brief 事件结束时间,index 3
	*/
	virtual void SetEventEndTime(const char* endTime) = 0;

	virtual void GetEventEndTime(char** endTime) = 0;



	/*
	*	\brief 事件信息,index 4
	*/
	virtual void SetEventData(const char* data) = 0;

	virtual void GetEventData(char** data) = 0;
};

#ifdef WIN32
HL7_ENGINE_DLL_API IEQP* CreateEQP();
#else
IEQP* CreateEQP();
#endif

#endif