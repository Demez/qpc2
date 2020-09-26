#include "args.h"
#include "util.h"
#include "builder.h"
#include "project.h"
#include "project_manager.h"
#include "generator_handler.h"
// #include "hash.h"


constexpr int g_QPCVersion = 10;
constexpr const char* g_QPCVersionS = "1.0";


// :trollface:
auto main(int argc, const char** argv) -> int
{
	std::string test = PlatformToStr((Platform)8);

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

	ProjectManager& manager = GetProjManager();

	for (std::string cfg: GetArgs().configs)
		manager.AddConfig(cfg);

	// this would go through every single project found
	// might need to change this a little bit,
	// but make sure to have it so you can add more projects in this for loop (adding dependencies)
	std::vector<ProjectContainer*> bruh;
	for (int i = 0; i < manager.m_projects.size(); i++)
	{
		ProjectInfo& info = manager.m_projects[i];

		if (!FileExists(info.m_path))
			continue;

		ProjectContainer* proj = builder.ParseProject(info.m_path);
		bruh.push_back(proj);
	}

	printf("Parsed %d projects: ", bruh.size());

	return 0;
}

