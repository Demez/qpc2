#pragma once

#include "util.h"
#include <vector>
#include <unordered_map>
#include <map>


enum class ProjManError
{
	NONE = 0,
	ALREADY_ADDED,      // group or project that's already added, can ignore nearly all the time lol
	EMPTY_PATH,         // project with an empty path
};


class ProjectInfo: public PlatformItem
{
public:
	ProjectInfo(std::string name);
	ProjectInfo();

	ProjManError AddDependency(ProjectInfo* proj);

	bool operator==(const ProjectInfo &other) const
	{
		bool isEqual = false;
		isEqual = (m_name == other.m_name);

		if (!m_path.empty())
			isEqual = (m_path == other.m_path);

		// isEqual = (m_hashPath == other.m_hashPath);

		if (!m_deps.empty())
			isEqual = (m_platforms == other.m_platforms);

		if (!m_deps.empty())
			isEqual = (m_deps == other.m_deps);

		return isEqual;
	}

	// apparently this is needed
	bool operator<(const ProjectInfo &other) const
	{
		return (m_deps < other.m_deps);
	}

	std::string m_name;
	std::string m_path;
	std::vector<ProjectInfo*> m_deps;

	// the hash system would be called in the constructor to create a path for this file
	std::string m_hashPath;
};


class ProjectGroup: public PlatformItem
{
public:
	ProjectGroup(std::string name);
	ProjectGroup();

	bool operator==(const ProjectGroup &other) const
	{
		bool isEqual = false;
		isEqual = (m_name == other.m_name);

		if (!m_projects.empty())
			isEqual = (m_projects == other.m_projects);

		if (!m_platforms.empty())
			isEqual = (m_platforms == other.m_platforms);

		return isEqual;
	}

	std::string m_name;

	// project, folder project is in
	std::map<ProjectInfo*, std::string> m_projects;
};


class ProjectManager
{
public:
	ProjectManager() {}

	static ProjectManager& GetProjManager()
	{
		static ProjectManager manager;
		return manager;
	}

	ProjectInfo*                        CreateProject(std::string name, std::string path);
	ProjectGroup*                       CreateGroup(std::string name);

	ProjManError                        AddProject(ProjectInfo* info);
	ProjManError                        AddGroup(ProjectGroup* group);
	ProjManError                        AddConfig(std::string &config);
	ProjManError                        AddProjToGroup(ProjectGroup* group, ProjectInfo* project, std::string folder);

	ProjectInfo*                        GetProject(std::string &pathOrName);
	ProjectInfo*                        GetProject(std::string &name, std::string &path);

	ProjectGroup*                       GetGroup(std::string &name);

	// project queue stuff
	ProjManError                        AddToBuildList(ProjectInfo* info);
	bool                                ShouldAddProject(ProjectInfo* info);
	bool                                ShouldRemoveProject(ProjectInfo* info);
	bool                                ShouldAddGroup(ProjectGroup* info);
	bool                                ShouldRemoveGroup(ProjectGroup* info);

	std::vector<std::string>            m_configs;

	std::vector<ProjectInfo*>           m_projects;
	std::vector<ProjectGroup*>          m_groups;

	std::vector<ProjectInfo*>           m_buildList;

	// i don't like this at ALL
	std::unordered_map< Platform, StringMap > m_macros;
};


static ProjectManager& GetProjManager()
{
	return ProjectManager::GetProjManager();
}

