#ifndef BASE_H__
#define BASE_H__

#ifdef HL7_ENGINE_DLL_BUILD
#define HL7_ENGINE_DLL_API		extern "C" __declspec (dllexport)
#define HL7_ENGINE_CLASS_DLL	__declspec (dllexport)
#else
#define HL7_ENGINE_DLL_API		extern "C" __declspec (dllimport)
#endif

#ifdef WIN32
/**	申请内存
*/
HL7_ENGINE_DLL_API void* GetMemory(int size);

/** 释放内存
*/
HL7_ENGINE_DLL_API void FreeMemory(void* p);


#else
/**	申请内存
*/
void* GetMemory(int size);

/** 释放内存
*/
void FreeMemory(void* p);
#endif

/**	自动使用lib文件
*/
//#ifndef HL7_ENGINE_DLL_BUILD
//#ifdef _DEBUG
//#define HL7ENGINE_LIB		"HL7Engine.lib"
//#else
//#define HL7ENGINE_LIB		"HL7Engine.lib"
//#endif
//
//#pragma comment(lib,HL7ENGINE_LIB)
//#pragma message("HL7Engine Library auto link to: " HL7ENGINE_LIB)
//#endif


/**
定义返回值，该返回值用于通用接口
*/
typedef int NRET;

#define N_OK					0			//正常
#define N_ERR					1			//一般性错误
#define N_NOTIMPL				2			//尚未实现方法，或者该功能不支持
#define N_UNKNOW_IF				3			//未知的接口
#define N_ERRPARAM				4			//错误的参数

/**
定义接口ID，采用INT来处理
*/
typedef int IFID;

#define IF_OBJECTBASE						0			//ObjectBase接口
#define IF_HMIMESSAGE						1
#define IF_HMIMESSAGELISTENER				2

			//GraphicsItem接口
#define IF_HL7ENGINEAPP						4
#define IF_HL7MESSAGE						5
#define IF_HL7SEGMENT						6



#define IF_PID								9
#define IF_OBR								10
#define IF_OBX								11
#define IF_DSC								12
#define IF_ERR								13
#define IF_MSA								14
#define IF_MSH								15
#define IF_QAK								16
#define IF_QRD								17
#define IF_QRF								18
#define IF_DSP								19
#define IF_ECD								20
#define IF_ECR								21
#define IF_EQP								22
#define IF_EQU								23
#define IF_ISD								24
#define IF_INV								25
#define IF_ROL								26

#define IF_ORU_R01							40
#define IF_DSR_Q03							41
#define IF_EAC_U07							42
#define IF_EAR_U08							43
#define IF_ESR_U02							44
#define IF_ESU_U01							45
#define IF_INR_U06							46
#define IF_INU_U05							47
#define IF_LSR_U13							48
#define IF_LSU_U12							49
#define IF_ORF_R04							50
#define IF_QCK_Q02							51
#define IF_QRY_Q01							52
#define IF_QRY_Q02							53
#define IF_QRY_R02							54
#define IF_ACK								55


#define IF_ORU_R01_PATIENT_RESULT			70
#define IF_ORF_R04_PATIENT_RESULT			71

#define IF_OBJECTLIST						100
#define IF_STRINGLIST						101
#define IF_PROPERTYLIST						102
#define IF_RESINFOLIST						103
#define IF_HMIACTION						200
#define IF_CRDEVICEOUTPUTACTION				201
#define IF_NAVIGATIONACTION					202
#define IF_VIEWLAYOUT						203
#define IF_IVIEWEDITACTION					204


typedef unsigned int UINT;

typedef unsigned int type_t;

#define TypeObjectBase					0
#define TypeObjectCloneAble				1
#define TypeMessageListener				2
#define TypeMessage						3
#define TypeGraphicsItem				4
#define TypeNanoHmiApp					5
#define TypeView						6
#define TypeProject						7
#define TypePage						8
#define TypeObjectList					9
#define TypeStringList					10
#define TypePropertyMgr					11
#define TypePropertyList				12
#define TypeResInfoList					13
#define TypeItemData					14
#define TypeCategoryList                15
#define TypeHmiAction					200

//无效子类型
#define SubTypeInvaild					0


#define  SEGMENT_SEPARATOR 0x0D

enum MessageState
{
	STATE_OK = 0,//信息被接受
	STATE_SEGMENT_SEQUENCE_ERROR = 100,//信息序列出错，或者段丢失
	STATE_FIELD_MISSING_ERROR = 101,//字段丢失
	STATE_DATA_TYPE_ERROR = 102,//数据类型错误
	STATE_VALUE_NO_FOUND_ERROR = 103,//未发现响应的表格中的取值
	STEATE_MESSAGE_TYPE_NO_SUPPORT = 200,//不支持的信息类型
	STATE_EVENT_NO_SUPPORT = 201,//不支持的事件代码
	STATE_HANDLE_ID_NO_SUPPORT = 202,//不支持的处理ID
	STATE_VERSION_ID_NO_SUPPORT = 203,//不支持的版本ID
	STATE_UNKNOW_ID = 204,//不认识的关键标识符
	STATE_ID_REPEAT = 205,//关键标识符出现重复
	STATE_LOCKED = 206,//应用程序锁定
	STATE_OTHER_ERROR = 207//以上错误不能覆盖的其他错误
};

#endif
