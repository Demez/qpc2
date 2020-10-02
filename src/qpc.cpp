#include "args.h"
#include "util.h"
#include "logging.h"
#include "builder.h"
#include "project.h"
#include "project_manager.h"
#include "generator_handler.h"
// #include "hash.h"


// constexpr int g_QPCVersion = 10;
// constexpr const char* g_QPCVersionS = "1.0";


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


bool ShouldBuildProject(ProjectInfo* info)
{
	if (GetArgs().force)
	{
		return true;
	}

	// TODO: only use the generators we want in the arguments, setup a list of that or something
	for (BaseGenerator* gen: GetGeneratorHandler().m_generators)
	{
		if (gen->DoesProjectNeedRebuild(info))
		{
			return true;
		}
	}

	// some hash stuff here

	return true;
}


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


// :trollface:
auto main(int argc, const char** argv) -> int
{
	printf("Quiver Project Creator C++\n");

	g_exePath = argv[0];

	GetGeneratorHandler().LoadGenerators();

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

	// m_buildList is a project queue in the ProjectManager class
	// any project added initially will be added to that queue, and you can add stuff to it while parsing projects
	// used for adding dependencies of a project
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
				for (BaseGenerator* gen: GetGeneratorHandler().m_generators)
				{
					gen->CreateProject(proj);
				}
			}
			else
			{
				for (BaseGenerator* gen: GetGeneratorHandler().m_generators)
				{
					// TODO: when you create the hashing system,
					// allow generators to put their own stuff in it, and check it here
					if (gen->DoesProjectNeedRebuild(info))
					{
						gen->CreateProject(proj);
					}
				}
			}

			delete proj;
		}
		else
		{
			// get the dependencies in the hash file
		}
	}

	// master file stuff now
	// maybe change to masterProject or masterProjectName?
	// though that might be confused with a default project in visual studio solutions
	if (!GetArgs().masterFile.empty())
	{
		for (BaseGenerator* gen: GetGeneratorHandler().m_generators)
		{
			if (!gen->GeneratesMasterFile())
			{
				continue;
			}

			if (gen->DoesMasterFileNeedRebuild(GetArgs().masterFile))
			{
				gen->CreateMasterFile(GetArgs().masterFile);
			}
		}
	}

	printf("\nFinished, Parsed %llu projects\n", manager.m_buildList.size());

	return 0;
}

