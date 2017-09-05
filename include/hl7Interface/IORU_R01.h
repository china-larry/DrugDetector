/*
 * IORU_R01.h
 *
 *  Created on: 2015年10月8日
 *      Author: root
 */

#ifndef IORU_R01_H
#define IORU_R01_H
#ifdef WIN32
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif
#endif


#include "IObjectBase.h"
#include "IDSC.h"
#include "IMSH.h"
#include "IObjectList.h"
#include "IORU_R01_PATITENT_RESULT.h"

/**
* ORU_R01消息结构包含如下内容：
*  MSH
* {
*  [PID]
*  {
*	OBR
*   {[OBX]}
*   }
*  }
* [DSC]
*
**/
class IORU_R01 : public IObjectBase
{
public:
	virtual void SetDSC(IDSC* dsc) = 0;

	virtual void SetMSH(IMSH* msh) = 0;

	virtual void Add_Patient_Result(IORU_R01_PATIENT_RESULT* patientResult) = 0;

	virtual bool GetMSH(IMSH* msh) = 0;

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
HL7_ENGINE_DLL_API IORU_R01* CreateOruR01();
#else
IORU_R01* CreateOruR01();
#endif

#endif


