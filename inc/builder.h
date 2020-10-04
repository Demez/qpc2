#pragma once

#include "util.h"
#include "project.h"
#include "project_manager.h"
#include "conditions.h"
#include "lexer.h"


// this probably doesn't need to be a class
class ProjectBuilder
{
public:
	ProjectBuilder() {}

	void ParseDefFile(std::string &path);
	ProjectContainer* ParseProject(ProjectInfo* info);

private:
	void ParseDefFileRecurse(QPCBlockRoot *root, Platform plat);
	void ParseProjRecurse(QPCBlockRoot *root);

	// these aren't the best names, but whatever
	inline void Manager_AddMacro(QPCBlock *block, Platform plat);
	void Manager_AddConfigs(QPCBlock *block, Platform plat);
	void Manager_AddProject(QPCBlock *block, Platform plat);
	void Manager_AddGroup(QPCBlock *block, Platform plat);

	void Manager_ParseGroup(QPCBlock *block, Platform plat, ProjectGroup* group, fs::path &folder);

	void Proj_SetScriptMacros(fs::path filePath);
	void Proj_AddMacro(QPCBlock *block);
	void Proj_HandleConfig(QPCBlock *block);
	void Proj_HandleBuildEvent(QPCBlock *block);
	void Proj_HandleDependencies(QPCBlock *block);

	void Proj_HandleFiles(QPCBlock *block, fs::path &folder);
//	void Proj_HandleSourceFile(QPCBlock &block);
	void Proj_AddFiles(QPCBlock *block, fs::path &folder);
//	void Proj_AddFilesGlob(QPCBlock &block, std::vector<std::string> &folders);
//	void Proj_RemoveFiles(QPCBlock &block, std::vector<std::string> &folders);

	void Proj_ParseConfigGeneral(StringUMap& macros, QPCBlock *group);
	void Proj_ParseConfigCompile(StringUMap& macros, QPCBlock *group);
	void Proj_ParseConfigLink(StringUMap& macros, QPCBlock *group);
	void Proj_ParseConfigDebug(StringUMap& macros, QPCBlock *group);
	void Proj_ParseConfigBuildEvent(StringUMap& macros, QPCBlock *event);

	void SetListOption(StringUMap& macros, QPCBlock *option, std::vector<std::string> &vec);
	std::string ReplaceProjMacros(const std::string &string);
	
	ProjectPass* m_proj = NULL;
	int m_counter = 0;
};





