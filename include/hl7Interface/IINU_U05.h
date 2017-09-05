﻿/*
* IINU_U05.h
*
*  Created on: 2015年11月9日
*      Author: maggie
*/
/// Represents a INU_U05 message structure (see chapter 13). This structure contains the 
/// following elements:
///<ol>
///<li>0: MSH (Message Header) </li>
///<li>1: EQU (Equipment Detail) </li>
///<li>2: INV (Inventory Detail) repeating</li>
///<li>3: ROL (Role) optional </li>

#ifndef IINU_U05_H
#define IINU_U05_H
#ifdef WIN32
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif
#endif


#include "IObjectBase.h"
#include "IMSH.h"
#include "IEQU.h"
#include "IINV.h"
#include "IROL.h"

class IINU_U05 : public IObjectBase
{
public:

	virtual void SetMSH(IMSH* msh) = 0;

	virtual void SetEQU(IEQU* equ) = 0;

	virtual void AddINV(IINV* inv) = 0;

	virtual void SetROL(IROL* rol) = 0;


	virtual bool GetMSH(IMSH* msh) = 0;

	virtual bool GetEQU(IEQU* equ) = 0;

	virtual bool GetINV(IINV* inv, std::size_t index) = 0;

	virtual bool GetROL(IROL* rol) = 0;

	virtual int GetINVSize() = 0;
};


#ifdef WIN32
HL7_ENGINE_DLL_API IINU_U05* CreateInuU05();
#else
IINU_U05* CreateInuU05();
#endif

#endif


