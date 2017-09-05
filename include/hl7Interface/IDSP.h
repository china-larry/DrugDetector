#ifndef _IDSP_H_
#define _IDSP_H_

#ifdef WIN32
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif
#endif

#include "IObjectBase.h"

/// Represents an HL7 DSP message segment. 
/// This segment has the following fields:
///<li>DSP-1: Set ID - DSP (SI)</li>
///<li>DSP-2: Display Level (SI)</li>
///<li>DSP-3: Data Line (TX)</li>
///<li>DSP-4: Logical Break Point (ST)</li>
///<li>DSP-5: Result ID (TX)</li>


struct IDSP : public IObjectBase
{
	/*
	*	\brief DSP ID,index 0
	*/
	virtual void SetDSPID(const char* dspID) = 0;

	virtual void GetDSPID(char** dspID) = 0;

	/*
	*	\brief 显示水平,index 1
	*/
	virtual void SetDisplayLevel(const char* displayLevel) = 0;

	virtual void GetDisplayLevel(char** displayLevel) = 0;

	/*
	*	\brief 数据行,index 2
	*/
	virtual void SetDataLine(const char* dataLine) = 0;

	virtual void GetDataLine(char** dataLine) = 0;

	/*
	*	\brief 逻辑断点,index 3
	*/
	virtual void SetLogicalBreakPoint(const char* logicalBreakPoint) = 0;

	virtual void GetLogicalBreakPoint(char** logicalBreakPoint) = 0;

	/*
	*	\brief 结果，开始日期,index 4
	*/
	virtual void SetResultID(const char* resultID) = 0;

	virtual void GetResultID(char** resultID) = 0;

};

#ifdef WIN32
HL7_ENGINE_DLL_API IDSP* CreateDSP();
#else
IDSP* CreateDSP();
#endif

#endif