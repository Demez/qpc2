#pragma once

#include <string>
#include <vector>
#include "util.h"
#include "project.h"
#include "project_manager.h"

class BaseGenerator;

// update this whenever the struct below is modified
constexpr unsigned char g_generatorInterfaceVer = 1;

struct GeneratorInterface
{
	void* mod;                                       // module dll handle
	const char* fileName;                            // filename of module
	unsigned char count = 1;                         // amount of generators in module
	BaseGenerator** genList;                         // all generators in module
};


typedef unsigned char (*FuncGetVersion)();
typedef GeneratorInterface* (*FuncGetInterface)();


class BaseGenerator: public PlatArchItem
{
public:
	BaseGenerator(std::string argName, std::string name)
	{
		m_argName = argName;
		m_name = name;
		str_upper(argName);
		m_macro = argName;
	}

	// Projects
	virtual void CreateProject(ProjectContainer* proj) {}
	virtual bool DoesProjectNeedRebuild(ProjectInfo* proj) { return false; }

	// Master Files
	virtual void CreateMasterFile(std::string &path) {}
	virtual bool DoesMasterFileNeedRebuild(std::string &path) { return false; }
	virtual bool GeneratesMasterFile() { return false; }
	virtual bool UsesMasterFileFolders() { return false; }

	// other
	virtual void PostProjectBuilding() {}

	std::string m_argName;
	std::string m_name;
	std::string m_macro;
};


#ifdef _WIN32
    #define DLL_EXPORT __declspec(dllexport)
    #define DLL_IMPORT __declspec(dllimport)
#elif __linux__
    #define DLL_EXPORT
    #define DLL_IMPORT
#endif


// Convienence macros
#define DECLARE_INTERFACE_VERSION() \
	extern "C" DLL_EXPORT unsigned char GetInterfaceVersion() \
	{ \
		return g_generatorInterfaceVer; \
	}

#define DECLARE_GENERATOR_INTERFACE(num) \
	extern "C" DLL_EXPORT GeneratorInterface* GetGeneratorInterface() \
	{ \
		GeneratorInterface* genInt = new GeneratorInterface; \
		genInt->count = num; \
		genInt->genList = new BaseGenerator*[num]; \
		size_t i = 0;

#define ADD_GENERATOR(genClass) \
		genInt->genList[i] = new genClass; \
		i++;

#define END_GENERATOR_INTERFACE() \
 		return genInt; \
	}


#define DECLARE_SINGLE_GENERATOR(genClass) \
	DECLARE_INTERFACE_VERSION() \
	DECLARE_GENERATOR_INTERFACE(1) \
	ADD_GENERATOR(genClass) \
	END_GENERATOR_INTERFACE()

