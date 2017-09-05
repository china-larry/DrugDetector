#ifndef _IISD_H_
#define _IISD_H_

#ifdef WIN32
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif
#endif

#include "IObjectBase.h"

/// Represents an HL7 ISD message segment. 
/// This segment has the following fields:<ol>
///<li>ISD-1: Reference Interaction Number (unique identifier) (NM)</li>
///<li>ISD-2: Interaction Type Identifier (CE)</li>
///<li>ISD-3: Interaction Active State (CE)</li>


struct IISD : public IObjectBase
{
	/*
	*	\brief 相互作用码,index 0
	*/
	virtual void SetRefInteractionNum(const char* refInteractionNum) = 0;

	virtual void GetRefInteractionNum(char** refInteractionNum) = 0;

	/*
	*	\brief 相互作用类型ID,index 1
	*/
	virtual void SetInteractionTypeID(const char* interactionTypeID) = 0;

	virtual void GetInteractionTypeID(char** interactionTypeID) = 0;

	/*
	*	\brief 相互作用类型描述文本,index 1
	*/
	virtual void SetInteractionTypeText(const char* interactionTypeText) = 0;

	virtual void GetInteractionTypeText(char** interactionTypeText) = 0;

	/*
	*	\brief 相互作用活动状态ID,index 2
	*/
	virtual void SetInteractionActiveStateID(const char* interactionActiveStateID) = 0;

	virtual void GetInteractionActiveStateID(char** interactionActiveStateID) = 0;

	/*
	*	\brief 相互作用活动状态文本描述,index 2
	*/
	virtual void SetInteractionActiveStateText(const char* interactionActiveStateText) = 0;

	virtual void GetInteractionActiveStateText(char** interactionActiveStateText) = 0;
};

#ifdef WIN32
HL7_ENGINE_DLL_API IISD* CreateISD();
#else
IISD* CreateISD();
#endif

#endif