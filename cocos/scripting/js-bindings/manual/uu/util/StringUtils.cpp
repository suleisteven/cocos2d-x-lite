#include "StringUtils.h"
#include <stdarg.h>

std::string StringUtils::format(const char* format, ...)
{
#define CC_MAX_STRING_LENGTH (1024*100)

	std::string ret;

	va_list ap;
	va_start(ap, format);

	char* buf = (char*)malloc(CC_MAX_STRING_LENGTH);
	if (buf != nullptr)
	{
		vsnprintf(buf, CC_MAX_STRING_LENGTH, format, ap);
		ret = buf;
		free(buf);
	}
	va_end(ap);

	return ret;
}
