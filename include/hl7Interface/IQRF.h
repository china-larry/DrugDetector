#ifndef _IQRF_H_
#define _IQRF_H_

#ifdef WIN32
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif
#endif

#include "IObjectBase.h"

/// Represents an HL7 QRF message segment. 
/// This segment has the following fields:<ol>
///<li>QRF-1: Where Subject Filter (ST)</li>
///<li>QRF-2: When Data Start Date/Time (TS)</li>
///<li>QRF-3: When Data End Date/Time (TS)</li>
///<li>QRF-4: What User Qualifier (ST), Empty and Keep back</li>
///<li>QRF-5: Other QRY Subject Filter (ST), Empty and Keep back</li>
///<li>QRF-6: Which Date/Time Qualifier (ID)</li>
///<li>QRF-7: Which Date/Time Status Qualifier (ID)</li>
///<li>QRF-8: Date/Time Selection Qualifier (ID)</li>
///<li>QRF-9: When Quantity/Timing Qualifier (TQ), Empty and Keep back</li>
///<li>QRF-10: Search Confidence Threshold (NM), Empty and Keep back</li>

struct IQRF : public IObjectBase
{
	/*
	*	\brief 何地主题筛选程序,index 0
	*/
	virtual void SetWhereSubjectFilter(const char* whereSubjectFilter) = 0;

	virtual void GetWhereSubjectFilter(char** whereSubjectFilter) = 0;

	/*
	*	\brief 数据开始日期/时间,index 1
	*/
	virtual void SetWhenDataStartDateTime(const char* whenDataStartDateTime) = 0;

	virtual void GetWhenDataStartDateTime(char** whenDataStartDateTime) = 0;

	/*
	*	\brief 	数据结束日期/时间,index 2
	*/
	virtual void SetWhenDataEndDateTime(const char* whenDataEndDateTime) = 0;

	virtual void GetWhenDataEndDateTime(char** whenDataEndDateTime) = 0;

	/*
	*	\brief 	哪个日期/时间限定符,index 5,默认取RC值(标本收到日期/时间)
	*/
	virtual void SetWhichDateTimeQualifier(const char* whichDateTimeQualifier) = 0;

	virtual void GetWhichDateTimeQualifier(char** whichDateTimeQualifier) = 0;

	/*
	*	\brief 哪个日期/时间状况限定符,index 6,默认取COR值(仅为修正值（不带有修正的最终值）)
	*/
	virtual void SetWhichDateTimeStatusQualifier(const char* whichDateTimeStatusQualifier) = 0;

	virtual void GetWhichDateTimeStatusQualifier(char** whichDateTimeStatusQualifier) = 0;


	/*
	*	\brief 日期/时间选择限定符,index 7,默认取ALL值(范围内的所有值)
	*/
	virtual void SetDateTimeSelectionQualifier(const char* dateTimeSelectionQualifier) = 0;

	virtual void GetDateTimeSelectionQualifier(char** dateTimeSelectionQualifier) = 0;
	
};

#ifdef WIN32
HL7_ENGINE_DLL_API IQRF* CreateQRF();
#else
IQRF* CreateQRF();
#endif

#endif