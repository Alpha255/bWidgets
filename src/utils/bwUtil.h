#pragma once

/**
 * Portable version of __PRETTY_FUNCTION__.
 */
#ifdef _MSC_VER
#  define PRETTY_FUNCTION __FUNCSIG__
#else
#  define PRETTY_FUNCTION __PRETTY_FUNCTION__
#endif

#include <string>
#include <algorithm>

namespace bwUtils
{
	void toLowercase(std::string& str);

	void toUppercase(std::string& str);

	std::string lowercase(const std::string& str);

	std::string uppercase(const std::string& str);
}
