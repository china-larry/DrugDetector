#ifndef _IQAK_H_
#define _IQAK_H_

#ifdef WIN32
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif
#endif

#include "IObjectBase.h"

/// Represents an HL7 QAK message segment. 
/// This segment has the following fields:<ol>
///<li>QAK-1: Query Tag (ST)</li>
///<li>QAK-2: Query Response Status (ID)</li>
///<li>QAK-3: Message Query Name (CE), Empty and Keep back</li>
///<li>QAK-4: Hit Count Total (NM), Empty and Keep back</li>
///<li>QAK-5: This payload (NM), Empty and Keep back</li>
///<li>QAK-6: Hits remaining (NM), Empty and Keep back</li>


struct IQAK : public IObjectBase
{
	/*
	*	\brief 查询标记符,index 0,取SR（Sample Request)
	*/
	virtual void SetQueryTag(const char* queryTag) = 0;

	virtual void GetQueryTag(char** queryTag) = 0;

	/*
	*	\brief 查询回应状况,index 1,可取值(OK/NF/AE/AR)
	*/
	virtual void SetQueryResponseStatus(const char* queryResponseStatus) = 0;

	virtual void GetQueryResponseStatus(char** queryResponseStatus) = 0;

};

#ifdef WIN32
HL7_ENGINE_DLL_API IQAK* CreateQAK();
#else
IQAK* CreateQAK();
#endif

#endif