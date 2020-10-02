#include "util.h"
#include "args.h"
#include "project_manager.h"
#include "project.h"
#include "config.h"



FileType GetFileType(std::string& filePath)
{
	std::string fileExt = fs::path(filePath).extension().string();

	if (CheckExtSource(fileExt))
		return FileType::SOURCE;
	else if (CheckExtHeader(fileExt))
		return FileType::HEADER;

	return FileType::OTHER;
}


ProjectContainer::ProjectContainer(fs::path path)
{
	ProjectManager& manager = GetProjManager();

	std::string dir = path.parent_path().string();
	fs::path name = path.filename();

	std::string nameNoExt = name.string();
	ReplaceString(nameNoExt, name.extension().string(), "");

	StringMap macros = {
		{ "ROOT_DIR_ABS",           GetArgs().rootDir },
		{ "ROOT_DIR",               path.relative_path().string() },
		{ "PROJECT_NAME",           nameNoExt },
		{ "PROJECT_SCRIPT_NAME",    nameNoExt },
		{ "PROJECT_DIR",            dir },
		{ "SCRIPT_NAME",            nameNoExt },
		{ "SCRIPT_DIR",             dir },
	};

	macros.insert(GetArgs().macros.begin(), GetArgs().macros.end());

	// awful, but i also need to add another for loop for generators with macros later
	for (std::string config: manager.m_configs)
	{
		for (Platform platform: GetArgPlatforms())
		{
			for (Arch arch: GetArgArchs())
			{
				ProjectPass* pass = new ProjectPass(this, config, platform, arch, macros);
				m_passes.push_back(pass);
			}
		}
	}
}


ProjectContainer::~ProjectContainer()
{
	for (ProjectPass* pass: m_passes)
	{
		delete pass;
	}
}


ProjectPass::ProjectPass(ProjectContainer* container, std::string config, Platform platform, Arch arch, StringMap &macros):
	cfg(this, config)
{
	m_container = container;
	m_platform = platform;
	m_arch = arch;
	m_macros = macros;

	m_macros["CONFIG"] = config;
	std::transform(config.begin(), config.end(), config.begin(), ::toupper);
	m_macros[config] = "1";

	SetPlatformMacros(m_macros, platform);
	SetArchMacros(m_macros, arch);
}


ProjectPass::~ProjectPass()
{
	RemoveAllFilesInternal(m_sourceFiles);
	RemoveAllFilesInternal(m_headerFiles);
	RemoveAllFilesInternal(m_files);
}


bool ProjectPass::AddFile(std::string &filePath, const std::string &folder, FileType type)
{
	if (IsFileAdded(filePath))
		return false;

	if (type == FileType::SOURCE)
	{
		SourceFile* file = new SourceFile{filePath, folder, type};
		m_sourceFiles.push_back(file);
	}
	else
	{
		File* file = new File{filePath, folder, type};
		
		if (type == FileType::HEADER)
			m_headerFiles.push_back(file);
		else
			m_files.push_back(file);
	}

	return true;
}


bool ProjectPass::IsFileAdded(std::string &filePath)
{
	if (IsFileAddedInternal(filePath, m_sourceFiles))
		return true;

	if (IsFileAddedInternal(filePath, m_headerFiles))
		return true;

	if (IsFileAddedInternal(filePath, m_files))
		return true;

	return false;
}


bool ProjectPass::IsFileAdded(std::string &filePath, FileType type)
{
	return false;
}


template <class T>
bool ProjectPass::IsFileAddedInternal(std::string &filePath, std::vector<T*> &files)
{
	for (T* file: files)
	{
		if (file->path == filePath)
		{
			return true;
		}
	}

	return false;
}


template <class T>
void ProjectPass::RemoveAllFilesInternal(std::vector<T*> &files)
{
	for (T* file: files)
	{
		delete file;
	}

	files.clear();
}


template <class T>
void ProjectPass::RemoveFileInternal(std::string &filePath, std::vector<T*> &files)
{
	for (T* file: files)
	{
		if (file->path == filePath)
		{
			delete file;
			return;
		}
	}
}


bool ProjectPass::AddDependency(std::string &filePath)
{
	if (vec_contains(m_deps, filePath))
	{
		return false;
	}

	m_deps.push_back(filePath);
	return true;
}


bool ProjectPass::RemoveDependency(std::string &filePath)
{
	if (vec_contains(m_deps, filePath))
	{
		vec_remove(m_deps, filePath);
		return true;
	}

	return false;
}


void ProjectPass::AddMacro(std::string key, std::string value)
{
	if (!value.empty())
	{
		m_macros[key] = value;
	}
	else if (m_macros.count(key) == 0)
	{
		m_macros[key] = "";
	}

	ReplaceUndefinedMacros();
}


void ProjectPass::ReplaceUndefinedMacros()
{
	for (auto const&[key, value]: m_macros)
	{
		m_macros[key] = ReplaceMacros(m_macros, value);
	}
}



