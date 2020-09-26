#pragma once

#include "util.h"
#include <vector>
#include <unordered_map>
#include "config.h"

class ProjectPass;
class Config;
class SourceFileCompile;



extern std::vector<std::string> g_extsSource;
extern std::vector<std::string> g_extsHeader;


enum class FileType
{
	SOURCE,
	HEADER,
	OTHER,
};


FileType GetFileType(std::string& filePath);


struct File
{
	std::string path;
	std::string folder;
	FileType type;
};


struct SourceFile: public File
{
	SourceFileCompile compile;
};



class ProjectContainer
{
public:
	ProjectContainer(fs::path path);
	~ProjectContainer();

	std::vector<ProjectPass*> m_passes;
};



class ProjectPass
{
public:
	ProjectPass(ProjectContainer* container, std::string config, Platform platform, Arch arch, StringMap &macros);
	~ProjectPass() {}

	bool AddDependency(std::string &filePath);
	bool RemoveDependency(std::string &filePath);

	bool AddMacro(std::string key, std::string value);

	bool AddFile(std::string &filePath, std::string &folder, FileType type);
	// bool AddFileGlob(std::string &filePath, std::string &folder, FileType &type);
	// bool RemoveFile(std::string &filePath);
	// bool RemoveFileGlob(std::string &filePath);

	bool IsFileAdded(std::string &filePath);
	bool IsFileAdded(std::string &filePath, FileType type);

	template <class T = File>
	bool IsFileAddedInternal(std::string &filePath, std::vector<T> &files);

	Config cfg;

	StringMap m_macros;

	std::vector<SourceFile> m_sourceFiles;
	std::vector<File> m_headerFiles;
	std::vector<File> m_files;
	std::vector<std::string> m_deps;

	Arch m_arch;
	Platform m_platform;
	ProjectContainer* m_container;
};

