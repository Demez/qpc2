#include "util.h"
#include "logging.h"
#include "generator_handler.h"

#include <fstream>
#include <iostream>


// junk stuff for other platforms that we need to stub for other platforms
#ifndef _WIN32
	typedef void* Module;
#endif


#ifdef _WIN32
	#include <libloaderapi.h>

	#define LOAD_LIBRARY(path) LoadLibraryA(path)
	#define LOAD_FUNC GetProcAddress
	#define EXT_DLL ".dll"
	typedef HMODULE Module;
#elif __linux__
	#include <unistd.h>
	#include <dlfcn.h>

	#define LOAD_LIBRARY(path) dlopen(path, RTLD_LAZY)
	#define LOAD_FUNC dlsym
	#define EXT_DLL ".so"
#endif


GeneratorHandler::GeneratorHandler()
{
}


GeneratorHandler::~GeneratorHandler()
{
	for (BaseGenerator* gen: m_generators)
	{
		delete gen;
	}

	m_generators.clear();

	for (GeneratorInterface* genInt: m_genIntList)
	{
		delete genInt;
	}

	m_genIntList.clear();
}


// TODO: replace this with reading from a file with a list of generators in it
void GeneratorHandler::LoadGenerators()
{
	fs::path exePath = g_exePath;
	fs::path exeDir = exePath.parent_path();

	// search for generators in this folder
	// like vstudio.dll or cmake.dll
	for( auto& p: fs::directory_iterator(exeDir) )
	{
		if (p.is_regular_file())
		{
			fs::path filePath = p.path();
			if (!filePath.has_extension())
			{
				continue;
			}

			if (filePath.extension() == EXT_DLL)
			{
				LoadGeneratorModule(filePath);
			}
		}
	}
}


void GeneratorHandler::LoadGeneratorModule(fs::path &filePath)
{
	std::string filePathStr = filePath.string();
	const char* filePathC = filePathStr.c_str();

	std::string fileNameStr = filePath.filename().string();
	const char* fileNameC = fileNameStr.c_str();

	Module mod = LOAD_LIBRARY(filePathC);

	if (!mod)
	{
		warning("Failed to load module \"%s\"", filePathC);
		return;
	}

	FuncGetInterface funcGetInterface = (FuncGetInterface)LOAD_FUNC(mod, "GetGeneratorInterface");

	if (!funcGetInterface)
	{
		warning("Failed to find GetGeneratorInterface function in \"%s\"", fileNameC);
		return;
	}

	GeneratorInterface* genInt = funcGetInterface();

	if (genInt->ver < g_generatorInterfaceVer)
	{
		printf("Generator Interface \"%s\" is outdated (Generator is ver %hhu, current ver is %hhu)", fileNameC, genInt->ver, g_generatorInterfaceVer);
		return;
	}

	for (int i = 0; i < genInt->count; i++)
	{
		BaseGenerator* gen = genInt->genList[i];
		m_generators.push_back(gen);
	}

	delete genInt->genList;

	genInt->mod = (void*)mod;
	genInt->fileName = fileNameC;

	m_genIntList.push_back(genInt);
}



