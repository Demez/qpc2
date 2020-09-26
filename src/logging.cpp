#include "logging.h"


int g_warningCount = 0;


// this will not work on first use
const char* FormatStringVA(const char* str, va_list args)
{
	char* finalStr = "";

	va_start(args, str);
	{
		str = va_arg(args, const char*);
		sprintf(finalStr, str);
	}
	va_end(args);

	return finalStr;
}


const char* FormatString(const char* str, ...)
{
	va_list args;
	return FormatStringVA(str, args);
}

