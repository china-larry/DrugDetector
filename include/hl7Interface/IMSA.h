#ifndef IMSA_H
#define IMSA_H
#ifdef WIN32
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif
#endif


#include "IObjectBase.h"
class IMSA : public IObjectBase
{
public:
	/*
	*	\brief 确认代码,index 0
	*/
	virtual void SetACKCode(const char* ackCodeStr) = 0;

	virtual void GetACKCode(char** ackCodeStr) = 0;
	/*
	*	\brief 信息控制ID,index 1
	*/
	virtual void SetMsgCtrlID(const char* msgCtrlIDStr) = 0;

	virtual void GetMsgCtrlID(char** msgCtrlIDStr) = 0;

	/*
	*	\brief 文本信息, index 2
	*/
	virtual void SetTextMessage(const char* textMsgCtrl) = 0;

	virtual void GetTextMessage(char** textMsgCtrl) = 0;

	/*
	*	\brief 期望系列号，index 3，目前保留置空
	*/
	virtual void SetExpectSequenceNum(const char* expectSequenceNum) = 0;

	virtual void GetExpectSequenceNum(char** expectSequenceNum) = 0;
	/*
	*	\brief 延迟确认类型，index 4, 目前保留置空
	*/
	virtual void SetDelayACKType(const char* delayAckTypeStr) = 0;

	virtual void GetDelayACKType(char** delayAckTypeStr) = 0;

	/*
	*	\brief 错误情况,index 5
	*/
	virtual void SetErrorCondition(const char* errorCondition) = 0;

	virtual void GetErrorCondition(char** errorCondition) = 0;
};


#ifdef WIN32
HL7_ENGINE_DLL_API IMSA* CreateMSA();
#else
IMSA* CreateMSA();
#endif

#endif