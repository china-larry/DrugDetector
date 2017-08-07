


#define STDDLL_API    __stdcall
extern "C" {
//#include "setupapi.h"
#include "hiddll.h"
}


__declspec(dllexport)  HANDLE STDDLL_API OpenMyHIDDevice(USHORT USB_PID,USHORT USB_VID);
__declspec(dllexport) unsigned int STDDLL_API WriteHidData(HANDLE hidHandle,BYTE * reportBuf,int len);
__declspec(dllexport) unsigned int STDDLL_API ReadHidData(HANDLE hidHandle,BYTE * recvDataBuf,int delaytime);
__declspec(dllexport) void STDDLL_API CloseDev(HANDLE hidHandle);

