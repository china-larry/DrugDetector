/*
 * IOBR.h
 *
 *  Created on: 2015年10月9日
 *      Author: root
 */

#ifndef INTERFACE_IOBR_H_
#define INTERFACE_IOBR_H_

#ifdef WIN32
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif
#endif

#include "IObjectBase.h"
enum OBRType
{
	SampleResult = 0,
	QCResult = 2
};
struct IOBR : public IObjectBase
{
public:
	virtual void SetOBRType(OBRType obrType) = 0;

	virtual OBRType GetOBRType() = 0;
	/*
	*	\brief OBR流水号，从1开始递增
	*/
	virtual void SetOBRIndex(const char* obrIndexStr) = 0;

	virtual void GetOBRIndex(char** obrIndexStr) = 0;
	//////////////////////适用于样本结果///////////////////////

	/*
	*	\brief 设置样本条码,index 1
	*/
	virtual void SetSampleBarcode(const char* sampleBarcode) = 0;

	virtual void GetSampleBarcode(char** sampleBarcode) = 0;
	/*
	*	\brief 设置样本编号,index 2
	*/
	virtual void SetSampleID(const char* sampleID) = 0;

	virtual void GetSampleID(char** sampleID) = 0;

	/*
	*	\brief 设置检验时间,index 6
	*/
	virtual void SetTestTime(const char* testTime) = 0;

	virtual void GetTestTime(char** testTime) = 0;

	/*
	*	\brief 设置病人临床诊断信息, index 12
	*/

	virtual void SetDiagnosticMessage(const char* diagnosticMessage) = 0;

	virtual void GetDiagnosticMessage(char** diagnosticMessage) = 0;

	/*
	*	\brief 设置样本类型， index 13
	*/
	virtual void SetSampleType(const char* sampleType) = 0;

	virtual void GetSampleType(char** sampleType) = 0;

	/*
	*	\brief 设置样本送检时间, index 14
	*/
	virtual void SetSubmitSampleTime(const char* submitSampleTime) = 0;

	virtual void GetSubmitSampleTime(char** submitSampleTime) = 0;


	/*
	*	\brief 设置送检医师，index 15
	*/
	virtual void SetSubmittingPhysician(const char* submittingPhysician) = 0;

	virtual void GetSubmittingPhysician(char** submittingPhysician) = 0;

	/*
	*	\brief 设置送检科室，index 16
	*/
	virtual void SetSubmittingDepartment(const char* submittingDepartment) = 0;

	virtual void GetSubmittingDepartment(char** submittingDepartment) = 0;

	/*
	*	\brief 设置主治医师，index 19
	*/
	virtual void SetAttendingPhysician(const char* attendingPhysician) = 0;

	virtual void GetAttendingPhysician(char** attendingPhysician) = 0;

	/*
	*	\brief 设置治疗科室，index 20
	*/
	virtual void SetTreatDepartment(const char* treatDepartment) = 0;

	virtual void GetTreatDepartment(char** treatDepartment) = 0;
	////////////////////////适用于质控/////////////////////////////////

	/*
	*	\brief 设置项目编号，index 1
	*/
	virtual void SetProjectID(const char* projectID) = 0;

	virtual void GetProjectID(char** projectID) = 0;

	/*
	*	\brief 设置项目名称, index 2
	*/
	virtual void SetProjectName(const char* projectName) = 0;

	virtual void GetProjectName(char** projectName) = 0;

	/*
	*	\brief 设置质控日期, index 6
	*/
	virtual void SetQCTime(const char* qcTime) = 0;

	virtual void GetQCTime(char** qcTime) = 0;

	/*
	*	\brief 设置质控液数量,index 10
	*/
	virtual void SetQCLiquidQuantity(const char* qcLiquidQuantity) = 0;

	virtual void GetQCLiquidQuantity(char** qcLiquidQuantity) = 0;

	/*
	*	\brief 设置质控液编号,index 11
	*/
	virtual void SetQCLiquidID(const char* qcLiquidID) = 0;

	virtual void GetQCLiquidID(char** qcLiquidID) = 0;
	/*
	*	\brief 设置质控液名称,index 12
	*/
	virtual void SetQCLiquidName(const char* qcLiquidName) = 0;

	virtual void GetQCLiquidName(char** qcLiquidName) = 0;
	/*
	*	\brief 设置质控液批号,index 13
	*/
	virtual void SetQCLiquidBatchNo(const char* qcLiquidBatchNo) = 0;

	virtual void GetQCLiquidBatchNo(char** qcLiquidBatchNo) = 0;
	/*
	*	\brief 设置质控液有效期,index 14
	*/
	virtual void SetQCLiquidValidDate(const char* qcLiquidValidDate) = 0;

	virtual void GetQCLiquidValidDate(char** qcLiquidValidDate) = 0;

	/*
	*	\brief 设置质控范围,index 15
	*/
	virtual void SetQCScope(const char* qcScope) = 0;

	virtual void GetQCScope(char** qcScope) = 0;

	/*
	*	\brief 设置质控液浓度水平，index 16
	*/
	virtual void SetQCLiquidDensityLevel(const char* qcLiquidDensityLevel) = 0;

	virtual void GetQCLiquidDensityLevel(char** qcLiquidDensityLevel) = 0;

	/*
	*	\brief 设置质控液浓度均值，index 17
	*/
	virtual void SetQCLiquidDensityAV(const char* qcLiquidDensityAV) = 0;

	virtual void GetQCLiquidDensityAV(char** qcLiquidDensityAV) = 0;

	/*
	*	\brief 设置质控液标准差，index 18
	*/
	virtual void SetQCLiquidSD(const char* qcLiquidSD) = 0;

	virtual void GetQCLiquidSD(char** qcLiquidSD) = 0;

	/*
	*	\brief 设置质控结果，index 19
	*/
	virtual void SetQCTestResult(const char* qcTestResult) = 0;

	virtual void GetQCTestResult(char** qcTestResult) = 0;

	/*
	*	\brief 设置质控试剂批号，index 20
	*/
	virtual void SetQCReagentBatchNo(const char* qcReagentBatchNo) = 0;

	virtual void GetQCReagentBatchNo(char** qcReagentBatchNo) = 0;
};

#ifdef WIN32
HL7_ENGINE_DLL_API IOBR* CreateOBR();
#else
IOBR* CreateOBR();
#endif

#endif /* INTERFACE_IOBR_H_ */
