#pragma once

#include "util.h"


// bruh moment
class Args
{
public:
	std::string rootDir;
	std::string baseFile;
	std::string masterFile;

	bool verbose = false;
	bool force = false;
	bool forceMaster = false;
	bool hideWarnings = false;

	std::vector<std::string> add;
	std::vector<std::string> remove;

	std::vector<std::string> addTree;
	std::vector<std::string> addDepend;

	std::vector<std::string> generators;
	std::vector<std::string> configs;
	std::vector<Platform> platforms;
	std::vector<Arch> archs;
	StringMap macros;
};


typedef int (*EnumParamConvertFunc)(std::string);


class ArgParser
{
public:
	void ParseArgs(int argc, const char** argv);

	bool CheckOtherArg(int i);

	bool CheckParam(const char* shortHand, const char* value);

	const char*                 GetParamValueStr(const char* name, const char* shortHand, const char* defaultValue = "");
	std::vector<std::string>    GetParamList(const char* name, const char* shortHand, std::vector<std::string> defaultValue = {}, std::vector<std::string> choices = {});
	StringMap                   GetParamStringMap(const char* name, const char* shortHand);

	template <class T>
	std::vector<T> GetParamList(const char* name, const char* shortHand, EnumParamConvertFunc func, std::vector<T> defaultValue);

	int argc;
	const char** argv;
};


// might benefit from inline?
inline Args& GetArgs()
{
	static Args args;
	return args;
}


inline std::vector<Platform>    GetArgPlatforms()   { return GetArgs().platforms; }
inline std::vector<Arch>        GetArgArchs()       { return GetArgs().archs; }
inline std::vector<std::string> GetArgConfigs()     { return GetArgs().configs; }
inline std::vector<std::string> GetArgGenerators()  { return GetArgs().generators; }



