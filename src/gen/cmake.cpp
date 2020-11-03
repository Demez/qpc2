#include "util.h"
#include "base_generator.h"
#include "project_manager.h"
#include <stdarg.h>
#include <iostream>
#include <fstream>


inline std::string StrEqual(std::string str1, std::string str2)
{
	return "(" + str1 + " STREQUAL " + str2 + ")";
}


std::string QuoteAbsPath(std::string path)
{
	return "\"" + GetCurrentDir() + "/" + path + "\"";
}


std::string JoinVecToString(std::vector<std::string> &vec, const char* joinChar = " ")
{
	std::string result;
	bool first = true;

	for (std::string item: vec)
	{
		// dumb
		if (!first)
		{
			result += joinChar;
		}

		first = false;
		result += "\"" + item + "\"";
	}

	return result;
}


std::string JoinStringAdv(std::vector<std::string> &vec, const char* startChar = " ", const char* endChar = " ")
{
	std::string result;

	for (size_t i = 0; i < vec.size(); i++)
	{
		result += startChar + vec[i];

		if (i + 1 < vec.size())
		{
			result += endChar;
		}
	}

	return result;
}


std::string JoinStringPath(std::vector<std::string> vec, const char* joinChar = " ")
{
	std::string result;
	bool first = true;

	for (std::string item: vec)
	{
		ReplaceString(item, "\\", "/");

		// dumb
		if (!first)
		{
			result += joinChar;
		}

		first = false;
		// result += "\"" + item + "\"";
		result += QuoteAbsPath(item);
	}

	return result;
}


std::string GenOption(size_t amount, std::string target, ...)
{
	std::string option = "\t" + target + "(";

	va_list args;
	va_start(args, target);
	{
		for (size_t i = 0; i < amount; i++)
		{
			std::string nextArg = va_arg(args, const char*);
			option += "\n\t\t" + nextArg;
		}
	}
	va_end(args);

	return option + "\n\t)\n\n";
}


class CMakeGenerator: public BaseGenerator
{
public:
	CMakeGenerator(): BaseGenerator("cmake", "CMakeLists.txt Generator")
	{
		AddPlatform(Platform::WINDOWS);
		AddPlatform(Platform::LINUX);
		AddPlatform(Platform::MACOS);

		AddArch(Arch::I386);
		AddArch(Arch::AMD64);
		AddArch(Arch::ARM);
		AddArch(Arch::ARM64);
	}

	std::vector<std::string> dirs;

	bool DoesProjectNeedRebuild(ProjectInfo* proj)
	{
		return true;
	}

	void CreateProject(ProjectContainer* proj)
	{
		// TODO: this will be changed later
		std::vector<ProjectPass*> passes = proj->m_passes;

		printf("Generating CMakeLists.txt for project");

		std::string mainCMakeLists = "";

		if (!vec_contains(dirs, GetCurrentDir()))
		{
			dirs.push_back(GetCurrentDir());
			mainCMakeLists += GenCMakeDeclaration();
		}
		else if (fs::is_regular_file("CMakeLists.txt"))
		{
			std::ifstream fileStream("CMakeLists.txt", std::ifstream::in);
            
            std::string previousCMakeLists(std::istream_iterator<char>(fileStream),
                                           std::istream_iterator<char>());
            mainCMakeLists += previousCMakeLists;
		}

		for (size_t i = 0; i < passes.size(); i++)
		{
			if (i > 0)
			{
				mainCMakeLists += "else";
			}

			ProjectPass* pass = passes[i];

			mainCMakeLists += "if( " + 
				StrEqual("CMAKE_BUILD_TYPE", pass->cfg.m_name) + " AND " +
				StrEqual("QPC_PLATFORM", PlatformToStr(pass->m_platform)) + " AND " +
				StrEqual("QPC_ARCH", ArchToStr(pass->m_arch)) + " )\n";

			mainCMakeLists += HandleProjectPass(proj->info, pass);
		}

		mainCMakeLists += "endif()\n\n";

		// output the file
		FILE* f = fopen("CMakeLists.txt", "w");

		if (f == NULL)
		{
			printf("ERROR: Failed to open file handle for output cmake file\n");
			return;
		}

		fputs( mainCMakeLists.c_str(), f );

		fclose(f);
	}

	std::string GenCMakeDeclaration()
	{
		return "cmake_minimum_required(VERSION 3.5)\n\n";
	}

	void HandleIncLibDirs(std::string &cmakeLists, std::string &projName, ProjectPass* proj, std::vector<std::string> &vec, std::string type)
	{
		if (!vec.empty())
		{
			cmakeLists += GenOption( 2, "target_" + type, (projName + " PRIVATE").c_str(), JoinStringPath(vec, "\n\t\t").c_str() );
		}
	}

	void HandleDependencies(std::string &cmakeLists, ProjectPass* proj, std::string &projName)
	{
		if (proj->m_container->info->m_deps.empty())
		{
			return;
		}

		std::vector<std::string> deps;
		ProjectManager* manager = GetProjManager();

		for (ProjectInfo* info: proj->m_container->info->m_deps)
		{
			if (vec_contains(manager->m_buildList, info))
			{
				std::string dep = info->m_name;
				str_upper(dep);
				deps.push_back(dep);
			}
		}

		if (!deps.empty())
		{
			cmakeLists += GenOption( 2, "add_dependencies", projName.c_str(), JoinVecToString(deps, "\n\t\t").c_str() );
		}
	}

	void HandleLibraries(std::string &cmakeLists, ProjectPass* proj, std::string &projName)
	{
		if (proj->cfg.link.libs.empty())
		{
			return;
		}

		std::vector<std::string> libs;

		for (std::string lib: proj->cfg.link.libs)
		{
			fs::path fsLib = lib;
			if (fsLib.has_parent_path())
			{
				if (!fsLib.has_extension())
				{
					lib += proj->m_macros["EXT_LIB"];
				}

				libs.push_back(GetCurrentDir() + "/" + lib);
			}
			else
			{
				libs.push_back(lib);
			}
		}

		if (!libs.empty())
		{
			cmakeLists += GenOption( 2, "target_link_libraries", projName.c_str(), JoinVecToString(libs, "\n\t\t").c_str() );
		}
	}

	std::string HandleProjectPass(ProjectInfo* info, ProjectPass* proj)
	{
		std::string cmakeLists = "";

		std::string projName = proj->m_macros["PROJECT_SCRIPT_NAME"];
		str_upper(projName);

		std::string target = "add_library";
		std::string targetType = "";
		std::string cmakeOutputDir = "RUNTIME_OUTPUT_DIRECTORY";

		if (proj->cfg.general.configType == ConfigType::APPLICATION)
		{
			target = "add_executable";
		}
		else if (proj->cfg.general.configType == ConfigType::STATIC_LIB)
		{
			targetType = " STATIC";
			cmakeOutputDir = "ARCHIVE_OUTPUT_DIRECTORY";
		}
		else if (proj->cfg.general.configType == ConfigType::DYNAMIC_LIB)
		{
			targetType = " SHARED";
		}

		cmakeLists += "\n";

		cmakeLists += GenOption( 2, "set", (projName + "_SRC_FILES").c_str(), JoinStringPath(proj->GetSourceFileList(), "\n\t\t").c_str() );
		cmakeLists += GenOption( 2, "set", (projName + "_INC_FILES").c_str(), JoinStringPath(proj->GetHeaderFileList(), "\n\t\t").c_str() );

		cmakeLists += GenOption(
			3,
			target,
			(projName + targetType).c_str(),
			("${" + projName + "_SRC_FILES}").c_str(),
			("${" + projName + "_INC_FILES}").c_str()
		);

		std::string lang = proj->cfg.general.lang == Language::CPP ? "CXX" : "C";
		std::string outputDir = proj->cfg.general.outDir;

		if (!proj->cfg.link.outFile.empty())
		{
			// fs::path fsOutputDir = proj->cfg.link.outFile;
			// outputDir = fsOutputDir.parent_path().string();
			outputDir = fs::path(proj->cfg.link.outFile).parent_path().string();
		}

		std::map<std::string, std::string> targetProps = {
			{"PREFIX",                      "\"" + proj->cfg.general.outNamePrefix + "\""},
			{"OUTPUT_NAME",                 "\"" + proj->cfg.general.outName + "\""},
			{lang + "_COMPILER",            "\"" + proj->cfg.general.compiler + "\""},
			{lang + "_STANDARD",            "\"" + StandardToNum(proj->cfg.general.standard) + "\""},
			// {lang + "_STANDARD_REQUIRED",   "\"YES\""},
			{cmakeOutputDir,                "\"" + outputDir + "\""},
			{cmakeOutputDir + "_RELEASE",   "\"" + outputDir + "\""},
			{cmakeOutputDir + "_DEBUG",     "\"" + outputDir + "\""},
		};

		if (!proj->cfg.link.impLib.empty())
		{
			fs::path fsImpLibDir = proj->cfg.link.impLib;
			std::string archiveOutputDir = QuoteAbsPath(fsImpLibDir.parent_path().string());
			targetProps["ARCHIVE_OUTPUT_DIRECTORY"] = archiveOutputDir;
			targetProps["ARCHIVE_OUTPUT_DIRECTORY_RELEASE"] = archiveOutputDir;
			targetProps["ARCHIVE_OUTPUT_DIRECTORY_DEBUG"] = archiveOutputDir;
		}
		else if (proj->cfg.general.configType != ConfigType::STATIC_LIB)
		{
			std::string archiveOutputDir = QuoteAbsPath(proj->cfg.general.buildDir);
			targetProps["ARCHIVE_OUTPUT_DIRECTORY"] = archiveOutputDir;
			targetProps["ARCHIVE_OUTPUT_DIRECTORY_RELEASE"] = archiveOutputDir;
			targetProps["ARCHIVE_OUTPUT_DIRECTORY_DEBUG"] = archiveOutputDir;
		}

		std::string targetPropsStr;

		bool first = true;
		for (auto const&[k, v]: targetProps)
		{
			if (!first)
			{
				targetPropsStr += "\n\t\t";
			}

			first = false;
			targetPropsStr += k + " " + v;
		}

		cmakeLists += GenOption( 2, "set_target_properties", (projName + " PROPERTIES").c_str(), targetPropsStr.c_str() );

		HandleDependencies(cmakeLists, proj, projName);

		HandleIncLibDirs(cmakeLists, projName, proj, proj->cfg.compile.incDirs, "include_directories");
		HandleIncLibDirs(cmakeLists, projName, proj, proj->cfg.link.libDirs, "link_directories");

		HandleLibraries(cmakeLists, proj, projName);

		if (!proj->cfg.compile.defines.empty())
		{
			cmakeLists += GenOption( 2, "target_compile_definitions", (projName + " PRIVATE").c_str(), JoinStringAdv(proj->cfg.compile.defines, "-D", "\n\t\t").c_str() );
		}

		if (!proj->cfg.compile.options.empty())
		{
			cmakeLists += GenOption( 2, "target_compile_options", (projName + " PRIVATE").c_str(), JoinStringAdv(proj->cfg.compile.options, "", "\n\t\t").c_str() );
		}

		std::vector<std::string> linkOptions;

		if (!proj->cfg.link.ignoreLibs.empty())
		{
			// TODO: use the cmd gen when that's made
			if (proj->cfg.general.compiler.substr(0, 4) == "msvc")
			{
				for (std::string ignoreLib: proj->cfg.link.ignoreLibs)
				{
					linkOptions.push_back("/NODEFAULTLIB:" + ignoreLib);
				}
			}
			else
			{
				// god awful
				linkOptions.push_back( "--exclude-libs," + JoinStringAdv(proj->cfg.link.ignoreLibs, ",", "") );
			}
		}

		if (!proj->cfg.link.options.empty())
		{
			linkOptions.reserve( linkOptions.size() + std::distance( proj->cfg.link.options.begin(), proj->cfg.link.options.end() ) );
			linkOptions.insert( linkOptions.end(), proj->cfg.link.options.begin(), proj->cfg.link.options.end() );
		}

		if (!linkOptions.empty())
		{
			cmakeLists += GenOption( 2, "target_link_options", (projName + " PRIVATE").c_str(), JoinStringAdv(linkOptions, "", "\n\t\t").c_str() );
		}

		return cmakeLists;
	}
};


DECLARE_SINGLE_GENERATOR(CMakeGenerator)




