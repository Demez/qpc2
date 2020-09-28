#pragma once

#include "util.h"

std::string ReplaceMacros(StringMap &macros, const std::string &string);
bool SolveCondition(StringMap &macros, std::string condition);




