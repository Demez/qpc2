#pragma once

#include <string>
#include <unordered_map>
#include <initializer_list>
#include <filesystem>

namespace fs = std::filesystem;


extern const char* g_hashDir;
extern std::vector<std::string> g_extsSource;
extern std::vector<std::string> g_extsHeader;

// only so i don't need to type all this out so many times
typedef std::unordered_map<std::string, std::string> StringMap;


template <class T>
size_t vec_index(std::vector<T> &vec, T item)
{
	for (size_t i = 0; i < vec.size(); i++)
	{
		if (vec[i] == item)
			return i;
	}

	return SIZE_MAX;
}


template <class T>
void vec_remove(std::vector<T> &vec, T item)
{
	vec.erase(vec.begin() + vec_index(vec, item));
}


template <class T>
bool vec_contains(std::vector<T> &vec, T item)
{
	for (T addedItem: vec)
	{
		if (addedItem == item)
			return true;
	}

	return false;
}


enum class Arch
{
	INVALID = 0,
	AMD64,
	I386,
	ARM,
	ARM64,
	COUNT,
};


enum class Platform
{
	INVALID = 0,
	WINDOWS,
	LINUX,
	MACOS,
	// ANDROID,
	COUNT,
};


Platform            StrToPlatform(std::string &plat);
Arch                StrToArch(std::string &plat);

std::string         PlatformToStr(Platform platform);
std::string         ArchToStr(Arch arch);

void                SetPlatformMacros(StringMap &macros, Platform platform);
void                SetArchMacros(StringMap &macros, Arch arch);

std::string         GetCurrentDir();
int                 ChangeDir(std::string &path);
int                 CreateDir(std::string &path);
bool                FileExists(std::string &path);

void                ReplaceString(std::string& str, const std::string& from, const std::string& to);
void                GetLongestString(std::vector<std::string> &strVec, std::string &longest);
std::string         GetLongestString(std::vector<std::string> &strVec);

bool                CheckExtSource(std::string& ext);
bool                CheckExtHeader(std::string& ext);


class PlatformItem
{
public:
	void AddPlatform(Platform platform);
	void RemovePlatform(Platform platform);
	inline bool SupportsPlatform(Platform platform);

	std::vector<Platform> m_platforms;
};


// shit name
class PlatArchItem: PlatformItem
{
public:
	void AddArch(Arch arch);
	void RemoveArch(Arch arch);
	inline bool SupportsArch(Arch arch);

	std::vector<Arch> m_archs;
};




