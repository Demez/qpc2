#include "util.h"
#include "args.h"
#include <map>
#include <io.h>

#ifdef _WIN32
#include <direct.h>
#else

#endif


const char* g_hashDir = "hashes";


size_t str_count(std::string string, std::string item)
{
	size_t count = 0;

	while (string.find(item) != std::string::npos)
	{
		string = string.substr(string.find(item) + 1);
		count++;
	}

	return count;
}


void PlatformItem::AddPlatform(Platform platform)
{
	if (!SupportsPlatform(platform))
		m_platforms.push_back(platform);
}

void PlatformItem::RemovePlatform(Platform platform)
{
	if (SupportsPlatform(platform))
		vec_remove(m_platforms, platform);
}

inline bool PlatformItem::SupportsPlatform(Platform platform)
{
	return vec_contains(m_platforms, platform);
}


// the same as above
void PlatArchItem::AddArch(Arch arch)
{
	if (!SupportsArch(arch))
		m_archs.push_back(arch);
}

void PlatArchItem::RemoveArch(Arch arch)
{
	if (SupportsArch(arch))
		vec_remove(m_archs, arch);
}

inline bool PlatArchItem::SupportsArch(Arch arch)
{
	return vec_contains(m_archs, arch);
}


void ReplaceString(std::string& str, const std::string& from, const std::string& to)
{
	if (str == "")
		return;

	while (str.find(from) != std::string::npos)
	{
		size_t start_pos = str.find(from);
		if(start_pos == std::string::npos)
			return;

		str.replace(start_pos, from.length(), to);
	}
}


void GetLongestString(std::vector<std::string> &strVec, std::string &longest)
{
	longest = strVec[0];
	for (std::string str: strVec)
	{
		if (str.length() > longest.length())
		longest = str;
	}
}

void GetLongestString(StringMap &strMap, std::string &longest)
{
	longest = "";
	for (auto const&[key, value]: strMap)
	{
		if (key.length() > longest.length())
			longest = key;
	}
}

std::string GetLongestString(std::vector<std::string> &strVec)
{
	std::string longest;
	GetLongestString(strVec, longest);
	return longest;
}

std::string GetLongestString(StringMap &strMap)
{
	std::string longest;
	GetLongestString(strMap, longest);
	return longest;
}


int ChangeDir(std::string &path)
{
	return chdir(path.c_str());
}

int CreateDir(std::string &path)
{
	if (GetArgs().verbose)
		printf("Creating Dir: \"%s\"", path.c_str());

	return mkdir(path.c_str());
}

std::string GetCurrentDir()
{
#ifdef _WIN32
	std::string cwd = getcwd( NULL, 0 );
	ReplaceString(cwd, "\\", "/");
	return cwd;
#else
	return getcwd( NULL, 0 );
#endif
}


bool FileExists(std::string &path)
{
	return fs::is_regular_file(path);
}

bool DirExists(std::string &path)
{
	return fs::is_directory(path);
}

bool ItemExists(std::string &path)
{
	return (access(path.c_str(), 0) != -1);
}



std::vector<std::string> g_extsSource = {".cpp", ".c", ".cc", ".cxx"};
std::vector<std::string> g_extsHeader = {".hpp", ".h", ".hh", ".hxx"};


bool CheckExtSource(std::string& ext)
{
	return vec_contains(g_extsSource, ext);
}

bool CheckExtHeader(std::string& ext)
{
	return vec_contains(g_extsHeader, ext);
}


std::vector<std::string> ArchStr = {
	"INVALID",
	"AMD64",
	"I386",
	"ARM",
	"ARM64",
};

std::vector<std::string> PlatformStr = {
	"INVALID",
	"WINDOWS",
	"LINUX",
	"MACOS",
};


/*template <class T>
std::string EnumToStr(T item)
{
	if (item >= T::COUNT || item < T::INVALID)
		return T::INVALID;
		// return ArchStr[0];

	return strList[(int)item];
}


template <class T>
T StrToEnum(std:vector<std::string> strList, std::string &plat)
{
}*/


std::string PlatformToStr(Platform platform)
{
	if (platform >= Platform::COUNT || platform < Platform::INVALID)
		return PlatformStr[0];

	return PlatformStr[(int)platform];
}

std::string ArchToStr(Arch arch)
{
	if (arch >= Arch::COUNT || arch < Arch::INVALID)
		return ArchStr[0];

	return ArchStr[(int)arch];
}


Platform StrToPlatform(std::string &plat)
{
	if (vec_contains(PlatformStr, plat))
		return (Platform)vec_index(PlatformStr, plat);

	return Platform::INVALID;
}

Arch StrToArch(std::string &arch)
{
	if (vec_contains(ArchStr, arch))
		return (Arch)vec_index(ArchStr, arch);

	return Arch::INVALID;
}


void SetPlatformMacros(StringMap &macros, Platform platform)
{
	macros["WINDOWS"] = platform == Platform::WINDOWS ? "1" : "0";
	macros["LINUX"] = platform == Platform::LINUX ? "1" : "0";
	macros["MACOS"] = platform == Platform::MACOS ? "1" : "0";
}


void SetArchMacros(StringMap &macros, Arch arch)
{
	macros["AMD64"] = arch == Arch::AMD64 ? "1" : "0";
	macros["I386"] = arch == Arch::I386 ? "1" : "0";
	macros["ARM"] = arch == Arch::ARM ? "1" : "0";
	macros["ARM64"] = arch == Arch::ARM64 ? "1" : "0";
}






