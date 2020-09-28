#pragma once

#include "util.h"
#include <stdio.h>
#include <stdarg.h>
#include <string>


extern int g_warningCount;


// this will not work on first use
const char* FormatStringVA(const char* str, va_list args);
const char* FormatString(const char* str, ...);


inline void warningFinal(const char* str)
{
	std::string finalStr = "[WARNING] ";
	puts( (finalStr + str + "\n").c_str() );
	g_warningCount++;
}


inline void warningVA(const char* str, va_list args)
{
	/*char argsStr[1024] = "";
	sprintf(argsStr, "%s", str);
	va_start(args, str);
	sprintf(argsStr, "%s%s", argsStr, va_arg(args, const char*));
	va_end(args);

	warningFinal(argsStr);*/
}


inline void warning(const char* str, ...)
{
	va_list args;
	char argsFull[1024] = {};
	va_start(args, str);
	sprintf(argsFull, str, va_arg(args, const char*));
	va_end(args);

	warningFinal(argsFull);
}



