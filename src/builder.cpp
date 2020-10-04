#include "util.h"
#include "args.h"
#include "logging.h"
#include "builder.h"
#include "lexer.h"
#include "conditions.h"


void ProjectBuilder::SetListOption(StringUMap& macros, QPCBlock *option, std::vector<std::string> &vec)
{
	std::vector<std::string> values;
	bool removing = false;

	for (QPCBlock* optionList: option->GetItemsCond(m_proj->m_macros))
	{
		removing = (optionList->m_key == "-");
		values = optionList->m_values;

		if (!removing)
		{
			values.insert(values.begin(), optionList->m_key);
		}
		else if (values.empty())
		{
			optionList->warning("No items set to remove in \"%s\"", option->m_key.c_str());
			continue;
		}

		for (std::string item: values)
		{
			item = ReplaceProjMacros(item);

			if (vec_contains(vec, item))
			{
				if (!removing)
				{
					optionList->warning("Item already added to list \"%s\": \"%s\"", option->m_key.c_str(), item.c_str());
				}
				else
				{
					vec_remove(vec, item);
				}
			}
			else
			{
				if (!removing)
				{
					vec.push_back(item);
				}
				else
				{
					optionList->warning("Trying to remove item not in list \"%s\": \"%s\"", option->m_key.c_str(), item.c_str());
				}
			}

		}
	}
}


void ProjectBuilder::ParseDefFile(std::string &path)
{
	if (!FileExists(path))
		return;

	QPCBlockRoot* qpcRoot = ReadFile(path);

	ProjectManager* manager = GetProjManager();

	for (Platform platform: GetArgPlatforms())
	{
		SetPlatformMacros(manager->m_macros[platform], platform);

		for (auto const& [key, val]: GetArgs().macros)
			manager->m_macros[platform][key] = val;

		ParseDefFileRecurse(qpcRoot, platform);
	}

	manager->SetupGroupIncludes();

	delete qpcRoot;
}


void ProjectBuilder::ParseDefFileRecurse(QPCBlockRoot *root, Platform plat)
{
	ProjectManager* manager = GetProjManager();

	for (QPCBlock* block: root->GetItemsCond(manager->m_macros[plat]))
	{
		std::string key = ReplaceMacros(manager->m_macros[plat], block->m_key);

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

inline void ProjectBuilder::Manager_AddMacro(QPCBlock *block, Platform plat)
{
	GetProjManager()->m_macros[plat][block->m_key] = block->m_values[0];
}

void ProjectBuilder::Manager_AddConfigs(QPCBlock *block, Platform plat)
{
	for (QPCBlock* cfg: block->GetItemsCond(GetProjManager()->m_macros[plat]))
	{
		GetProjManager()->AddConfig(cfg->m_key);
	}
}


void ProjectBuilder::Manager_AddProject(QPCBlock *block, Platform plat)
{
	ProjectInfo* project = GetProjManager()->CreateProject(block->GetValue(0), block->GetValue(1));
	if (!project)
		return;

	project->AddPlatform(plat);
	GetProjManager()->AddProject(project);
}


void ProjectBuilder::Manager_AddGroup(QPCBlock *block, Platform plat)
{
	ProjectGroup* group = GetProjManager()->CreateGroup(block->GetValue(0));
	if (!group)
		return;

	group->AddPlatform(plat);
	GetProjManager()->AddGroup(group);

	// check if we want to add this to other groups?
	// item->m_values.size() > 1

	fs::path fspath("");
	Manager_ParseGroup(block, plat, group, fspath);
}


void ProjectBuilder::Manager_ParseGroup(QPCBlock *block, Platform plat, ProjectGroup* group, fs::path &folder)
{
	ProjectManager* manager = GetProjManager();

	for (QPCBlock* item: block->GetItemsCond(manager->m_macros[plat]))
	{
		if (item->m_key == "folder")
		{
			if (item->m_values.empty())
			{
				item->warning("Group folder with no name!");
				// just add the projects to the current folder
				Manager_ParseGroup(item, plat, group, folder);
			}
			else
			{
				folder.append(item->GetValue(0));
				Manager_ParseGroup(item, plat, group, folder);
				folder = folder.parent_path();
			}

			continue;
		}
		else if (item->m_key == "contains")
		{
			if (item->m_values.empty())
			{
				item->warning("No groups selected to add into this group!");
			}

			for (std::string otherName: item->m_values)
			{
				otherName = ReplaceMacros(manager->m_macros[plat], otherName);

				ProjectGroup* other = manager->GetGroup(otherName);

				if (!other)
				{
					other = manager->CreateGroup(otherName);
					manager->AddGroup(other);
				}

				group->m_otherGroups[other] = folder.string();
			}
		}

		ProjectInfo* project = manager->GetProject(item->m_key);

		if (!project)
		{
			std::string name = item->m_key;
			std::string path = "";

			// are we adding a project by it's path in a group?
			if (FileExists(item->m_key))
			{
				fs::path fspath = fs::path(item->m_key).filename();
				name = fspath.string(); 

				if (fspath.has_extension())
				{
					// remove the file extension
					name = name.substr(0, name.length() - fspath.extension().string().length()); 
				}

				path = item->m_key;
			}

			project = manager->CreateProject(name, path);
			if (!project)
				continue;

			project->AddPlatform(plat);
			manager->AddProject(project);
		}

		manager->AddProjToGroup(group, project, folder.string());
	}
}


// ==========================================================================


ProjectContainer* ProjectBuilder::ParseProject(ProjectInfo* info)
{
	QPCBlockRoot* qpcRoot = ReadFile(info->m_path.string());

	ProjectContainer* projContainer = new ProjectContainer(info);

	std::string dir = info->m_path.parent_path().string();
	ChangeDir(dir);

	for (ProjectPass* pass: projContainer->m_passes)
	{
		m_proj = pass;
		ParseProjRecurse(qpcRoot);
	}

	delete qpcRoot;

	return projContainer;
}


void ProjectBuilder::ParseProjRecurse(QPCBlockRoot *root)
{
	Proj_SetScriptMacros(root->m_filePath);

	for (QPCBlock* block: root->m_items)
	{
		// cannot use GetItemsCond, as new macros can be defined here
		if (!block->SolveCondition(m_proj->m_macros))
		{
			continue;
		}

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
			fs::path fspath("");
			Proj_HandleFiles(block, fspath);
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

			std::string path = ReplaceProjMacros(block->m_values[0]);
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
	for (QPCBlock* item: block->GetItemsCond(m_proj->m_macros))
	{
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
		filePath = ReplaceProjMacros(filePath);

		if ( !m_proj->AddFile(filePath, folder.string(), GetFileType(filePath)) )
		{
			warning("File already added: \"%s\"", filePath.c_str());
			continue;
		};
	}
}


void ProjectBuilder::Proj_AddMacro(QPCBlock *block)
{
	std::string key = ReplaceProjMacros(block->GetValue(0));
	std::string value = ReplaceProjMacros(block->GetValue(1));
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


void ProjectBuilder::Proj_HandleConfig(QPCBlock *cfg)
{
	for (QPCBlock* groupBlock: cfg->GetItemsCond(m_proj->m_macros))
	{
		std::string group = ReplaceProjMacros(groupBlock->m_key);

		if (group == "general")
		{
			Proj_ParseConfigGeneral(m_proj->m_macros, groupBlock);
		}
		else if (group == "compile")
		{
			Proj_ParseConfigCompile(m_proj->m_macros, groupBlock);
		}
		else if (group == "link")
		{
			Proj_ParseConfigLink(m_proj->m_macros, groupBlock);
		}
		else if (group == "debug")
		{
			Proj_ParseConfigDebug(m_proj->m_macros, groupBlock);
		}
		else if (group == "pre_build" || group == "pre_link" || group == "post_build")
		{
			Proj_ParseConfigBuildEvent(m_proj->m_macros, groupBlock);
		}
		else
		{
			printf("Unknown group option: \"%s\"", group.c_str());
		}
	}
}


void ProjectBuilder::Proj_HandleBuildEvent(QPCBlock *block)
{
}


void ProjectBuilder::Proj_SetScriptMacros(fs::path filePath)
{
	std::string nameNoExt = filePath.filename().string();
	ReplaceString(nameNoExt, filePath.extension().string(), "");

	m_proj->AddMacro("SCRIPT_NAME", nameNoExt);
	m_proj->AddMacro("SCRIPT_DIR", filePath.parent_path().string());
}


void ProjectBuilder::Proj_ParseConfigGeneral(StringUMap& macros, QPCBlock *group)
{
	for (QPCBlock* option: group->GetItemsCond(m_proj->m_macros))
	{
		std::string key = ReplaceProjMacros(option->m_key);

		if (key == "options")
		{
			option->warning("options list in general is removed, as it's useless");
			continue;
		}
		else if (option->m_values.empty())
		{
			option->warning("No value specified after option \"%s\"", key.c_str());
			continue;
		}

		std::string value = ReplaceProjMacros(option->GetValue());

		if (key == "out_dir")
		{
			m_proj->cfg.general.outDir = value;
		}
		else if (key == "build_dir")
		{
			m_proj->cfg.general.buildDir = value;
		}
		else if (key == "out_name")
		{
			m_proj->cfg.general.outName = value;
		}
		else if (key == "out_name_prefix")
		{
			m_proj->cfg.general.outNamePrefix = value;
		}
		else if (key == "out_name_postfix")
		{
			m_proj->cfg.general.outNamePostfix = value;
		}
		else if (key == "compiler")
		{
			m_proj->cfg.general.compiler = value;
		}
		else if (key == "config_type")
		{
			if (m_proj->cfg.general.SetConfigType(value) == ProjError::INVALID_OPTION)
			{
				option->warning("Invalid Option for config_type: \"%s\"", value.c_str());
				continue;
			}
		}
		else if (key == "language" || key == "lang")
		{
			if (m_proj->cfg.general.SetLanguageAndStandard(value) == ProjError::INVALID_OPTION)
			{
				option->warning("Invalid Option for language: \"%s\"", value.c_str());
				continue;
			}
		}
		else
		{
			option->warning("Unknown option in general group: \"%s\"", key.c_str());
		}
	}
}


void ProjectBuilder::Proj_ParseConfigCompile(StringUMap& macros, QPCBlock *group)
{
	std::string key;

	for (QPCBlock* option: group->GetItemsCond(m_proj->m_macros))
	{
		key = ReplaceProjMacros(option->m_key);

		if (option->m_items.empty())
		{
			if (option->m_values.empty())
			{
				option->warning("No value specified after option \"%s\"", key.c_str());
				continue;
			}

			std::string value = ReplaceProjMacros(option->GetValue());

			if (key == "default_inc_dirs")
			{
				if (m_proj->cfg.compile.SetDefaultIncDirs(value) == ProjError::INVALID_OPTION)
				{
					option->warning("Invalid Option for default_inc_dirs: \"%s\"", value.c_str());
					continue;
				}
			}
			else if (key == "pch")
			{
				if (m_proj->cfg.compile.SetPCH(value) == ProjError::INVALID_OPTION)
				{
					option->warning("Invalid Option for pch: \"%s\"", value.c_str());
					continue;
				}
			}
			else if (key == "pch_file")
			{
				m_proj->cfg.compile.pchFile = value;
			}
			else if (key == "pch_out")
			{
				m_proj->cfg.compile.pchOut = value;
			}
			else
			{
				option->warning("Unknown option in compile group: \"%s\"", key.c_str());
			}
		}
		else
		{
			if (key == "defines")
			{
				SetListOption(macros, option, m_proj->cfg.compile.defines);
			}
			else if (key == "inc_dirs")
			{
				SetListOption(macros, option, m_proj->cfg.compile.incDirs);
			}
			else if (key == "options")
			{
				SetListOption(macros, option, m_proj->cfg.compile.options);
			}
			else
			{
				option->warning("Unknown option in compile group: \"%s\"", key.c_str());
			}
		}
	}
}


void ProjectBuilder::Proj_ParseConfigLink(StringUMap& macros, QPCBlock *group)
{
	std::string key;

	for (QPCBlock* option: group->GetItemsCond(m_proj->m_macros))
	{
		key = ReplaceProjMacros(option->m_key);

		if (option->m_items.empty())
		{
			if (option->m_values.empty())
			{
				option->warning("No value specified after option \"%s\"", key.c_str());
				continue;
			}

			std::string value = ReplaceProjMacros(option->GetValue());

			if (key == "default_lib_dirs")
			{
				if (m_proj->cfg.link.SetDefaultLibDirs(value) == ProjError::INVALID_OPTION)
				{
					option->warning("Invalid Option for default_lib_dirs: \"%s\"", value.c_str());
					continue;
				}
			}
			else if (key == "ignore_import_lib")
			{
				if (m_proj->cfg.link.SetIgnoreImpLib(value) == ProjError::INVALID_OPTION)
				{
					option->warning("Invalid Option for ignore_import_lib: \"%s\"", value.c_str());
					continue;
				}
			}
			else if (key == "output_file")
			{
				m_proj->cfg.link.outFile = value;
			}
			else if (key == "debug_file")
			{
				m_proj->cfg.link.dbgFile = value;
			}
			else if (key == "import_lib")
			{
				m_proj->cfg.link.impLib = value;
			}
			else if (key == "entry_point")
			{
				m_proj->cfg.link.entryPoint = value;
			}
			else
			{
				option->warning("Unknown option in link group: \"%s\"", key.c_str());
			}
		}
		else
		{
			if (key == "libs")
			{
				SetListOption(macros, option, m_proj->cfg.link.libs);
			}
			else if (key == "ignore_libs")
			{
				SetListOption(macros, option, m_proj->cfg.link.ignoreLibs);
			}
			else if (key == "lib_dirs")
			{
				SetListOption(macros, option, m_proj->cfg.link.libDirs);
			}
			else if (key == "options")
			{
				SetListOption(macros, option, m_proj->cfg.link.options);
			}
			else
			{
				option->warning("Unknown option in link group: \"%s\"", key.c_str());
			}
		}
	}
}


void ProjectBuilder::Proj_ParseConfigDebug(StringUMap& macros, QPCBlock *group)
{
	std::string key;

	for (QPCBlock* option: group->GetItemsCond(m_proj->m_macros))
	{
		key = ReplaceProjMacros(option->m_key);

		if (option->m_values.empty())
		{
			option->warning("No value specified after option \"%s\"", key.c_str());
			continue;
		}

		std::string value = ReplaceProjMacros(option->GetValue());

		if (key == "arguments" || key == "args")
		{
			m_proj->cfg.debug.args = value;
		}
		else if (key == "command" || key == "cmd")
		{
			m_proj->cfg.debug.cmd = value;
		}
		else if (key == "working_dir" || key == "cwd")
		{
			m_proj->cfg.debug.cwd = value;
		}
		else
		{
			option->warning("Unknown option in debug group: \"%s\"", key.c_str());
		}
	}
}


void ProjectBuilder::Proj_ParseConfigBuildEvent(StringUMap& macros, QPCBlock *event)
{

}


std::string ProjectBuilder::ReplaceProjMacros(const std::string &string)
{
	return ::ReplaceMacros(m_proj->m_macros, string);
}


