#pragma once
#ifndef __Config_H_Included__
#define __Config_H_Included__

#include <string>
#include <vector>

class Config {
public:
	static std::vector<std::string> readConfig(std::vector<std::string>);
private:
	static int getLength(const std::string*);
};

#endif