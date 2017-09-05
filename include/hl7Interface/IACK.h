/*
* IORU_R01.h
*
*  Created on: 2015年10月8日
*      Author: maggie
*/

#ifndef _IACK_H_
#define _IACK_H_

#ifdef WIN32
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif
#endif


#include "IObjectBase.h"
#include "IMSH.h"
#include "IMSA.h"
#include "IERR.h"

/***ACK消息结构**
* MSH
* MSA
* [ERR]
**/
class IACK : public IObjectBase
{
public:
	virtual void SetMSH(IMSH* msh) = 0;
	virtual bool GetMSH(IMSH* msh) = 0;

	virtual void SetMSA(IMSA* msa) = 0;
	virtual bool GetMSA(IMSA* msa) = 0;

	virtual void SetERR(IERR* err) = 0;
	virtual bool GetERR(IERR* err) = 0;
};


#ifdef WIN32
HL7_ENGINE_DLL_API IACK* CreateACK();
#else
IACK* CreateACK();
#endif

#endif
