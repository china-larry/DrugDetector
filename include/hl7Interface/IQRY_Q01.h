/*
* IQRY_Q01.h
*
*  Created on: 2015年11月9日
*      Author: maggie
*/
/// Represents a QRY_Q01 message structure (see chapter 5). This structure contains the 
/// following elements:
///<ol>
///<li>0: MSH (Message Header) </li>
///<li>1: QRD (Original-Style Query Definition) </li>
///<li>2: QRF (Original Style Query Filter) optional </li>
///<li>3: DSC (Continuation Pointer) optional </li>

#ifndef IEAC_U07_H
#define IEAC_U07_H
#ifdef _WINDOW_PLATFORM_
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif
#endif


#include "IObjectBase.h"
#include "IMSH.h"
#include "IQRD.h"
#include "IQRF.h"
#include "IDSC.h"

class IQRY_Q01 : public IObjectBase
{
public:

	virtual void SetMSH(IMSH* msh) = 0;

	virtual void SetQRD(IQRD* qrd) = 0;

	virtual void SetQRF(IQRF* qrf) = 0;

	virtual void SetDSC(IDSC* dsc) = 0;


	virtual bool GetMSH(IMSH* msh) = 0;

	virtual bool GetQRD(IQRD* qrd) = 0;

	virtual bool GetQRF(IQRF* qrf) = 0;

	virtual bool GetDSC(IDSC* dsc) = 0;

};


#ifdef _WINDOW_PLATFORM_
HL7_ENGINE_DLL_API IQRY_Q01* CreateQryQ01();
#else
IQRY_Q01* CreateQryQ01();
#endif

#endif


