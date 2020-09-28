#include "util.h"
#include "args.h"
#include "builder.h"
#include "project_manager.h"


ProjectInfo::ProjectInfo(std::string name)
{
	m_name = name;
}


ProjManError ProjectInfo::AddDependency(ProjectInfo* proj)
{
	if (vec_contains(m_deps, proj))
	{
		return ProjManError::ALREADY_ADDED;
	}

	m_deps.push_back(proj);
	return ProjManError::NONE;
}


ProjectGroup::ProjectGroup(std::string name)
{
	m_name = name;
}


ProjectInfo::ProjectInfo() {}
ProjectGroup::ProjectGroup() {}


ProjectInfo* ProjectManager::CreateProject(std::string name, std::string path)
{
	// does this file actually exist?
	if (path != "" && !FileExists(path))
		return NULL;

	// check if this is added already
	for (ProjectInfo* info: m_projects)
	{
		if (!path.empty() && info->m_path == path)
			return info;

		if (info->m_name == name)
			return info;
	}

	ProjectInfo* project = new ProjectInfo(name);
	project->m_path = path;

	return project;
}


ProjectGroup* ProjectManager::CreateGroup(std::string name)
{
	// check if this is added already
	for (ProjectGroup* addedGroup: m_groups)
	{
		// already have a group with this name
		if (addedGroup->m_name == name)
			return addedGroup;
	}

	ProjectGroup* group = new ProjectGroup(name);
	return group;
}


ProjManError ProjectManager::AddGroup(ProjectGroup* group)
{
	// is this group being removed on the command line?
	if (ShouldRemoveGroup(group))
		return ProjManError::NONE;

	// check if this is added already
	for (ProjectGroup* addedGroup: m_groups)
	{
		// already have a group with this name
		if (addedGroup == group)
		{
			// merge these groups? idk
			return ProjManError::ALREADY_ADDED;
		}
	}
	
	m_groups.push_back(group);
	return ProjManError::NONE;
}


ProjManError ProjectManager::AddProject(ProjectInfo* project)
{
	// is this project being removed on the command line?
	if (ShouldRemoveProject(project))
		return ProjManError::NONE;

	// check if this is added already
	for (ProjectInfo* addedProject: m_projects)
	{
		// already have a group with this name
		if (addedProject == project)
		{
			return ProjManError::ALREADY_ADDED;
		}
	}

	m_projects.push_back(project);

	if (ShouldAddProject(project))
		AddToBuildList(project);

	return ProjManError::NONE;
}


ProjManError ProjectManager::AddConfig(std::string &config)
{
	// is this group being removed on the command line?

	// check if this is added already
	for (std::string addedCfg: m_configs)
	{
		// already have a group with this name
		if (addedCfg == config)
		{
			return ProjManError::ALREADY_ADDED;
		}
	}

	m_configs.push_back(config);
	return ProjManError::NONE;
}


ProjManError ProjectManager::AddProjToGroup(ProjectGroup* group, ProjectInfo* project, std::string folder)
{
	// check if this is added already
	for (auto const& addedProj: group->m_projects)
	{
		// already have a group with this name
		if (addedProj.first == project)
		{
			return ProjManError::ALREADY_ADDED;
		}
	}

	group->m_projects[project] = folder;

	if (ShouldAddGroup(group))
	{
		AddToBuildList(project);
	}

	return ProjManError::NONE;
}


ProjectInfo* ProjectManager::GetProject(std::string &pathOrName)
{
	return GetProject(fs::path(pathOrName).filename().string(), pathOrName);
}


ProjectInfo* ProjectManager::GetProject(const std::string &name, std::string &path)
{
	// check if this is added already
	for (ProjectInfo* addedProject: m_projects)
	{
		if (addedProject->m_name == name || addedProject->m_path == path)
		{
			return addedProject;
		}
	}

	return NULL;
}


ProjectGroup* ProjectManager::GetGroup(std::string &name)
{
	// check if this is added already
	for (ProjectGroup* addedgroup: m_groups)
	{
		if (addedgroup->m_name == name)
		{
			// same comment as the function above
			return addedgroup;
		}
	}

	return NULL;
}


ProjManError ProjectManager::AddToBuildList(ProjectInfo* info)
{
	if (info->m_path == "")
	{
		return ProjManError::EMPTY_PATH;
	}

	if (vec_contains(m_buildList, info))
	{
		return ProjManError::ALREADY_ADDED;
	}

	m_buildList.push_back(info);
	return ProjManError::NONE;
}


bool ProjectManager::ShouldAddProject(ProjectInfo* item)
{
	for (std::string add: GetArgs().add)
	{
		if (add == item->m_name || add == item->m_path)
			return true;
	}

	for (std::string add: GetArgs().addDepend)
	{
		if (add == item->m_name || add == item->m_path)
			return true;
	}

	for (std::string add: GetArgs().addTree)
	{
		if (add == item->m_name || add == item->m_path)
			return true;
	}

	return false;
}


bool ProjectManager::ShouldAddGroup(ProjectGroup* item)
{
	for (std::string add: GetArgs().add)
	{
		if (add == item->m_name)
			return true;
	}

	for (std::string add: GetArgs().addDepend)
	{
		if (add == item->m_name)
			return true;
	}

	for (std::string add: GetArgs().addTree)
	{
		if (add == item->m_name)
			return true;
	}

	return false;
}


bool ProjectManager::ShouldRemoveProject(ProjectInfo* item)
{
	for (std::string add: GetArgs().remove)
	{
		if (add == item->m_name || add == item->m_path)
			return true;
	}

	return false;
}


bool ProjectManager::ShouldRemoveGroup(ProjectGroup* item)
{
	for (std::string rm: GetArgs().remove)
	{
		if (rm == item->m_name)
			return true;
	}

	return false;
}




