/*
* IEAR_U08.h
*
*  Created on: 2015年11月9日
*      Author: maggie
*/
/// Represents a EAR_U08 message structure (see chapter 13). This structure contains the 
/// following elements:
///<ol>
///<li>0: MSH (Message Header) </li>
///<li>1: EQU (Equipment Detail) </li>
///<li>2: EAR_U08_COMMAND_RESPONSE (a Group object) repeating</li>
///<li>3: ROL (Role) optional </li>

#ifndef IEAR_U08_H
#define IEAR_U08_H
#ifdef WIN32
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif
#endif


#include "IObjectBase.h"
#include "IMSH.h"
#include "IEQU.h"
#include "IEAR_U08_COMMAND_RESPONSE.h"
#include "IROL.h"
#include "IObjectList.h"

class IEAR_U08 : public IObjectBase
{
public:

	virtual void SetMSH(IMSH* msh) = 0;

	virtual void SetEQU(IEQU* equ) = 0;

	virtual void Add_Command_Response(IEAR_U08_COMMAND_RESPONSE* cmdResponse) = 0;

	virtual void SetROL(IROL* rol) = 0;

	/*
	 *	\brief 返回CommandResponse结构的个数
	 */
	virtual int  GetCommandResponseSize() = 0;



	virtual bool GetMSH(IMSH* msh) = 0;

	virtual bool GetEQU(IEQU* equ) = 0;

	virtual NRET GetCommandResponseAll(IObjectList* objectList) = 0;

	virtual bool GetROL(IROL* rol) = 0;

	/*
	 *	\brief 返回序号为Index的CommandResponse结构中的ECD段
	 *  \param ecd，传递指针ecd，如果有对应的ECD段，值拷贝
	 *  \return bool,如果拷贝成功返回true，否则返回false
	 */
	virtual bool GetECD(IECD* ecd, std::size_t index) = 0;

	/*
	*	\brief 返回序号为Index的CommandResponse结构中的ECR段
	*  \param ecr，传递指针ecr，如果有对应的ECD段，值拷贝
	*  \return bool,如果拷贝成功返回true，否则返回false
	*/
	virtual bool GetECR(IECR* ecr, std::size_t index) = 0;
};


#ifdef WIN32
HL7_ENGINE_DLL_API IEAR_U08* CreateEarU08();
#else
IEAR_U08* CreateEarU08();
#endif

#endif


