#pragma once

#include "util.h"
#include "base_generator.h"


class GeneratorHandler
{
public:
	GeneratorHandler();
	~GeneratorHandler();

	static GeneratorHandler& GetHandler()
	{
		static GeneratorHandler handler;
		return handler;
	}

	int GetGeneratorID(BaseGenerator* generator)
	{
		if (vec_contains(m_generators, generator))
			return vec_index(m_generators, generator);

		return -1;
	}
	
	std::vector<std::string> GetArgNames()
	{
		std::vector<std::string> argNames;

		for (BaseGenerator* gen: m_generators)
			argNames.push_back(gen->m_argName);

		return argNames;
	}

	void LoadGenerators();
	void LoadGeneratorModule(fs::path &filePath);

	std::vector<BaseGenerator*> m_generators;
	std::vector<GeneratorInterface*> m_genIntList;
};


inline GeneratorHandler& GetGeneratorHandler()
{
	return GeneratorHandler::GetHandler();
}


