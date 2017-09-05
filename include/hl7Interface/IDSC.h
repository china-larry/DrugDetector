/*
* IOBX.h
*
*  Created on: 2015年10月9日
*      Author: root
*/

#ifndef _IDSC_H_
#define _IDSC_H_


#ifdef WIN32
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif
#endif

#include "IObjectBase.h"
enum ContinueFormat
{
	Fragmentation,//分割式连续
	Interactive_Continuation//交互式连续
};
struct IDSC : public IObjectBase
{
	/*
	*	\brief 连续指针,index 0
	*/
	virtual void SetContinuePointer(const char* continuePointerStr)= 0;

	virtual void GetContinuePointer(char** continuePointerStr)= 0;

	/*
	*	\brief 连续格式,index 1
	*/
	virtual void SetContinueFormat(ContinueFormat continueFormat)= 0;

	virtual void GetContinueFormat(char** continueFormat)= 0;
};

#ifdef WIN32
HL7_ENGINE_DLL_API IDSC* CreateDSC();
#else
IDSC* CreateDSC();
#endif

#endif /* INTERFACE_IOBX_H_ */