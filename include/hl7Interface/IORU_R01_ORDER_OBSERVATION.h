/*
 * IORU_R01_ORDER_OBSERVATION.h
 *
 *  Created on: 2015年10月9日
 *      Author: maggie
 */
///Represents the ORU_R01_ORDER_OBSERVATION Group.  A Group is an ordered collection of message 
/// segments that can repeat together or be optionally in/excluded together.
/// This Group contains the following elements: 
///<li>0: OBR (Observation Request) </li>
///<li>1: OBX collection</li>

#ifndef _IORU_R01_ORDER_OBSERVATION_H_
#define _IORU_R01_ORDER_OBSERVATION_H_

#include "IOBR.h"
#include "IOBX.h"
#include "IObjectList.h"

struct IORU_R01_ORDER_OBSERVATION : public IObjectBase
{
	virtual bool GetOBR(IOBR* obr) = 0;

	virtual void SetOBR(IOBR* obr) = 0;

	virtual NRET GetOBXCollection(IObjectList* objectList) = 0;

	virtual void GetString(char** str) = 0;

	virtual void AddOBX(IOBX* obx) = 0;

	virtual int GetOBXSize() = 0;

	virtual bool GetOBX(IOBX* obx, std::size_t obxIndex) = 0;
};

#ifdef WIN32
HL7_ENGINE_DLL_API IORU_R01_ORDER_OBSERVATION* CreateOruR01OrderObservation();
#else
IORU_R01_ORDER_OBSERVATION* CreateOruR01OrderObservation();
#endif

#endif /* _IORU_R01_ORDER_OBSERVATION_H_ */
