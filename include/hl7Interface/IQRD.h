#ifndef _IQRD_H_
#define _IQRD_H_

#ifdef WIN32
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif
#endif

#include "IObjectBase.h"

/// Represents an HL7 QRD message segment. 
/// This segment has the following fields:<ol>
///<li>QRD-1: Query Date/Time (TS)</li>
///<li>QRD-2: Query Format Code (ID)</li>
///<li>QRD-3: Query Priority (ID)</li>
///<li>QRD-4: Query ID (ST)</li>
///<li>QRD-5: Deferred Response Type (ID), Empty and Keep back</li> 
///<li>QRD-6: Deferred Response Date/Time (TS), Empty and Keep back</li>
///<li>QRD-7: Quantity Limited Request (CQ)</li>
///<li>QRD-8: Sample Barcode (ST)</li>
///<li>QRD-9: What Subject Filter (CE)</li>
///<li>QRD-10: What Department Data Code (CE), Empty and Keep back</li>
///<li>QRD-11: What Data Code Value Qual. (VR), Empty and Keep back</li>
///<li>QRD-12: Query Results Level (ID)</li>

struct IQRD : public IObjectBase
{
	/*
	*	\brief 查询日期时间,index 0
	*/
	virtual void SetQueryDateTime(const char* queryDateTime) = 0;

	virtual void GetQueryDateTime(char** queryDateTime) = 0;

	/*
	*	\brief 查询格式编码,index 1，本协议取R值（记录导向格式的回应）
	*/
	virtual void SetQueryFormatCode(const char* queryFormatCode) = 0;

	virtual void GetQueryFormatCode(char** queryFormatCode) = 0;

	/*
	*	\brief 查询优先值,index 2,本协议取D值(延迟的）
	*/
	virtual void SetQueryPriority(const char* queryPriority) = 0;

	virtual void GetQueryPriority(char** queryPriority) = 0;

	/*
	*	\brief 查询ID,index 3
	*/
	virtual void SetQueryID(const char* queryID) = 0;

	virtual void GetQueryID(char** queryID) = 0;

	/*
	*	\brief 数量限制请求_数量,index 6
	*/
	virtual void SetQuantityLimitedRequest_Quantity(const char* quantity) = 0;

	virtual void GetQuantityLimitedRequest_Quantity(char** quantity) = 0;

	/*
	*	\brief 数量限制请求_单位,index 6， 默认值是RD（记录)
	*/
	virtual void SetQuantityLimitedRequest_Units(const char* units) = 0;

	virtual void GetQuantityLimitedRequest_Units(char** units) = 0;

	/*
	*	\brief 样本条码,index 7，批量查询为空
	*/
	virtual void SetSampleBarcode(const char* sampleBarcode) = 0;

	virtual void GetSampleBarcode(char** sampleBarcode) = 0;
	
	/*
	*	\brief 什么主题筛选程序,index 8，查询时置为OTH，取消时取CAN
	*/
	virtual void SetWhatSubjectFilter(const char* whatSubjectFilter) = 0;

	virtual void GetWhatSubjectFilter(char** whatSubjectFilter) = 0;


	/*
	*	\brief 查询结果水平,index 12，默认取T
	*/
	virtual void SetQueryResultsLevel(const char* queryResultsLevel) = 0;

	virtual void GetQueryResultsLevel(char** queryResultsLevel) = 0;
};

#ifdef WIN32
HL7_ENGINE_DLL_API IQRD* CreateQRD();
#else
IQRD* CreateQRD();
#endif

#endif