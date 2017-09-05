/*
* IESR_U02.h
*
*  Created on: 2015年11月9日
*      Author: maggie
*/
/// Represents a ESR_U02 message structure (see chapter 13). This structure contains the 
/// following elements:
///<ol>
///<li>0: MSH (Message Header) </li>
///<li>1: EQU (Equipment Detail) </li>
///<li>2: ROL (Role) optional </li>

#ifndef IESR_U02_H
#define IESR_U02_H
#ifdef WIN32
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif
#endif


#include "IObjectBase.h"
#include "IMSH.h"
#include "IEQU.h"
#include "IROL.h"

class IESR_U02 : public IObjectBase
{
public:

	virtual void SetMSH(IMSH* msh) = 0;

	virtual void SetEQU(IEQU* equ) = 0;

	virtual void SetROL(IROL* rol) = 0;


	virtual bool GetMSH(IMSH* msh) = 0;

	virtual bool GetEQU(IEQU* equ) = 0;

	virtual bool GetROL(IROL* rol) = 0;

};


#ifdef WIN32
HL7_ENGINE_DLL_API IESR_U02* CreateEsrU02();
#else
IESR_U02* CreateEsrU02();
#endif

#endif


