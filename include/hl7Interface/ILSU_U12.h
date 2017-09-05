/*
* ILSU_U12.h
*
*  Created on: 2015年11月9日
*      Author: maggie
*/
/// Represents a LSU_U12 message structure (see chapter 13). This structure contains the 
/// following elements:
///<ol>
///<li>0: MSH (Message Header) </li>
///<li>1: EQU (Equipment Detail) </li>
///<li>2: EQP (Equipment/log Service) repeating</li>
///<li>3: ROL (Role) optional </li>

#ifndef ILSU_U12_H
#define ILSU_U12_H
#ifdef WIN32
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif
#endif


#include "IObjectBase.h"
#include "IMSH.h"
#include "IEQU.h"
#include "IEQP.h"
#include "IROL.h"

class ILSU_U12 : public IObjectBase
{
public:

	virtual void SetMSH(IMSH* msh) = 0;

	virtual void SetEQU(IEQU* equ) = 0;

	virtual void AddEQP(IEQP* eqp) = 0;

	virtual void SetROL(IROL* rol) = 0;


	virtual bool GetMSH(IMSH* msh) = 0;

	virtual bool GetEQU(IEQU* equ) = 0;

	virtual bool GetEQP(IEQP* eqp, std::size_t index) = 0;

	virtual bool GetROL(IROL* rol) = 0;

	virtual int GetEQPSize() = 0;
};


#ifdef WIN32
HL7_ENGINE_DLL_API ILSU_U12* CreateLsuU12();
#else
ILSU_U12* CreateLsuU12();
#endif

#endif


