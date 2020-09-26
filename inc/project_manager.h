#pragma once

#include "util.h"
#include <vector>
#include <unordered_map>
#include <map>


class ProjectManager;


// ok i probably don't need this tbh
class ProjectManagerItem: public PlatformItem
{
public:
	ProjectManagerItem(std::string name);
	ProjectManagerItem();

	inline bool Valid()
	{
		return m_valid;
	}

	bool m_valid = false;

	std::string m_name;
};


class ProjectInfo: public ProjectManagerItem
{
public:
	ProjectInfo(std::string name);
	ProjectInfo();

	static ProjectInfo& GetInvalid()
	{
		static const ProjectInfo invalid;
		return const_cast<ProjectInfo&>(invalid);
	}

	std::string m_path;

	// the hash system would be called in the constructor to create a path for this file
	std::string m_hashPath;
};


class ProjectGroup: public ProjectManagerItem
{
public:
	ProjectGroup(std::string name);
	ProjectGroup();

	static ProjectGroup& GetInvalid()
	{
		static const ProjectGroup invalid;
		return const_cast<ProjectGroup&>(invalid);
	}

	// project, folder project is in
	std::map<ProjectInfo, std::string> m_projects;
};


enum class ProjManError
{
	NONE = 0,
	ALREADY_ADDED,
};


// used to be BaseInfoPlatform
// ...but naming this ProjectManagerPlatform looks really dumb (along with the original)
// so im not sure how i would do this
// adding a "platform" std::vector on Projects and Groups would be fine tbh
// ...but how would i handle macros?
class ProjectManager
{
public:
	ProjectManager() {}

	static ProjectManager& GetProjManager()
	{
		static ProjectManager manager;
		return manager;
	}

	ProjectInfo                         CreateProject(std::string name, std::string path);
	ProjectGroup                        CreateGroup(std::string name);

	ProjManError                        AddProject(ProjectInfo projectDef);
	ProjManError                        AddGroup(ProjectGroup group);
	ProjManError                        AddConfig(std::string &config);

	ProjectInfo&                        GetProject(std::string &pathOrName);
	ProjectInfo&                        GetProject(std::string &name, std::string &path);

	ProjectGroup&                       GetGroup(std::string &name);

	std::vector<std::string>            m_configs;

	std::vector<ProjectInfo>            m_projects;
	std::vector<ProjectGroup>           m_groups;

	// i don't like this at ALL
	std::unordered_map< Platform, StringMap > m_macros;

	// remove these 3
	bool                                AddProject(std::string name, std::string path);
	bool                                AddProject(std::string &path);
	void                                AddGroup(std::string name);
};


static ProjectManager& GetProjManager()
{
	return ProjectManager::GetProjManager();
}

