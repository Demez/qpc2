#include "util.h"
#include "builder.h"
#include "project_manager.h"


ProjectManagerItem::ProjectManagerItem(std::string name)
{
	m_name = name;
}


ProjectInfo::ProjectInfo(std::string name):
	ProjectManagerItem(name)
{
}


ProjectGroup::ProjectGroup(std::string name):
	ProjectManagerItem(name)
{
}


ProjectManagerItem::ProjectManagerItem() {}
ProjectInfo::ProjectInfo(): ProjectManagerItem() {}
ProjectGroup::ProjectGroup(): ProjectManagerItem() {}


ProjectInfo ProjectManager::CreateProject(std::string name, std::string path)
{
	ProjectInfo project(name);

	// is this project being removed on the command line?

	// does this file actually exist?
	if (!FileExists(path))
		return project;

	// check if this is added already
	for (ProjectInfo info: m_projects)
	{
		if (info.m_path == path)
			return info;

		if (info.m_name == path)
			return info;
	}

	project.m_valid = true;
	project.m_path = path;

	return project;
}


ProjectGroup ProjectManager::CreateGroup(std::string name)
{
	ProjectGroup group(name);

	// is this project being removed on the command line?

	// check if this is added already
	for (ProjectGroup addedGroup: m_groups)
	{
		// already have a group with this name
		if (addedGroup.m_name == name)
			return addedGroup;
	}

	group.m_valid = true;
	return group;
}




bool ProjectManager::AddProject(std::string &path)
{
	// TODO: get the base name of the path here for the first argument
	return AddProject(path, path);
}


bool ProjectManager::AddProject(std::string name, std::string path)
{
	// is this project being removed on the command line?

	// does this file actually exist?
	if (!FileExists(path))
		return false;

	// check if this is added already
	for (ProjectInfo info: m_projects)
	{
		if (info.m_path == path)
			return false;

		if (info.m_name == path)
			return false;
	}

	ProjectInfo project(name);
	project.m_path = path;

	m_projects.push_back(project);

	return true;
}


void ProjectManager::AddGroup(std::string name)
{
	// is this group being removed on the command line?

	// check if this is added already
	for (ProjectGroup group: m_groups)
	{
		// already have a group with this name
		if (group.m_name == name)
			return;
	}

	ProjectGroup group(name);
	m_groups.push_back(group);
}


ProjManError ProjectManager::AddGroup(ProjectGroup group)
{
	// is this group being removed on the command line?

	// check if this is added already
	for (ProjectGroup addedGroup: m_groups)
	{
		// already have a group with this name
		if (addedGroup.m_name == group.m_name)
		{
			// merge these groups? idk
			return ProjManError::ALREADY_ADDED;
		}
	}
	
	m_groups.push_back(group);
	return ProjManError::NONE;
}


ProjManError ProjectManager::AddProject(ProjectInfo project)
{
	// is this group being removed on the command line?

	// check if this is added already
	for (ProjectInfo addedProject: m_projects)
	{
		// already have a group with this name
		if (addedProject.m_name == project.m_name)
		{
			return ProjManError::ALREADY_ADDED;
		}
	}

	m_projects.push_back(project);
	return ProjManError::NONE;
}


ProjectInfo& ProjectManager::GetProject(std::string &pathOrName)
{
	return GetProject(fs::path(pathOrName).filename().string(), pathOrName);
}


ProjectInfo& ProjectManager::GetProject(std::string &name, std::string &path)
{
	// check if this is added already
	for (ProjectInfo addedProject: m_projects)
	{
		if (addedProject.m_name == name || addedProject.m_path == path)
		{
			// do i need to make this a pointer or something? since im returning the address of a temp var
			return addedProject;
		}
	}

	return ProjectInfo::GetInvalid();
}


ProjectGroup& ProjectManager::GetGroup(std::string &name)
{
	// check if this is added already
	for (ProjectGroup addedgroup: m_groups)
	{
		if (addedgroup.m_name == name)
		{
			// same comment as the function above
			return addedgroup;
		}
	}

	return ProjectGroup::GetInvalid();
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



