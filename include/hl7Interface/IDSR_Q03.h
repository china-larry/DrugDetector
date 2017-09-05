/*
* IDSR_Q03.h
*
*  Created on: 2015年11月9日
*      Author: maggie
*/
/// Represents a DSR_Q03 message structure (see chapter [AAA]). This structure contains the 
/// following elements:
///<ol>
///<li>0: MSH (Message Header) </li>
///<li>1: MSA (Message Acknowledgment) optional </li>
///<li>2: ERR (Error) optional </li>
///<li>3: QAK (Query Acknowledgment) optional </li>
///<li>4: QRD (Original-Style Query Definition) </li>
///<li>5: QRF (Original Style Query Filter) optional </li>
///<li>6: DSP (Display Data) repeating</li>
///<li>7: DSC (Continuation Pointer) optional </li>

#ifndef IDSR_Q03_H
#define IDSR_Q03_H
#ifdef WIN32
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif
#endif


#include "IObjectBase.h"
#include "IDSC.h"
#include "IMSH.h"
#include "IMSA.h"
#include "IQAK.h"
#include "IERR.h"
#include "IQRD.h"
#include "IQRF.h"
#include "IDSP.h"
#include "IObjectList.h"
class IDSR_Q03 : public IObjectBase
{
public:

	virtual void SetMSH(IMSH* msh) = 0;

	virtual void SetMSA(IMSA* msa) = 0;

	virtual void SetERR(IERR* err) = 0;

	virtual void SetQAK(IQAK* qak) = 0;

	virtual void SetQRD(IQRD* qrd) = 0;

	virtual void SetQRF(IQRF* qrf) = 0;
	
	virtual void AddDSP(IDSP* dsp) = 0;

	virtual void SetDSC(IDSC* dsc) = 0;


	virtual bool GetMSH(IMSH* msh) = 0;

	virtual bool GetMSA(IMSA* msa) = 0;

	virtual bool GetERR(IERR* err) = 0;

	virtual bool GetQAK(IQAK* qak) = 0;

	virtual bool GetQRD(IQRD* qrd) = 0;

	virtual bool GetQRF(IQRF* qrf) = 0;

	virtual bool GetDSP(IDSP* dsp, int index) = 0;

	virtual bool GetDSC(IDSC* dsc) = 0;

	virtual int GetDSPSize() = 0;

};


#ifdef WIN32
HL7_ENGINE_DLL_API IDSR_Q03* CreateDsrQ03();
#else
IDSR_Q03* CreateDsrQ03();
#endif

#endif


