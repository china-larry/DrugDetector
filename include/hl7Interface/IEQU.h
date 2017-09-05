#ifndef _IEQU_H_
#define _IEQU_H_

#ifdef WIN32
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif
#endif

#include "IObjectBase.h"

/// Represents an HL7 EQU message segment. 
/// This segment has the following fields:<ol>
///<li>EQU-1: Equipment Instance Identifier (EI)</li>
///<li>EQU-2: Event Date/Time (ST)</li>
///<li>EQU-3: Equipment State (CE)</li>
///<li>EQU-4: Local/Remote Control State (CE)</li>
///<li>EQU-5: Alert Level (CE)</li>


struct IEQU : public IObjectBase
{
	/*
	*	\brief 设备实例标识符_设备标识符,index 0
	*/
	virtual void SetDeviceID(const char* deviceID) = 0;

	virtual void GetDeviceID(char** deviceID) = 0;

	/*
	*	\brief 设备实例标识符_设备名称,index 0
	*/
	virtual void SetDeviceName(const char* deviceName) = 0;

	virtual void GetDeviceName(char** deviceName) = 0;

	/*
	*	\brief 设备实例标识符_机构ID,index 0
	*/
	virtual void SetUniversalID(const char* universalID) = 0;

	virtual void GetUniversalID(char** universalID) = 0;

	/*
	*	\brief 发生时间,index 1
	*/
	virtual void SetEventDateTime(const char* eventDateTime) = 0;

	virtual void GetEventDateTime(char** eventDateTime) = 0;


	/*
	*	\brief 设备状态ID,index 2
	*/
	virtual void SetDeviceStateID(const char* stateID) = 0;

	virtual void GetDeviceStateID(char** stateID) = 0;

	/*
	*	\brief 设备状态文本描述,index 2
	*/
	virtual void SetDeviceStateText(const char* stateText) = 0;

	virtual void GetDeviceStateText(char** stateText) = 0;
	/*
	*	\brief 本地远程控制情况ID,index 3
	*/
	virtual void SetControlStateID(const char* controlStateID) = 0;

	virtual void GetControlStateID(char** controlStateID) = 0;

	/*
	*	\brief 本地远程控制情况文本描述,index 3
	*/
	virtual void SetControlStateText(const char* controlStateText) = 0;

	virtual void GetControlStateText(char** controlStateText) = 0;

	/*
	*	\brief Code卡版本号,index 3
	*/
	virtual void SetCodeVersionText(const char* codeVersion) = 0;

	virtual void GetCodeVersionText(char** codeVersion) = 0;

	/*
	*	\brief 程序版本号,index 3
	*/
	virtual void SetAppVersionText(const char* appVersion) = 0;

	virtual void GetAppVersionText(char** appVersion) = 0;

	/*
	*	\brief 仪器总测试次数
	*/
	virtual void SetTestCountText(const char* testCountText) = 0;

	virtual void GetTestCountText(char** testCountText) = 0;

	/*
	*	\brief [仪器测试项目,测试次数;],index 3
	*/
	virtual void SetSubProjectTestCountText(const char* testCountText) = 0;

	virtual void GetSubProjectTestCountText(char** testCountText) = 0;
	/*
	*	\brief 告警级别ID,index 4
	*/
	virtual void SetAlertLevelID(const char* alertLevelID) = 0;

	virtual void GetAlertLevelID(char** alertLevelID) = 0;

	/*
	*	\brief 告警级别文本描述,index 4
	*/
	virtual void SetAlertLevelText(const char* alertLevelText) = 0;

	virtual void GetAlertLevelText(char** alertLevelText) = 0;
};

#ifdef WIN32
HL7_ENGINE_DLL_API IEQU* CreateEQU();
#else
IEQU* CreateEQU();
#endif

#endif