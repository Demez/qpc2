#include "util.h"
#include "builder.h"
#include "config.h"
#include "project.h"


ProjError ConvertBoolOption(bool &prevValue, std::string value)
{
	std::transform(value.begin(), value.end(), value.begin(), ::toupper);

	if (value == "true" || value == "1")
	{
		prevValue = true;
	}
	else if (value == "false" || value == "0")
	{
		prevValue = false;
	}
	else
	{
		return ProjError::INVALID_OPTION;
	}

	return ProjError::NONE;
}


ProjError VerifyEnum(int &value, int max)
{
	if (value >= max || value < 0)
	{
		value = 0;
		return ProjError::INVALID_OPTION;
	}

	return ProjError::NONE;
}


ProjError General::SetConfigType(std::string value)
{
	return ProjError::NONE;
}


ProjError General::SetConfigType(ConfigType value)
{
	int iValue = (int)value;
	ProjError ret = VerifyEnum(iValue, (int)ConfigType::COUNT);

	if (ret != ProjError::NONE)
		configType = value;

	return ret;
}


Config::Config(ProjectPass* proj, std::string name)
{
	this->proj = proj;
	m_name = name;
	
	general.Init(this);
	compile.Init(this);
	link.Init(this);
	debug.Init(this);
}


// i probably should not use this
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

	// should this be DYNAMIC_LIB by default or something? idk
	configType = ConfigType::INVALID;
	lang = Language::CPP;
	standard = Standard::CPP17;

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



