#pragma once

#include "util.h"
#include "project.h"
#include <vector>
#include <unordered_map>
#include <regex>


class Config;
class ProjectPass;


enum class ProjError
{
	NONE = 0,
	INVALID_OPTION,
	EMPTY_VALUE,
	COUNT
};

enum class EPCH
{
	INVALID = 0,
	NONE,
	CREATE,
	USE,
	COUNT
};

enum class ConfigType
{
	INVALID = 0,
	DYNAMIC_LIB,
	STATIC_LIB,
	APPLICATION,
	COUNT
};

enum class Language
{
	INVALID = 0,
	CPP,
	C,
	COUNT
};

enum class Standard
{
	INVALID = 0,

	CPPLATEST,
	CPP17,
	CPP14,
	CPP11,
	CPP03,
	CPP98,

	C11,
	C99,
	C95,
	C90,
	C89,

	COUNT
};


std::string StandardToNum(Standard standard);


class ConfigGroup
{
public:
	virtual void Init(Config* config)
	{
		cfg = config;
	}

	Config* cfg;
};


class General: public ConfigGroup
{
public:

	std::string outDir;
	std::string buildDir;  // maybe change to objDir?

	std::string outName;
	std::string outNamePrefix;
	std::string outNamePostfix;

	std::string compiler;

	ConfigType configType;
	Language lang;
	Standard standard;

	std::vector<std::string> options;

	inline std::string GetOutName()
	{
		return outNamePrefix + outName + outNamePostfix;
	}

	ProjError SetConfigType(std::string value);
	ProjError SetConfigType(ConfigType value);

	ProjError SetLanguageAndStandard(std::string value);

	ProjError SetLanguage(std::string value);
	ProjError SetLanguage(Language value);

	ProjError SetStandard(Standard value);

	void Init(Config* config);

};


class Compile: public ConfigGroup
{
public:
	bool defaultIncDirs = true;

	EPCH pch;
	std::string pchFile;
	std::string pchOut;

	std::vector<std::string> defines;
	std::vector<std::string> incDirs;
	std::vector<std::string> options;

	ProjError SetPCH(EPCH value);
	ProjError SetPCH(std::string value);

	ProjError SetDefaultIncDirs(std::string &value);
};


class Link: public ConfigGroup
{
public:
	bool defaultLibDirs = true;
	bool ignoreImpLib = false;

	std::string outFile;
	std::string dbgFile;
	std::string impLib;

	std::string entryPoint;

	std::vector<std::string> ignoreLibs;
	std::vector<std::string> libs;
	std::vector<std::string> libDirs;
	std::vector<std::string> options;

	ProjError SetDefaultLibDirs(std::string &value);
	ProjError SetIgnoreImpLib(std::string &value);
};


class Debug: public ConfigGroup
{
public:
	std::string cmd;
	std::string args;
	std::string cwd;
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
};


class SourceFileCompile: public Compile
{
public:
	bool build = true;
};


