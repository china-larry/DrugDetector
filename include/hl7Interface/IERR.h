#ifndef IERR_H
#define IERR_H
#ifdef WIN32
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif
#endif


#include "IObjectBase.h"
class IERR : public IObjectBase
{
public:
	/*
	*	\brief 错误代码,index 0
	*/
	virtual void SetErrorCode(const char* errorCodeStr) = 0;

	virtual void GetErrorCode(char** errorCodeStr) =0;
};


#ifdef WIN32
HL7_ENGINE_DLL_API IERR* CreateERR();
#else
IERR* CreateERR();
#endif

#endif