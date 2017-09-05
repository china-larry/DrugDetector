#ifndef IHL7MESSAGE_H
#define IHL7MESSAGE_H

#ifdef WIN32
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif
#endif

#include "IObjectBase.h"
#include "base.h"
#include "IObjectList.h"

struct IHL7Message : public IObjectBase
{
	virtual NRET GetAllSegment(IObjectList* objectList) = 0;

	/*
	*@brief 返回消息字符串
	*/
	virtual void GetMessageString(char** messageString) = 0;
	/*
	*@brief 构建消息，输入参数为按顺序排列的字段集合
	*/
	virtual void Build(IObjectList* objectList) = 0;
	/*
	*@brief 分解消息为段
	*/
	virtual void Parse(const char* messageStr) = 0;
	/*
	 *	\brief 返回消息状态
	 */
	virtual MessageState GetMessageState() = 0;
};

#ifdef WIN32
HL7_ENGINE_DLL_API IHL7Message* CreateMessage();
#else
IHL7Message* CreateMessage();
#endif

#endif

