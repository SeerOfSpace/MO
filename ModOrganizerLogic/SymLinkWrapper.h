#pragma once
#ifndef __SymLinkwrapper_H_Included__
#define __SymLinkwrapper_H_Included__

#include <string>
#include <Windows.h>

class SymLinkWrapper {
public:
	static bool createSymLink(std::string, std::string);
	static bool deleteSymLink(std::string);
	static bool isSymLink(std::string);

private:
	static bool convertString(const std::string&, LPWSTR&);

};

#endif