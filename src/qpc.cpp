#include "args.h"
#include "util.h"
#include "logging.h"
#include "builder.h"
#include "project.h"
#include "project_manager.h"
#include "generator_handler.h"
// #include "hash.h"


constexpr int g_QPCVersion = 10;
constexpr const char* g_QPCVersionS = "1.0";


inline bool ShouldAddDependencies(ProjectInfo* info)
{
	return (vec_contains(GetArgs().addDepend, info->m_name) || vec_contains(GetArgs().addDepend, info->m_path));
}


ProjectContainer* ParseProject(ProjectBuilder& builder, ProjectInfo* info)
{
	ProjectContainer* proj = builder.ParseProject(info->m_path);
	ProjectManager& manager = GetProjManager();

	for (ProjectPass* pass: proj->m_passes)
	{
		for (std::string dep: pass->m_deps)
		{
			ProjectInfo* depInfo = GetProjManager().GetProject(dep);
			if (!depInfo)
				continue;

			// are we adding dependencies for this project?
			// and are we not removing this project?
			if (ShouldAddDependencies(info) && !manager.ShouldRemoveProject(depInfo))
			{
				manager.AddToBuildList(depInfo);
			}

			info->AddDependency(depInfo);
		}
	}

	return proj;
}


bool ShouldBuildProject(ProjectInfo* info)
{
	if (GetArgs().force)
	{
		return true;
	}

	// some hash stuff here

	return true;
}


void SetupArgProjects()
{
	ProjectManager& manager = GetProjManager();

	for (std::string arg: GetArgs().add)
	{
		std::string name = arg;
		std::string path = arg;

		// are we adding a project by it's path in a group?
		if (FileExists(path))
		{
			fs::path fspath = fs::path(path).filename();
			name = fspath.string(); 

			if (fspath.has_extension())
			{
				// remove the file extension
				name = name.substr(0, name.length() - fspath.extension().string().length()); 
			}
		}
		else if (!manager.GetProject(path) && !manager.GetGroup(path))
		{
			warning("File, Project, or Group does not exist: \"%s\"", path.c_str());
			continue;
		}

		ProjectInfo* project = manager.CreateProject(name, path);
		if (!project)
			continue;

		for (Platform plat: GetArgPlatforms())
		{
			project->AddPlatform(plat);
		}

		manager.AddProject(project);
	}
}


// :trollface:
auto main(int argc, const char** argv) -> int
{
	printf("Quiver Project Creator C++\n");

	ArgParser* argParser = new ArgParser;
	argParser->ParseArgs(argc, argv);
	delete argParser;

	ChangeDir(GetArgs().rootDir);

	ProjectBuilder builder;

	std::string baseFile = GetArgs().baseFile;
	if (baseFile != "")
	{
		if (FileExists(baseFile))
			builder.ParseDefFile(baseFile);
		else
			printf("Base File does not exist: \"%s\"", baseFile.c_str());
	}

	SetupArgProjects();

	ProjectManager& manager = GetProjManager();

	for (std::string cfg: GetArgs().configs)
		manager.AddConfig(cfg);

	// this would go through every single project found
	// might need to change this a little bit,
	// but make sure to have it so you can add more projects in this for loop (adding dependencies)
	std::vector<ProjectContainer*> bruh;

	// maybe make a project queue in the ProjectManager class?
	// and any project added initially will be added to that queue?
	for (int i = 0; i < manager.m_buildList.size(); i++)
	{
		ProjectInfo* info = manager.m_buildList[i];

		// i probably don't need to check this, since the builder/manager handles that
		// if (!FileExists(info->m_path))
		// 	continue;

		if (ShouldBuildProject(info))
		{
			printf("\nParsing Project: \"%s\"\n", info->m_name.c_str());

			ProjectContainer* proj = ParseProject(builder, info);

			if (GetArgs().force)
			{
				// all generators rebuild
			}
			else
			{
				// for loop through each generator,
				// check if it's project exists (some just always return false lol)
				// if true, then check the hash of each to see if we should rebuild
			}

			bruh.push_back(proj);

			// delete proj;
		}
		else
		{
			// get the dependencies in the hash file
		}
	}

	printf("Parsed %d projects: ", bruh.size());

	return 0;
}

