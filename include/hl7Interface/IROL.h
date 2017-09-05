#ifndef _IROL_H_
#define _IROL_H_

#ifdef WIN32
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif
#endif

#include "IObjectBase.h"

/// Represents an HL7 ROL message segment. 
/// This segment has the following fields:<ol>
///<li>ROL-1: Role Instance ID (EI)</li>
///<li>ROL-2: Action Code (ID)</li>
///<li>ROL-3: Role-ROL (CE)</li>
///<li>ROL-4: Role Person (ST)</li>
///<li>ROL-5: Role Begin Date/Time (TS), Empty and Keep back</li>
///<li>ROL-6: Role End Date/Time (TS), Empty and Keep back</li>
///<li>ROL-7: Role Duration (CE), Empty and Keep back</li>
///<li>ROL-8: Role Action Reason (CE)</li>
///<li>ROL-9: Provider Type (CE), Empty and Keep back</li>
///<li>ROL-10: Organization Unit Type - ROL (CE)</li>
///<li>ROL-11: Office/Home Address (XAD)</li>
///<li>ROL-12: Phone (XTN)</li>


struct IROL : public IObjectBase
{
	/*
	 *	\brief 设置用户ID， index 0
	 */
	virtual void SetRoleInstanceID(const char* roleInstanceID) = 0;

	virtual void GetRoleInstanceID(char** roleInstanceID) = 0;

	/*
	*	\brief 设置行为码， index 1
	*/
	virtual void SetActionCode(const char* actionCode) = 0;

	virtual void GetActionCode(char** actionCode) = 0;

	/*
	*	\brief 用户所扮演的角色ID， index 2
	*/
	virtual void SetRoleID(const char* roleID) = 0;

	virtual void GetRoleID(char** roleID) = 0;

	/*
	*	\brief 用户所扮演的角色详细描述， index 2
	*/
	virtual void SetRoleText(const char* roleText) = 0;

	virtual void GetRoleText(char** roleText) = 0;

	/*
	*	\brief 设置用户名称， index 3
	*/
	virtual void SetRolePersonName(const char* name) = 0;

	virtual void GetRolePersonName(char** name) = 0;
	/*
	*	\brief 设置角色行动原因， index 7
	*/
	virtual void SetRoleActionReason(const char* roleActionReason) = 0;

	virtual void GetRoleActionReason(char** roleActionReason) = 0;
	/*
	*	\brief 设置所处的环境的ID， index 9，如医院、门诊、急疹、家等
	*/
	virtual void SetOrganizationUnitTypeID(const char* organizationUnitTypeID) = 0;

	virtual void GetOrganizationUnitTypeID(char** organizationUnitTypeID) = 0;
	/*
	*	\brief 设置所处的环境的描述， index 9，如医院、门诊、急疹、家等
	*/
	virtual void SetOrganizationUnitTypeText(const char* organizationUnitTypeText) = 0;

	virtual void GetOrganizationUnitTypeText(char** organizationUnitTypeText) = 0;
	/*
	*	\brief 设置用户地址， index 10
	*/
	virtual void SetAddress(const char* address) = 0;

	virtual void GetAddress(char** address) = 0;

	/*
	*	\brief 设置用户联系方式， index 11
	*/
	virtual void SetTelephoneNum(const char* telephoneNum) = 0;

	virtual void GetTelephoneNum(char** telephoneNum) = 0;
};

#ifdef WIN32
HL7_ENGINE_DLL_API IROL* CreateROL();
#else
IROL* CreateROL();
#endif

#endif