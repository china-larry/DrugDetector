/*
 * IORU_R01_PATITENT_RESULT.h
 *
 *  Created on: 2015年10月9日
 *      Author: root
 */
///Represents the ORU_R01_PATIENT_RESULT Group.  A Group is an ordered collection of message 
/// segments that can repeat together or be optionally in/excluded together.
/// This Group contains the following elements: 
///<li>0: PID optional </li>
///<li>1: ORU_R01_ORDER_OBSERVATION (a Group object) repeating</li>

/**观察结果观察报告组集合
{
PID
{
OBR
{[OBX]}
}
}
**/

#ifndef INTERFACE_IORU_R01_PATITENT_RESULT_H_
#define INTERFACE_IORU_R01_PATITENT_RESULT_H_

#include "IObjectBase.h"
#include "base.h"
#include "IObjectList.h"
#include "IPID.h"
#include "IORU_R01_ORDER_OBSERVATION.h"


struct IORU_R01_PATIENT_RESULT  : public IObjectBase
{
public:
	virtual bool GetPatient(IPID* pid)=0;

	virtual void SetPatient(const IPID* pid)=0;

	virtual void Add_ORU_R01_ORDER_OBSERVATION(IORU_R01_ORDER_OBSERVATION* orderObservation)=0;

	virtual void Delete_ORU_R01_ORDER_OBSERVATION(std::size_t index)=0;

	virtual void Delete_ALL_ORU_R01_ORDER_OBSERVATIONS() =0;

	virtual NRET Get_ORU_R01_ORDER_OBSERVATION_COLLECTION(IObjectList* objectList) = 0;

	virtual void GetString(char** messageString) = 0;

	virtual int GetOrderObservationSize() = 0 ;

	virtual int GetOBXSize(std::size_t orderObservationIndex) = 0;

	virtual bool GetOBR(IOBR* obr, std::size_t orderOBRIndex) = 0;

	virtual bool GetOBX(IOBX* obx, std::size_t orderOBRIndex,
		std::size_t obxIndex) = 0;
};

#ifdef WIN32
HL7_ENGINE_DLL_API IORU_R01_PATIENT_RESULT* CreateOruR01PatientResult();
#else
IORU_R01_PATIENT_RESULT* CreateOruR01PatientResult();
#endif

#endif /* INTERFACE_IORU_R01_PATITENT_RESULT_H_ */
