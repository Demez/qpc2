#include "util.h"
#include "builder.h"
#include "config.h"
#include "project.h"


const std::vector<std::string> g_configType = 
{
	"DYNAMIC_LIB",
	"STATIC_LIB",
	"APPLICATION",
};

const std::vector<std::string> g_standard = 
{
	"CPPLATEST",
	"CPP17",
	"CPP14",
	"CPP11",
	"CPP03",
	"CPP98",

	"C11",
	"C99",
	"C95",
	"C90",
	"C89",
};

const std::vector<std::string> g_standardNum = 
{
	"20",
	"17",
	"17",
	"11",
	"03",
	"98",

	"11",
	"99",
	"95",
	"90",
	"89",
};


ProjError VerifyEnum(int &value, int max)
{
	if (value >= max || value < 0)
	{
		value = 0;
		return ProjError::INVALID_OPTION;
	}

	return ProjError::NONE;
}


std::string StandardToNum(Standard standard)
{
	int iValue = (int)standard;
	VerifyEnum(iValue, (int)Standard::COUNT);
	return g_standardNum[iValue];
}


ProjError ConvertBoolOption(bool &prevValue, std::string value)
{
	str_upper(value);

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


ProjError General::SetConfigType(std::string value)
{
	str_upper(value);

	if (!vec_contains(g_configType, value))
	{
		return ProjError::INVALID_OPTION;
	}

	SetConfigType((ConfigType)(vec_index(g_configType, value) + 1));
	return ProjError::NONE;
}


ProjError General::SetConfigType(ConfigType value)
{
	int iValue = (int)value;
	ProjError ret = VerifyEnum(iValue, (int)ConfigType::COUNT);

	if (ret == ProjError::NONE)
	{
		configType = value;
	}

	return ret;
}


ProjError General::SetLanguageAndStandard(std::string value)
{
	str_upper(value);
	
	if (value.substr(0, 3) == "CPP")
	{
		lang = Language::CPP;
	}
	else if (value.substr(0, 1) == "C")
	{
		lang = Language::C;
	}
	else
	{
		return ProjError::INVALID_OPTION;
	}

	if (vec_contains(g_standard, value))
	{
		SetStandard((Standard)vec_index(g_standard, value));
	}

	return ProjError::NONE;
}


ProjError General::SetLanguage(std::string value)
{
	str_upper(value);

	if (value == "CPP")
	{
		lang = Language::CPP;
	}
	else if (value == "C")
	{
		lang = Language::C;
	}
	else
	{
		return ProjError::INVALID_OPTION;
	}

	return ProjError::NONE;
}


ProjError General::SetLanguage(Language value)
{
	int iValue = (int)value;
	ProjError ret = VerifyEnum(iValue, (int)Language::COUNT);

	if (ret == ProjError::NONE)
	{
		lang = value;
	}

	return ret;
}


ProjError General::SetStandard(Standard value)
{
	int iValue = (int)value;
	ProjError ret = VerifyEnum(iValue, (int)Standard::COUNT);

	if (ret == ProjError::NONE)
	{
		standard = value;
	}

	return ret;
}


// ================================================================


ProjError Compile::SetPCH(EPCH value)
{
	int iValue = (int)value;
	ProjError ret = VerifyEnum(iValue, (int)EPCH::COUNT);

	if (ret == ProjError::NONE)
	{
		pch = value;
	}

	return ret;
}


ProjError Compile::SetPCH(std::string value)
{
	str_upper(value);

	if (value == "NONE")
	{
		pch = EPCH::NONE;
	}
	else if (value == "CREATE")
	{
		pch = EPCH::CREATE;
	}
	else if (value == "USE")
	{
		pch = EPCH::USE;
	}
	else
	{
		return ProjError::INVALID_OPTION;
	}

	return ProjError::NONE;
}


ProjError Compile::SetDefaultIncDirs(std::string &value)
{
	return ConvertBoolOption(defaultIncDirs, value);
}


// ================================================================


ProjError Link::SetDefaultLibDirs(std::string &value)
{
	return ConvertBoolOption(defaultLibDirs, value);
}


ProjError Link::SetIgnoreImpLib(std::string &value)
{
	return ConvertBoolOption(ignoreImpLib, value);
}


// ================================================================


Config::Config(ProjectPass* proj, std::string name)
{
	this->proj = proj;
	m_name = name;
	
	general.Init(this);
	compile.Init(this);
	link.Init(this);
	debug.Init(this);
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



