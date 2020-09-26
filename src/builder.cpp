#include "util.h"
#include "args.h"
#include "logging.h"
#include "builder.h"
#include "lexer.h"
#include "conditions.h"


void ProjectBuilder::ParseDefFile(std::string &path)
{
	if (!FileExists(path))
		return;

	QPCBlockRoot* qpcRoot = ReadFile(path);

	ProjectManager& manager = GetProjManager();

	for (Platform platform: GetArgPlatforms())
	{
		SetPlatformMacros(manager.m_macros[platform], platform);

		for (auto const& [key, val]: GetArgs().macros)
			manager.m_macros[platform][key] = val;

		ParseDefFileRecurse(qpcRoot, platform);
	}

	delete qpcRoot;
}


void ProjectBuilder::ParseDefFileRecurse(QPCBlockRoot *root, Platform &plat)
{
	ProjectManager& manager = GetProjManager();

	for (QPCBlock* block: root->m_items)
	{
		if (!block->SolveCondition(manager.m_macros[plat]))
			continue;

		std::string key = ReplaceMacros(manager.m_macros[plat], block->m_key);

		if (key == "macro")
			Manager_AddMacro(block, plat);

		else if (key == "configs")
			Manager_AddConfigs(block, plat);

		else if (key == "project")
			Manager_AddProject(block, plat);

		else if (key == "group")
			Manager_AddGroup(block, plat);

		else if (key == "include")
		{
			if (block->m_values.empty())
			{
				warning("Include Key with no path!");
				continue;
			}

			QPCBlockRoot* qpcRoot = ReadFile(block->m_values[0]);
			ParseDefFileRecurse(qpcRoot, plat);
			delete qpcRoot;
		}
		else
		{
			warning("Unknown Base File Item: \"%s\"", key.c_str());
		}
	}
}

inline void ProjectBuilder::Manager_AddMacro(QPCBlock *block, Platform &plat)
{
	GetProjManager().m_macros[plat][block->m_key] = block->m_values[0];
}

void ProjectBuilder::Manager_AddConfigs(QPCBlock *block, Platform &plat)
{
	ProjectManager& manager = GetProjManager();
	for (QPCBlock* cfg: block->GetItemsCond(manager.m_macros[plat]))
	{
		manager.AddConfig(cfg->m_key);
	}
}


void ProjectBuilder::Manager_AddProject(QPCBlock *block, Platform &plat)
{
	ProjectInfo project = GetProjManager().CreateProject(block->GetValue(0), block->GetValue(1));
	if (!project.Valid())
		return;

	project.AddPlatform(plat);
	GetProjManager().AddProject(project);
}


void ProjectBuilder::Manager_AddGroup(QPCBlock *block, Platform &plat)
{
	ProjectGroup group = GetProjManager().CreateGroup(block->GetValue(0));
	if (!group.Valid())
		return;

	group.AddPlatform(plat);
	if (GetProjManager().AddGroup(group) != ProjManError::NONE)
		return;

	Manager_ParseGroup(block, plat, group, std::vector<std::string> {});
}


void ProjectBuilder::Manager_ParseGroup(QPCBlock *block, Platform &plat, ProjectGroup &group, std::vector<std::string> &folderList)
{
	ProjectManager& manager = GetProjManager();

	for (QPCBlock* item: block->GetItemsCond( manager.m_macros[plat] ))
	{
		ProjectInfo& project = manager.GetProject(item->m_key);

		if (project.Valid())
		{

		}
		else
		{

		}
	}
}


// ==========================================================================


ProjectContainer* ProjectBuilder::ParseProject(std::string &path)
{
	QPCBlockRoot* qpcRoot = ReadFile(path);

	ProjectManager& manager = GetProjManager();

	fs::path fsPath = path;
	ProjectContainer* projContainer = new ProjectContainer(fsPath);

	std::string prevDir = GetCurrentDir();
	std::string dir = fsPath.parent_path().string();
	ChangeDir(dir);

	for (ProjectPass* pass: projContainer->m_passes)
	{
		m_proj = pass;
		ParseProjRecurse(qpcRoot);
	}

	ChangeDir(prevDir);
	delete qpcRoot;

	return projContainer;
}


void ProjectBuilder::ParseProjRecurse(QPCBlockRoot *root)
{
	Proj_SetScriptMacros(root->m_filePath);

	for (QPCBlock* block: root->GetItemsCond(m_proj->m_macros))
	{
		if (block->m_key == "macro")
		{
			Proj_AddMacro(block);
		}
		else if (block->m_key == "config")
		{
			Proj_HandleConfig(block);
		}
		else if (block->m_key == "files")
		{
			Proj_HandleFiles(block, fs::path(""));
		}
		else if (block->m_key == "requires")
		{
			Proj_HandleDependencies(block);
		}
		else if (block->m_key == "build_event")
		{
			Proj_HandleBuildEvent(block);
		}
		else if (block->m_key == "include")
		{
			if (block->m_values.size() < 1)
			{
				block->warning("No path specified after include!");
				continue;
			}

			std::string path = ReplaceMacros(m_proj->m_macros, block->m_values[0]);
			if (!FileExists(path))
			{
				block->warning("File does not exist: \"%s\"!", path.c_str());
				continue;
			}

			QPCBlockRoot* include = ReadFile(path);
			ParseProjRecurse(include);
			Proj_SetScriptMacros(root->m_filePath);
			// hmm, maybe we should keep this? idk
			delete include;
		}
		else
		{
			block->warning("Unknown key - \"%s\"", block->m_key.c_str());
		}
	}
}


void ProjectBuilder::Proj_HandleFiles(QPCBlock *block, fs::path &folder)
{
	for (QPCBlock* item: block->m_items)
	{
		if (!item->SolveCondition(m_proj->m_macros))
			continue;

		if (item->m_key == "folder")
		{
			folder.append(item->m_values[0]);
			Proj_HandleFiles(item, folder);
			folder = folder.parent_path();
		}
		else if (item->m_key == "-")
		{
			// remove file
		}
		/*else if (item->m_key == "source")
		{
			// add source file
		}
		else if (item->m_key == "header")
		{
			// add header file
		}
		else if (item->m_key == "other")
		{
			// add aux file
		}*/
		else
		{
			// add file and automatically decide the type
			Proj_AddFiles(item, folder);
		}
	}
}


void ProjectBuilder::Proj_AddFiles(QPCBlock *block, fs::path &folder)
{
	for (std::string filePath: block->GetList())
	{
		filePath = ReplaceMacros(m_proj->m_macros, filePath);

		if (m_proj->IsFileAdded(filePath))
		{
			warning("File already added: \"%s\"", filePath.c_str());
			continue;
		}

		m_proj->AddFile(filePath, folder.string(), GetFileType(filePath));
	}
}


void ProjectBuilder::Proj_AddMacro(QPCBlock *block)
{
	std::string key = ReplaceMacros(m_proj->m_macros, block->GetValue(0));
	std::string value = ReplaceMacros(m_proj->m_macros, block->GetValue(1));
	m_proj->AddMacro(key, value);
}


void ProjectBuilder::Proj_HandleDependencies(QPCBlock *block)
{
	for (QPCBlock* item: block->GetItemsCond(m_proj->m_macros))
	{
		if (item->m_key == "-")
		{
			for (std::string path: item->m_values)
			{
				m_proj->RemoveDependency(path);
			}
		}
		else
		{
			for (std::string path: item->GetList())
			{
				m_proj->AddDependency(path);
			}
		}
	}
}


void ProjectBuilder::Proj_HandleConfig(QPCBlock *block)
{
}


void ProjectBuilder::Proj_HandleBuildEvent(QPCBlock *block)
{
}


void ProjectBuilder::Proj_SetScriptMacros(fs::path filePath)
{
	m_proj->AddMacro("SCRIPT_NAME", filePath.filename().string());
	m_proj->AddMacro("SCRIPT_DIR", filePath.parent_path().string());
}


