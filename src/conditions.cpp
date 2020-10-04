#include "conditions.h"
#include <regex>


std::regex g_condOperators("(\\(|\\)|\\|\\||\\&\\&|>=|<=|==|!=|>|<)");


// finds macros that could potentially work in this string
StringUMap GetPotentialMacros(StringUMap &macros, const std::string &string)
{
	StringUMap potentialMacros;
	for (auto const&[key, value]: macros)
	{
		std::string macro = "$" + key + "$";
		if (string.find(macro) != std::string::npos)
		{
			potentialMacros[macro] = value;
		}
	}

	return potentialMacros;
}


// why tf did i put this in conditions.cpp
std::string ReplaceMacros(StringUMap &macros, const std::string &string)
{
	if (string.find("$") == std::string::npos)
		return string;

	std::string newString = string;

	StringUMap potentialMacros = GetPotentialMacros(macros, string);

	while (potentialMacros.size() > 0)
	{
		std::string macro = GetLongestString(potentialMacros);
		
		if (newString.find(macro) != std::string::npos)
		{
			ReplaceString(newString, macro, potentialMacros[macro]);
		}

		potentialMacros.erase(macro);
	}

	return newString;
}


void ReplaceConditionValue(StringUMap &macros, std::string &string)
{
	bool notOperator = (str_count(string, "!") % 2);

	// get rid of the tokens if there are any
	if (string[0] == '$' && string.back() == '$')
	{
		std::string item = string.substr(1, string.length() - 2);

		StringUMap::iterator pos = macros.find(item);
		if (pos != macros.end())
		{
			string = notOperator ? "0": macros[item];
		}
		else
		{
			string = notOperator ? "1": "0";
		}
	}
	else
	{
		if (notOperator)
			string = "1";
	}
}


std::vector<std::string> SplitCondition(std::string cond)
{
	std::vector<std::string> splitCond;

	std::sregex_token_iterator iter(cond.begin(), cond.end(), g_condOperators, -1);
	std::sregex_token_iterator end;

	for ( ; iter != end; ++iter)
	{
		if (splitCond.size() % 2)
		{
			// get the operator like an idiot
			std::string prevItem = splitCond.back();

			size_t lEnd = cond.find(prevItem) + prevItem.length();
			size_t rPos = cond.find(*iter);
			std::string oper = cond.substr(lEnd, rPos - lEnd);

			splitCond.push_back(oper);
		}

		splitCond.push_back(*iter);
	}

	return splitCond;
}


int StrToIntCond(std::string& cond)
{
	if (cond.empty())
	{
		return 0;
	}

	int value = atoi(cond.c_str());

	// atoi returns 0 if no conversion could be made
	// so just check if we aren't actually using 0 here before setting it to 1
	if (value == 0 && cond != "0") 
	{
		value = 1;
	}

	return value;
}


void SolveSingleCondition(StringUMap &macros, std::vector<std::string> &cond)
{
	size_t pos = 1;
	int result = 0;

	if (vec_contains(cond, std::string("==")))
	{
		pos = vec_index(cond, std::string("=="));
		result = (cond[pos - 1] == cond[pos + 1]);
	}

	else if (vec_contains(cond, std::string("!=")))
	{
		pos = vec_index(cond, std::string("!="));
		result = (cond[pos - 1] != cond[pos + 1]);
	}

	else if (vec_contains(cond, std::string("&&")))
	{
		pos = vec_index(cond, std::string("&&"));
		result = (StrToIntCond(cond[pos - 1]) > 0 && StrToIntCond(cond[pos + 1]) > 0);
	}

	else if (vec_contains(cond, std::string("||")))
	{
		pos = vec_index(cond, std::string("||"));
		result = (StrToIntCond(cond[pos - 1]) > 0 || StrToIntCond(cond[pos + 1]) > 0);
	}

	cond[pos] = std::to_string(result);
	cond.erase(cond.begin() + pos + 1);
	cond.erase(cond.begin() + pos - 1);
}


size_t GetNestedCondEndPos(std::string cond)
{
	size_t startPos =  cond.find("(") + 1;
	size_t endPos = startPos;

	for (size_t depth = 1; endPos < cond.length(); ++endPos)
	{
		if (cond[endPos] == '(')
			depth++;

		else if (cond[endPos] == ')')
			depth--;

		if (depth == 0)
			break;
	}

	return endPos - 1;
}


bool SolveCondition(StringUMap &macros, std::string cond)
{
	if (cond.empty())
		return true;

	// handle nested conditions first
	while (cond.find("(") != std::string::npos)
	{
		std::string nestedCond = cond.substr( cond.find("(") + 1, GetNestedCondEndPos(cond) - cond.find("(") );
		int result = SolveCondition(macros, nestedCond);
		ReplaceString(cond, "(" + nestedCond + ")", std::to_string(result));
	}

	std::vector<std::string> splitCond = SplitCondition(cond);

	for (size_t i = 0; i < splitCond.size(); i += 2)
	{
		ReplaceConditionValue(macros, splitCond[i]);
	}

	while (splitCond.size() > 1)
	{
		SolveSingleCondition(macros, splitCond);
	}

	return StrToIntCond(splitCond[0]);
}



