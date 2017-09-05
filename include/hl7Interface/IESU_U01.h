/*
* IESU_U01.h
*
*  Created on: 2015年11月9日
*      Author: maggie
*/
/// Represents a ESU_U01 message structure (see chapter 13). This structure contains the 
/// following elements:
///<ol>
///<li>0: MSH (Message Header) </li>
///<li>1: EQU (Equipment Detail) </li>
///<li>2: ISD (Interaction Status Detail) optional repeating</li>
///<li>3: ROL (Role) optional </li>

#ifndef IESU_U01_H
#define IESU_U01_H
#ifdef WIN32
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif
#endif


#include "IObjectBase.h"
#include "IMSH.h"
#include "IEQU.h"
#include "IISD.h"
#include "IROL.h"

class IESU_U01 : public IObjectBase
{
public:

	virtual void SetMSH(IMSH* msh) = 0;

	virtual void SetEQU(IEQU* equ) = 0;

	virtual void AddISD(IISD* isd) = 0;

	virtual void SetROL(IROL* rol) = 0;


	virtual bool GetMSH(IMSH* msh) = 0;

	virtual bool GetEQU(IEQU* equ) = 0;

	virtual bool GetISD(IISD* isd, std::size_t index) = 0;

	virtual bool GetROL(IROL* rol) = 0;

	virtual int GetISDSize() = 0;
};


#ifdef WIN32
HL7_ENGINE_DLL_API IESU_U01* CreateEsuU01();
#else
IESU_U01* CreateEsuU01();
#endif

#endif


