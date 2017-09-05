#ifndef _IINV_H_
#define _IINV_H_

#ifdef WIN32
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif
#endif

#include "IObjectBase.h"

/// Represents an HL7 INV message segment. 
/// This segment has the following fields:<ol>
///<li>INV-1: Substance Identifier (CE)</li>
///<li>INV-2: Substance Status (CE)</li>
///<li>INV-3: Substance Type (CE)</li>
///<li>INV-4: Inventory Container Identifier (CE)</li>
///<li>INV-5: Container Carrier Identifier (CE)</li>
///<li>INV-6: Position on Carrier (CE)</li>
///<li>INV-7: Initial Quantity (NM)</li>
///<li>INV-8: Current Quantity (NM)</li>
///<li>INV-9: Available Quantity (NM)</li>
///<li>INV-10: Consumption Quantity (NM)</li>
///<li>INV-11: Quantity Units (CE)</li>
///<li>INV-12: Expiration Date/Time (TS)</li>
///<li>INV-13: First Used Date/Time (TS)</li>
///<li>INV-14: On Board Stability Duration (TQ)</li>
///<li>INV-15: Test/Fluid Identifier(s) (CE),Empty and Keep back</li>
///<li>INV-16: Manufacturer Lot Number (ST)</li>
///<li>INV-17: Manufacturer Identifier (CE)</li>
///<li>INV-18: Supplier Identifier (CE)</li>

struct IINV : public IObjectBase
{
	/*
	*	\brief 耗材标识符,index 0
	*/
	virtual void SetSubstanceID(const char* substanceID) = 0;

	virtual void GetSubstanceID(char** substanceID) = 0;

	/*
	*	\brief 耗材描述文本,index 0
	*/
	virtual void SetSubstanceText(const char* substanceText) = 0;

	virtual void GetSubstanceText(char** substanceText) = 0;

	/*
	*	\brief 耗材状态ID,index 1
	*/
	virtual void SetSubstanceStatusID(const char* substanceStatusID) = 0;

	virtual void GetSubstanceStatusID(char** substanceStatusID) = 0;

	/*
	*	\brief 耗材状态文本描述,index 1
	*/
	virtual void SetSubstanceStatusText(const char* substanceStatusText) = 0;

	virtual void GetSubstanceStatusText(char** substanceStatusText) = 0;

	/*
	*	\brief 耗材类型ID,index 2
	*/
	virtual void SetSubstanceTypeID(const char* substanceTypeID) = 0;

	virtual void GetSubstanceTypeID(char** substanceTypeID) = 0;

	/*
	*	\brief 耗材类型文本描述,index 2
	*/
	virtual void SetSubstanceTypeText(const char* substanceTypeText) = 0;

	virtual void GetSubstanceTypeText(char** substanceTypeText) = 0;


	/*
	*	\brief 货存清单容器标识符ID,index 3
	*/
	virtual void SetInventoryContainerID(const char* inventoryContainerID) = 0;

	virtual void GetInventoryContainerID(char** inventoryContainerID) = 0;

	/*
	*	\brief 货存清单容器标识符文本描述,index 3
	*/
	virtual void SetInventoryContainerText(const char* inventoryContainerText) = 0;

	virtual void GetInventoryContainerText(char** inventoryContainerText) = 0;


	/*
	*	\brief 容器架标识符ID,index 4
	*/
	virtual void SetContainerCarrierID(const char* containerCarrierID) = 0;

	virtual void GetContainerCarrierID(char** containerCarrierID) = 0;

	/*
	*	\brief 容器架标识符文本描述,index 4
	*/
	virtual void SetContainerCarrierText(const char* containerCarrierText) = 0;

	virtual void GetContainerCarrierText(char** containerCarrierText) = 0;


	/*
	*	\brief 架子上的位置ID,index 5
	*/
	virtual void SetPositionOnCarrierID(const char* positionOnCarrierID) = 0;

	virtual void GetPositionOnCarrierID(char** positionOnCarrierID) = 0;

	/*
	*	\brief 架子上的位置文本描述,index 5
	*/
	virtual void SetPositionOnCarrierText(const char* positionOnCarrierText) = 0;

	virtual void GetPositionOnCarrierText(char** positionOnCarrierText) = 0;

	/*
	*	\brief 初始数量,index 6
	*/
	virtual void SetInitialQuantity(const char* initialQuantity) = 0;

	virtual void GetInitialQuantity(char** initialQuantity) = 0;

	/*
	*	\brief 当前数量,index 7
	*/
	virtual void SetCurrentQuantity(const char* currentQuantity) = 0;

	virtual void GetCurrentQuantity(char** currentQuantity) = 0;

	/*
	*	\brief 可用数量,index 8
	*/
	virtual void SetAvailableQuantity(const char* availableQuantity) = 0;

	virtual void GetAvailableQuantity(char** availableQuantity) = 0;

	/*
	*	\brief 消耗数量,index 9
	*/
	virtual void SetConsumptionQuantity(const char* consumptionQuantity) = 0;

	virtual void GetConsumptionQuantity(char** consumptionQuantity) = 0;


	/*
	*	\brief 数量单位,index 10
	*/
	virtual void SetQuantityUnits(const char* quantityUnits) = 0;

	virtual void GetQuantityUnits(char** quantityUnits) = 0;

	/*
	*	\brief 过期时间,index 11
	*/
	virtual void SetExpirationDate(const char* expirationDate) = 0;

	virtual void GetExpirationDate(char** expirationDate) = 0;

	/*
	*	\brief 第一次使用时间,index 12
	*/
	virtual void SetFirstUsedDate(const char* firstUsedDate) = 0;

	virtual void GetFirstUsedDate(char** firstUsedDate) = 0;

	/*
	*	\brief 稳定使用的持续时间_开始日期,index 13
	*/
	virtual void SetOnBoardStabilityDuration_StartTime(const char* startTime) = 0;

	virtual void GetOnBoardStabilityDuration_StartTime(char** startTime) = 0;

	/*
	*	\brief 稳定使用的持续时间_截止日期,index 13
	*/
	virtual void SetOnBoardStabilityDuration_EndTime(const char* endTime) = 0;

	virtual void GetOnBoardStabilityDuration_EndTime(char** endTime) = 0;

	/*
	*	\brief 制造商标签号,index 15
	*/
	virtual void SetManufacturerLotNumber(const char* manufacturerLotNumber) = 0;

	virtual void GetManufacturerLotNumber(char** manufacturerLotNumber) = 0;

	/*
	*	\brief 制造商标示符,index 16
	*/
	virtual void SetManufacturerID(const char* manufacturerID) = 0;

	virtual void GetManufacturerID(char** manufacturerID) = 0;


	/*
	*	\brief 供应商标示符,index 17
	*/
	virtual void SetSupplierID(const char* supplierID) = 0;

	virtual void GetSupplierID(char** supplierID) = 0;

};

#ifdef WIN32
HL7_ENGINE_DLL_API IINV* CreateINV();
#else
IINV* CreateINV();
#endif

#endif