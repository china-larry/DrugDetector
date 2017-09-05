/*
* IORF_R04_PATITENT_RESULT.h
*
*  Created on: 2015年10月9日
*      Author: root
*/
///Represents the ORF_R04_PATIENT_RESULT Group.  A Group is an ordered collection of message 
/// segments that can repeat together or be optionally in/excluded together.
/// This Group contains the following elements: 
///<li>0: PID optional </li>
///<li>1: ORF_R04_ORDER_OBSERVATION (a Group object) repeating</li>
#ifndef INTERFACE_IORF_R04_PATITENT_RESULT_H_
#define INTERFACE_IORF_R04_PATITENT_RESULT_H_

#include "IObjectBase.h"
#include "base.h"
#include "IObjectList.h"
#include "IPID.h"
#include "IORF_R04_ORDER_OBSERVATION.h"

//观察结果观察报告组集合{OBR{[OBX]}}
struct IORF_R04_PATIENT_RESULT : public IObjectBase
{
public:
	virtual bool GetPatient(IPID* pid) = 0;

	virtual void SetPatient(const IPID* pid) = 0;

	virtual void Add_ORF_R04_ORDER_OBSERVATION(IORF_R04_ORDER_OBSERVATION* orderObservation) = 0;

	virtual void Delete_ORF_R04_ORDER_OBSERVATION(std::size_t index) = 0;

	virtual void Delete_ALL_ORF_R04_ORDER_OBSERVATIONS() = 0;

	virtual NRET Get_ORF_R04_ORDER_OBSERVATION_COLLECTION(IObjectList* objectList) = 0;

	virtual void GetString(char** messageString) = 0;

	virtual int GetOrderObservationSize() = 0;

	virtual int GetOBXSize(std::size_t orderObservationIndex) = 0;

	virtual bool GetOBR(IOBR* obr, std::size_t orderOBRIndex) = 0;

	virtual bool GetOBX(IOBX* obx, std::size_t orderOBRIndex,
		std::size_t obxIndex) = 0;
};

#ifdef WIN32
HL7_ENGINE_DLL_API IORF_R04_PATIENT_RESULT* CreateOrfR04PatientResult();
#else
IORF_R04_PATIENT_RESULT* CreateOrfR04PatientResult();
#endif

#endif /* INTERFACE_IORF_R04_PATITENT_RESULT_H_ */
