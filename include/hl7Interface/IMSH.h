#ifndef IMSH_H
#define IMSH_H
#ifdef WIN32
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif
#endif

/// Represents an HL7 MSH message segment. 
/// This segment has the following fields:<ol>
///<li>MSH-1: Field Separator (ST)</li>
///<li>MSH-2: Encoding Characters (ST)</li>
///<li>MSH-3: Sending Application (ST)</li>
///<li>MSH-4: Sending Facility (HD)</li>
///<li>MSH-5: Receiving Application (ST)</li>
///<li>MSH-6: Receiving Facility (HD)</li>
///<li>MSH-7: Date/Time Of Message (TS)</li>
///<li>MSH-8: Security (ST), Empty and Keep back</li>
///<li>MSH-9: Message Type (MSG)</li>
///<li>MSH-10: Message Control ID (ST)</li>
///<li>MSH-11: Processing ID (ST)</li>
///<li>MSH-12: Version ID (VID)</li>
///<li>MSH-13: Sequence Number (NM), Empty and Keep back</li>
///<li>MSH-14: Continuation Pointer (ST), Empty and Keep back</li>
///<li>MSH-15: Accept Acknowledgment Type (ID), Empty and Keep back</li>
///<li>MSH-16: Application Acknowledgment Type (ID)</li>
///<li>MSH-17: Country Code (ID)</li>
///<li>MSH-18: Character Set (ST)</li>
///<li>MSH-19: Principal Language Of Message (CE), Empty and Keep back</li>
///<li>MSH-20: Alternate Character Set Handling Scheme (ID), Empty and Keep back</li>
///<li>MSH-21: Conformance Statement ID (ID), Empty and Keep back</li>

#include "IObjectBase.h"
class IMSH : public IObjectBase
{
public:

	/*
	 *	\brief 发送应用程序，index 1
	 */
	virtual void SetSendApp(const char* sendApp) = 0;
	
	virtual void GetSendApp(char** sendApp) = 0;

	/*
	*	\brief 设置发送设备名称，index 2
	*/
	virtual void SetSendFacilityName(const char* sendFacilityName)= 0;

	virtual void GetSendFacilityName(char** sendFacilityName) = 0;

	/*
	*	\brief 设置发送设备唯一标识符，index 2
	*/
	virtual void SetSendFacilityID(const char* sendFacilityID) = 0;

	virtual void GetSendFacilityID(char** sendFacilityID) = 0;
	/*
	*	\brief 设置发送设备型号，index 2
	*/
	virtual void SetSendFacilityType(const char* sendFacilityType) = 0;

	virtual void GetSendFacilityType(char** sendFacilityType) = 0;

	/*
	 *	\brief 设置接收应用程序，index 3
	 */
	virtual void SetRecvApp(const char* recvApp) = 0;
	
	virtual void GetRecvApp(char** recvApp) = 0;
	/*
	*	\brief 设置接收设备名称，index 4
	*/
	virtual void SetRecvFacilityName(const char* recvFacilityName) = 0;

	virtual void GetRecvFacilityName(char** recvFacilityName) = 0;

	/*
	*	\brief 设置接收设备唯一标识符，index 4
	*/
	virtual void SetRecvFacilityID(const char* recvFacilityID) = 0;
	
	virtual void GetRecvFacilityID(char** recvFacilityID) = 0;
	/*
	*	\brief 设置接收设备型号，index 4
	*/
	virtual void SetRecvFacilityType(const char* recvFacilityType) = 0;
	
	virtual void GetRecvFacilityType(char** recvFacilityType) = 0;

	/*
	 *	\brief 信息的日期时间，index 5
	 */
	virtual void SetDate(const char* date) = 0;

	virtual void GetDate(char** date) = 0;

	/*
	*	\brief 消息类型， index 7
	*/
	virtual void SetMessageType(const char* msgType) = 0;

	virtual void GetMessageType(char** messageType) = 0;
	/*
	*	\brief 消息类型，index 7
	*/
	virtual void SetTriggerEvent(const char* triggerEvent) = 0;

	virtual void GetTriggerEvent(char** triggerEvent) = 0;
	/*
	*	\brief 消息结构, index 7
	*/
	virtual void SetMessageStructure(const char* msgStructure) =0;

	virtual void GetMessageStructure(char** msgStructure) = 0;

	/*
	 *	\brief 信息控制ID，index 8
	 */
	virtual void SetMsgCtrlID(const char* msgCtrlID) = 0;

	virtual void GetMsgCtrlID(char** msgCtrlID) = 0;

	/*
	 *	\brief 处理ID号,index 9
	 */
	virtual void SetProcessingID(const char* processingID) = 0;

	virtual void GetProcessingID(char** processingID) = 0;

	/*
	 *	\brief 版本ID号，index 10
	 */
	virtual void SetVersionID(const char* versionID) = 0;

	virtual void GetVersionID(char** versionID) = 0;

	/*
	 *	\brief 应用程序确认类型，index 14
	 */
	virtual void SetApplicationAckType(const char* applicationAckType) = 0;

	virtual void GetApplicationAckType(char** applicationAckType) = 0;

	/*
	 *	\brief 国家代码，index 15
	 */
	virtual void SetCountryCode(const char* countryCode) = 0;
	
	virtual void GetCountryCode(char** countryCode) = 0;
	/*
	 *	\brief 字符集，index 16
	 */
	virtual void SetCharacterSet(const char* charset) = 0;

	virtual void GetCharacterSet(char** characterSet) = 0;

};


#ifdef WIN32
HL7_ENGINE_DLL_API IMSH* CreateMSH();
#else
IMSH* CreateMSH();
#endif

#endif
