#pragma once

#include "util.h"
#include "project.h"
#include "lexer.h"
#include <vector>
#include <unordered_map>
#include <regex>


class Config;
class ProjectPass;


class ConfigGroup
{
public:
	virtual void Init(Config* config)
	{
		cfg = config;
	}

	virtual void ParseOption(StringMap& macros, QPCBlock& option) = 0;

	Config* cfg;
};


class General: public ConfigGroup
{
public:

	std::string outName;
	std::string outNamePrefix;
	std::string outNamePostfix;

	std::string compiler;

	inline std::string GetOutName()
	{
		return outNamePrefix + outName + outNamePostfix;
	}

	void Init(Config* config);
	void ParseOption(StringMap& macros, QPCBlock& option);

};


class Compile: public ConfigGroup
{
public:

	// void Init(Config* config);
	void ParseOption(StringMap& macros, QPCBlock& option);
};


class Link: public ConfigGroup
{
public:

	// void Init(Config* config);
	void ParseOption(StringMap& macros, QPCBlock& option);
};


class Debug: public ConfigGroup
{
public:

	// void Init(Config* config);
	void ParseOption(StringMap& macros, QPCBlock& option);
};


class Config
{
public:
	Config(ProjectPass* proj, std::string name);
	// Config() {}

	ProjectPass* proj = NULL;
	std::string m_name;

	General general;
	Compile compile;
	Link link;
	Debug debug;

	void ParseOption(std::string &group, QPCBlock& option);
};


class SourceFileCompile: public Compile
{
public:
	bool build = true;
};


