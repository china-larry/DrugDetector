#ifndef _IPID_H_
#define _IPID_H_

#ifdef WIN32
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif
#endif

#include "IObjectBase.h"
struct IPID : public IObjectBase
{
		/*
		 *	\brief 设置Id-PID 递增ID
		 */
		virtual void SetPIDIndex(const char* pidIndex) = 0;

		virtual void GetPIDIndex(char** pidIndex) = 0;
		/*
		*	\brief 设置身份证
		*/
		virtual void SetID(const char* id) = 0;

		virtual void GetID(char** id) = 0;

		/*
		*	\brief 设置病历号
		*/
		virtual void SetCardNum(const char* cardNum) = 0;

		virtual void GetCardNum(char** cardNum) = 0;
		/*
		 *	\brief 设置住院号
		 */
		virtual void SetAdmissionNum(const char* admissionNo) = 0;

		virtual void BuildPID() = 0;

		virtual void GetAdmissionNum(char** admissionNum) = 0;
		/*
		 *	\brief 设置床号
		 */
		virtual void SetBed(const char* bed) = 0;

		virtual void GetBed(char** bed) = 0;
		/*
		 *	\brief 患者姓名
		 */
		virtual void SetName(const char* name) = 0;

		virtual void GetName(char** name) = 0;
		/*
		 *	\brief 出生日期
		 */
		virtual void SetBirthDate(const char* age) = 0;

		virtual void GetBirthDate(char** birthDate) = 0;
		/*
		 *	\brief 性别
		 */
		virtual void SetSex(const char* sex) = 0;

		virtual void GetSex(char** sex) = 0;
		/*
		 *	\brief 血型
		 */
		virtual void SetBloodType(const char* bloodType) = 0;

		virtual void GetBloodType(char** bloodType) = 0;
		/*
		 *	\brief 地址
		 */
		virtual void SetAddress(const char* address) = 0;

		virtual void GetAddress(char** address) = 0;

		/*
		 *	\brief 家庭号码
		 */
		virtual void SetHomePhoneNum(const char* homePhoneNum) = 0;

		virtual void GetHomePhoneNum(char** homePhoneNum) = 0;
		/*
		 *	\brief 手机号码
		 */
		virtual void SetTelephoneNum(const char* telephoneNum) = 0;

		virtual void GetTelephoneNum(char** telephoneNum) = 0;
		/*
		 *	\brief 社保号
		 */
		virtual void SetSSNNum(const char* ssnNum) = 0;

		virtual void GetSSNNum(char** ssnNum) = 0;
		/*
		 *	\brief 民族
		 */
		virtual void SetNation(const char* nation) = 0;

		virtual void GetNation(char** nation) = 0;
		/*
		 *	\brief 籍贯
		 */
		virtual void SetNativePlace(const char* nativePlace) = 0;

		virtual void GetNativePlace(char** nativePlace) = 0;
};

#ifdef WIN32
HL7_ENGINE_DLL_API IPID* CreatePID();
#else
IPID* CreatePID();
#endif
#endif

