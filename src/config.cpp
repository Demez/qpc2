#include "util.h"
#include "builder.h"
#include "config.h"
#include "project.h"


Config::Config(ProjectPass* proj, std::string name)
{
	this->proj = proj;
	m_name = name;
	
	general.Init(this);
	compile.Init(this);
	link.Init(this);
	debug.Init(this);
}


void Config::ParseOption(std::string &group, QPCBlock& option)
{
	if (group == "general")
		general.ParseOption(proj->m_macros, option);

	else if (group == "compile")
		compile.ParseOption(proj->m_macros, option);

	else if (group == "link")
		link.ParseOption(proj->m_macros, option);

	else if (group == "debug")
		debug.ParseOption(proj->m_macros, option);

	else if (group == "pre_build" || group == "pre_link" || group == "post_build")
	{

	}
	else
	{
		printf("Unknown group option: \"%s\"", group.c_str());
	}
}



void General::Init(Config* config)
{
	ConfigGroup::Init(config);

	compiler = cfg->proj->m_platform == Platform::WINDOWS ? "msvc" : "g++";

	// std::string default_dir = f"{self._config.get_name()}/f{platform.name.lower()}"
}


void General::ParseOption(StringMap& macros, QPCBlock& option)
{
}

void Compile::ParseOption(StringMap& macros, QPCBlock& option)
{
}

void Link::ParseOption(StringMap& macros, QPCBlock& option)
{
}

void Debug::ParseOption(StringMap& macros, QPCBlock& option)
{
}



