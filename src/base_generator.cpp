#include "base_generator.h"
#include "generator_handler.h"


BaseGenerator::BaseGenerator(std::string argName, std::string name)
{
	m_argName = argName;
	m_name = name;
	GetGeneratorHandler().RegisterGenerator(this);
}

