#pragma once

#include <string>

class StringConverter
{
public:
	static std::wstring StringToWide(std::string str)
	{
		return std::wstring(str.cbegin(), str.cend());
		
	}
};