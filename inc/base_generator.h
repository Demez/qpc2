#pragma once

#include <string>
#include <vector>
#include "util.h"

class BaseGenerator: PlatArchItem
{
public:
	BaseGenerator(std::string argName, std::string name);

	// void Register(int id);

	std::string m_argName;
	std::string m_name;
};


