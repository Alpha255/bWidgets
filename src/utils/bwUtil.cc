#include "bwUtil.h"

#include <algorithm>

namespace bwUtils
{
	void toLowercase(std::string& str)
	{
		std::transform(str.begin(), str.end(), str.begin(), [](std::string::value_type c) {
			return (std::string::value_type)std::tolower(c);
		});
	}

	void toUppercase(std::string& str)
	{
		std::transform(str.begin(), str.end(), str.begin(), [](std::string::value_type c) {
			return (std::string::value_type)std::toupper(c);
		});
	}

	std::string lowercase(const std::string& str)
	{
		std::string result = str;
		toLowercase(result);
		return result;
	}

	std::string uppercase(const std::string& str)
	{
		std::string result = str;
		toUppercase(result);
		return result;
	}
}