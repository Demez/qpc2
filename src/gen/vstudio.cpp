#include "util.h"
#include "base_generator.h"
#include "project_manager.h"
#include "project.h"


const char* g_uuidCPP = "{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}";
const char* g_uuidFilter = "{2150E333-8FDC-42A3-9474-1A3956D46DE8}";


class VisualStudioGen: public BaseGenerator
{
public:
	VisualStudioGen(): BaseGenerator("vstudio", "Visual Studio Generator")
	{
		AddPlatform(Platform::WINDOWS);
		AddArch(Arch::I386);
		AddArch(Arch::AMD64);
	}

	bool DoesProjectNeedRebuild(ProjectInfo* proj)
	{
		return true;
	}

	void CreateProject(ProjectContainer* proj)
	{

	}
};


DECLARE_SINGLE_GENERATOR(VisualStudioGen)

