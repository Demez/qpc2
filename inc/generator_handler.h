#pragma once

#include "util.h"
#include "base_generator.h"



class GeneratorHandler
{
public:
	GeneratorHandler() {}

	static GeneratorHandler& GetHandler()
	{
		static GeneratorHandler handler;
		return handler;
	}

	void LoadGenerators();

	void RegisterGenerator(BaseGenerator* generator)
	{
		m_generators.push_back(generator);
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

	std::vector<BaseGenerator*> m_generators;
};


inline GeneratorHandler& GetGeneratorHandler()
{
	return GeneratorHandler::GetHandler();
}


