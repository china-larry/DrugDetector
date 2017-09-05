/*
 * IOBX.h
 *
 *  Created on: 2015年10月9日
 *      Author: root
 */

#ifndef INTERFACE_IOBX_H_
#define INTERFACE_IOBX_H_


#ifdef WIN32
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif
#endif

#include "IObjectBase.h"
struct IOBX : public IObjectBase
{
public:
	/*
	*	\brief 递增ID, index 0
	*/
	virtual void SetOBXIndex(const char* obxIndexStr) = 0;

	virtual void GetOBXIndex(char** obxIndexStr) = 0;

	/*
	*	\brief 值类型，index 1
	*/
	virtual void SetValueType(const char* valueTypeStr) = 0;

	virtual void GetValueType(char** valueTypeStr) = 0;

	/*
	*	\brief 项目ID, index 2
	*/
	virtual void SetItemID(const char* itemIDStr) = 0;

	virtual void GetItemID(char** itemIDStr) = 0;

	/*
	*	\brief 项目名称, index 3
	*/
	virtual void SetItemName(const char* itemName) = 0;

	virtual void GetItemName(char** itemName) = 0;

	/*
	*	\brief 结果，index 4
	*/
	virtual void SetItemResult(const char* itemResultStr) = 0;

	virtual void GetItemResult(char** itemResultStr) = 0;

	/*
	*	\brief code卡批号，index 4
	*/
	virtual void SetCodeBatchNo(const char* codeBatchNo) = 0;

	virtual void GetCodeBatchNo(char** codeBatchNo) = 0;

	/*
	*	\brief 试剂卡批号，index 4
	*/
	virtual void SetReagentCardNo(const char* reagentCardNo) = 0;

	virtual void GetReagentCardNo(char** reagentCardNo) = 0;

	/*
	*	\brief 单位，index 5
	*/
	virtual void SetUnit(const char* unitStr) = 0;

	virtual void GetUnit(char** unitStr) = 0;

	/*
	*	\brief 参考值范围, index 6
	*/
	virtual void SetRefValue(const char* refValue) = 0;

	virtual void GetRefValue(char** refValue) = 0;

	/*
	*	\brief 不正常标记，index 7
	*/
	virtual void SetUnnormalMark(const char* unnormalMark) = 0;

	virtual void GetUnnormalMark(char** unnormalMark) = 0;

	/*
	*	\brief 结果状态，index 10
	*/
	virtual void SetResultState(const char* resultStateStr) = 0;

	virtual void GetResultState(char** resultStateStr) = 0;

	/*
	*	\brief 原始结果, index 12
	*/
	virtual void SetOriginalResult(const char* originalResult) = 0;

	virtual void GetOriginalResult(char** originalResult) = 0;

	/*
	*	\brief 检验日期, index 13
	*/
	virtual void SetTestTime(const char* testDateStr) = 0;

	virtual void GetTestTime(char** testDateStr) = 0;

	/*
	*	\brief 检验医生, index 15
	*/
	virtual void SetTestPhysician(const char* testPhysician) = 0;

	virtual void GetTestPhysician(char** testPhysician) = 0;
};

#ifdef WIN32
HL7_ENGINE_DLL_API IOBX* CreateOBX();
#else
IOBX* CreateOBX();
#endif

#endif /* INTERFACE_IOBX_H_ */
