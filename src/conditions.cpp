#include "conditions.h"




// this doesn't modify the input in case we want to set a new var in one line
// also this could probably be way better
std::string ReplaceMacros(StringMap &macros, std::string &string)
{
	if (string.find("$") == std::string::npos)
		return string;

	std::string newString = string;
	StringMap::iterator it = macros.begin();

	// find macros that could potentially work here
	StringMap potentialMacros;
	std::vector<std::string> keys;
	while (it != macros.end())
	{
		std::string macro = "$" + it->first + "$";
		if (string.find(macro) != std::string::npos)
		{
			potentialMacros[macro] = it->second;
			keys.push_back(macro);
		}

		it++;
	}

	while (potentialMacros.size() > 0)
	{
		// get the longest macro
		std::string macro = GetLongestString(keys);
		
		if (string.find(macro) != std::string::npos)
		{
			ReplaceString(newString, macro, potentialMacros[macro]);
		}

		potentialMacros.erase(macro);
		vec_remove(keys, macro);
	}

	return newString;
}


bool SolveCondition(StringMap &macros, std::string &condition)
{
	return true;
}



