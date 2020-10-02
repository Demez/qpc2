#include "util.h"
#include "base_generator.h"


class CMakeGenerator: public BaseGenerator
{
public:
	CMakeGenerator(): BaseGenerator("cmake", "CMakeLists.txt Generator")
	{
		AddPlatform(Platform::WINDOWS);
		AddPlatform(Platform::LINUX);
		AddPlatform(Platform::MACOS);

		AddArch(Arch::I386);
		AddArch(Arch::AMD64);
		AddArch(Arch::ARM);
		AddArch(Arch::ARM64);
	}

	bool DoesProjectNeedRebuild(ProjectInfo* proj)
	{
		return true;
	}

	void CreateProject(ProjectContainer* proj)
	{

	}
};


DECLARE_SINGLE_GENERATOR(CMakeGenerator)

