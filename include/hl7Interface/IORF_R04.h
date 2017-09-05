/*
* IORF_R04.h
*
*  Created on: 2015年11月9日
*      Author: maggie
*/
/// Represents a ORF_R04 message structure (see chapter 7). This structure contains the 
/// following elements:
///<ol>
///<li>0: MSH (Message Header) </li>
///<li>1: MSA (Message Acknowledgment) </li>
///<li>2: QRD (Original-Style Query Definition) </li>
///<li>3: QRF (Original Style Query Filter) optional </li>
///<li>4: ORF_R04_RESPONSE (a Group object) repeating</li>
///<li>5: ERR (Error) optional </li>
///<li>6: QAK (Query Acknowledgment) optional </li>
///<li>7: DSC (Continuation Pointer) optional </li>

#ifndef IORF_R04_H
#define IORF_R04_H
#ifdef WIN32
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif
#endif


#include "IObjectBase.h"
#include "IMSH.h"
#include "IMSA.h"
#include "IQRF.h"
#include "IQRD.h"
#include "IERR.h"
#include "IQAK.h"
#include "IDSC.h"
#include "IORF_R04_PATIENT_RESULT.h"
#include "IObjectList.h"

class IORF_R04 : public IObjectBase
{
public:

	virtual void SetMSH(IMSH* msh) = 0;

	virtual void SetMSA(IMSA* msa) = 0;

	virtual void SetQRD(IQRD* qrd) = 0;

	virtual void SetQRF(IQRF* qrf) = 0;

	virtual void SetERR(IERR* err) = 0;

	virtual void SetQAK(IQAK* qak) = 0;

	virtual void SetDSC(IDSC* dsc) = 0;

	virtual void Add_Patient_Result(IORF_R04_PATIENT_RESULT* patientResult) = 0;

	virtual bool GetMSH(IMSH* msh) = 0;

	virtual bool GetMSA(IMSA* msa) = 0;

	virtual bool GetERR(IERR* err) = 0;

	virtual bool GetQAK(IQAK* qak) = 0;

	virtual bool GetQRD(IQRD* qrd) = 0;

	virtual bool GetQRF(IQRF* qrf) = 0;

	virtual bool GetDSC(IDSC* dsc) = 0;

	virtual NRET GetPatientResultAll(IObjectList* objectList) = 0;

	virtual int GetPatientResultSize() = 0;

	virtual int GetOrderObservationSize(std::size_t patientResultIndex) = 0;

	virtual int GetOBXSize(std::size_t patientResultIndex, std::size_t orderOBRIndex) = 0;

	virtual bool GetPID(IPID* pid, std::size_t index) = 0;

	virtual bool GetOBR(IOBR* obr, std::size_t patientResultIndex,
		std::size_t orderOBRIndex) = 0;

	virtual bool GetOBX(IOBX* obx, std::size_t patientResultIndex,
		std::size_t orderOBRIndex, std::size_t obxIndex) = 0;

};


#ifdef WIN32
HL7_ENGINE_DLL_API IORF_R04* CreateOrfR04();
#else
IORF_R04* CreateOrfR04();
#endif

#endif


