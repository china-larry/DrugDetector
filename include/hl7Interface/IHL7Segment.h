#ifndef IHL7SEGMENT_H
#define IHL7SEGMENT_H

#ifdef WIN32
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif
#endif

#include "base.h"
#include "IObjectBase.h"
struct IHL7Segment : public IObjectBase
{
	/*
	*@brief 返回段字符串
	*/
	virtual void GetSegmentString(char** segmentStr)=0;

	virtual void SetSegmentID(const char* segmentID) = 0;

	virtual void GetSegmentID(char** segmentID) = 0;
};
//#ifdef WIN32
//HL7_ENGINE_DLL_API IHL7Segment* CreateSegment();
//#else
//IHL7Segment* CreateSegment();
//#endif

#endif
