#ifndef _HL7_PARSER_H_
#define _HL7_PARSER_H_

#ifdef WIN32
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif
#endif

#include <string>
#include "IHL7Message.h"

#ifdef WIN32

HL7_ENGINE_DLL_API IHL7Message* Parse(std::string messageStr);

#else
IHL7Message* Parse(std::string messageStr);

#endif//_WINDOW_PLATFORM_
#endif //_HL7_PARSER_H_
