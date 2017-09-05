/*
* IQCK_Q02.h
*
*  Created on: 2015年11月9日
*      Author: maggie
*/
/// Represents a QCK_Q02 message structure (see chapter [AAA]). This structure contains the 
/// following elements:
///<ol>
///<li>0: MSH (Message Header) </li>
///<li>1: MSA (Message Acknowledgment) </li>
///<li>2: ERR (Error) optional </li>
///<li>3: QAK (Query Acknowledgment) optional </li>

#ifndef IQCK_Q02_H
#define IQCK_Q02_H
#ifdef WIN32
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif
#endif


#include "IObjectBase.h"
#include "IMSH.h"
#include "IMSA.h"
#include "IERR.h"
#include "IQAK.h"

class IQCK_Q02 : public IObjectBase
{
public:

	virtual void SetMSH(IMSH* msh) = 0;

	virtual void SetERR(IERR* err) = 0;

	virtual void SetQAK(IQAK* qak) = 0;

	virtual void SetMSA(IMSA* msa) = 0;


	virtual bool GetMSH(IMSH* msh) = 0;

	virtual bool GetERR(IERR* err) = 0;

	virtual bool GetQAK(IQAK* qak) = 0;

	virtual bool GetMSA(IMSA* msa) = 0;

};


#ifdef WIN32
HL7_ENGINE_DLL_API IQCK_Q02* CreateQckQ02();
#else
IQCK_Q02* CreateQckQ02();
#endif

#endif


