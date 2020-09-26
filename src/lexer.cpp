// basic file operations
#include <iostream>
#include <fstream>
#include <string>
#include <list>

#include "lexer.h"
#include "logging.h"
#include "conditions.h"


// temporary lexer, need to make a better lexer


QPCBlock::QPCBlock(std::string key, std::vector<std::string> values, std::string condition, int lineNum):
	QPCBlock(NULL, key, values, condition, lineNum)
{	
}


QPCBlock::QPCBlock(QPCBlock* parent, std::string key, std::vector<std::string> values, std::string condition, int lineNum)
{
	m_parent = parent;
	m_lineNum = lineNum;
	m_key = key;
	m_values = values;
	m_condition = condition;
	m_valid = true;
}


QPCBlock::QPCBlock(bool valid)
{
	m_parent = NULL;
	m_valid = valid;
	m_lineNum = 0;
	m_key = "";
	m_condition = "";
}


QPCBlock::~QPCBlock()
{
	for (QPCBlock* block: m_items)
	{
		delete block;
	}
}


QPCBlock& QPCBlock::GetInvalid()
{
	static const QPCBlock invalid;
	return const_cast<QPCBlock&>(invalid);
}


std::vector<std::string> QPCBlock::GetList()
{
	std::vector<std::string> list = {m_key};
	list.insert(list.begin(), m_values.begin(), m_values.end());
	return list;
}

std::vector<QPCBlock*> QPCBlock::GetItemsCond(StringMap &macros)
{
	std::vector<QPCBlock*> items;

	for (QPCBlock* item: m_items)
	{
		if (item && item->SolveCondition(macros))
			items.push_back(item);
	}

	return items;
}


std::string QPCBlock::GetValue(int index)
{
	return m_values.size() > index ? m_values[index] : ""; 
}


const char* QPCBlock::GetFilePath()
{
	return (m_parent) ? m_parent->GetFilePath() : "";
}


std::string QPCBlock::GetFormattedInfo()
{
	std::string info;

	std::string path = GetFilePath();
	if (!path.empty())
		info += ("File \"" + path + "\" : ");

	if (m_lineNum != -1)
	{
		char lineStr[15] = {};
		sprintf(lineStr, "Line %d : ", m_lineNum);
		info += lineStr;
	}

	info += ("Key \"" + m_key + "\"");

	return info;
}


/*void QPCBlock::warning(const char* str, va_list args)
{
	std::string info = GetFormattedInfo();
	info += str;

	::warningVA( (info).c_str(), args);
}*/

void QPCBlock::warning(const char* str, ...)
{
	char finalStr[1024] = {};
	sprintf(finalStr, "%s", str);

	const char* vaStr;
	va_list args;
	va_start(args, str);
	{
		vaStr = va_arg(args, const char*);
		char* strCheck = strstr(finalStr, "%s");

		if (strCheck)
		{
			char tmpStr[1024] = {};
			sprintf(tmpStr, finalStr, vaStr);
			sprintf(finalStr, "%s", tmpStr);
		}
		else
		{
			sprintf(finalStr, "%s%s", finalStr, vaStr);
		}
	}
	va_end(args);

	std::string info = GetFormattedInfo();
	info += "\n          ";
	info += finalStr;

	::warning( (info).c_str() );
}



QPCBlockRoot::QPCBlockRoot(std::string filePath): QPCBlock()
{
	m_filePath = filePath;
}

QPCBlockRoot::~QPCBlockRoot()
{	
}

const char* QPCBlockRoot::GetFilePath()
{
	return m_filePath.c_str();
}


QPCLexer::QPCLexer(std::string &path)
{
	m_filePath = path;
	m_file = "";
	std::ifstream qpcFile(path, std::ifstream::in);

	// put it into a huge string to parse through
	// slow as FUCK
	std::string line;
	while (std::getline(qpcFile, line))
	{
		m_file += line + "\n";
	}

	m_fileLen = m_file.size();

	qpcFile.close();
}

QPCLexer::~QPCLexer()
{
}


char QPCLexer::NextSymbol()
{
	char ch;
	while (m_chari < m_fileLen)
	{
		ch = m_file[m_chari];

		if (CheckItem(ch))
		{
			IncChar();
			return ch;
		}

		else if (CheckEscape(ch))
			IncChar(2);

		else if (CheckComment(ch))
			SkipComment();

		else if (ch == '\n')
			NextLine();

		else if (!CheckSpace(ch))
			break;

		IncChar();
	}

	return '\0';
}


std::string QPCLexer::NextKey()
{
	char ch;
	std::string str = "";
	int line_num = 0;

	while (m_chari < m_fileLen)
	{
		ch = m_file[m_chari];

		if (CheckItem(ch))
			break;

		else if (CheckSpace(ch))
		{
			if (str != "")
			{
				break;
			}
		}

		else if (CheckQuote(ch))
		{
			str = ReadQuote(ch);
			break;
		}

		else if (CheckComment(ch))
			SkipComment();

		else if (ch == '\n')
			NextLine();

		else
			str += ch;

		IncChar();
	}

	return str;
}


std::vector<std::string> QPCLexer::NextValueList()
{
	char ch;
	std::vector<std::string> values;
	std::string current_value = "";

	while (m_chari < m_fileLen)
	{
		ch = m_file[m_chari];
		if (CheckItem(ch))
			break;

		else if (CheckSpace(ch))
		{
			if (current_value != "")
			{
				if (current_value != "\\")
				{
					values.push_back(current_value);
					current_value = "";
				}
			}
		}

		else if (CheckQuote(ch))
		{
			values.push_back(ReadQuote(ch));
			continue;
		}

		else if (CheckEscape(ch))
		{
			IncChar(2);
			current_value += m_file[m_chari];
		}

		else if (ch == '\n')
		{
			if (current_value.empty())
			{
				break;
			}
			else if (current_value.back() != '\\')
			{
				if (current_value[0] != '[' && current_value.back() != ']')
				{
					values.push_back(current_value);
				}

				break;
			}
			else
			{
				NextLine();
			}
		}

		else if (CheckComment(ch))
		{
			SkipComment();
		}

		else
		{
			if (CheckCondition(m_file[m_chari]))
				break;

			if (current_value == "\\")
				current_value = "";

			current_value += ch;
		}

		IncChar();
	}

	return values;
}


bool QPCLexer::CheckEscape(char ch)
{
	if (ch == '\\' && (PeekChar() == '\'' || PeekChar() == '"' || PeekChar() == '\\'))
		return true;
	return false;
}


bool QPCLexer::CheckComment(char ch)
{
	if (ch == '/' && (PeekChar() == '/' || PeekChar() == '*'))
		return true;
	return false;
}


void QPCLexer::SkipComment()
{
	IncChar();
	char ch = m_file[m_chari];
	if (ch == '/')
	{
		while (m_chari < m_fileLen)
		{
			IncChar();
			if (m_file[m_chari] == '\n')
			{
				NextLine();
				break;
			}
		}
	}

	else if (ch == '*')
	{
		while (m_chari < m_fileLen)
		{
			if (m_file[m_chari] == '*' && PeekChar() == '/')
				break;

			else if (m_file[m_chari] == '\n')
				NextLine();

			IncChar();
		}
	}
}


std::string QPCLexer::NextCondition()
{
	char ch;
	std::string cond = "";

	while (m_chari < m_fileLen)
	{
		ch = m_file[m_chari];

		if (CheckItem(ch))
			break;

		else if (ch == '[' || CheckSpace(ch))
		{
			IncChar();
			continue;
		}

		else if (ch == ']')
		{
			IncChar();
			break;
		}

		else if (ch == '\n')
		{
			NextLine();
			IncChar();
			break;
		}

		else if (CheckComment(ch))
			SkipComment();

		else
		{
			cond += ch;
		}

		IncChar();
	}

	return cond;
}


std::string QPCLexer::ReadQuote(char qchar)
{
	char ch;
	std::string quote = "";

	while (m_chari < m_fileLen)
	{
		IncChar();
		ch = m_file[m_chari];
		if (CheckEscape(ch))
		{
			quote += PeekChar();
			IncChar();
		}

		else if (ch == qchar)
		{
			break;
		}

		else
		{
			quote += ch;
		}
	}

	IncChar();
	return quote;
}


void ParseRecursive(QPCLexer& lexer, QPCBlock* block)
{
	while (lexer.m_chari < lexer.m_fileLen)
	{
		std::string key = lexer.NextKey();

		if (key == "")
		{
			if (lexer.NextSymbol() == '}')
			{
				return;
			}
			else if (lexer.m_chari >= lexer.m_fileLen)
			{
				if (block->m_parent)
					block->warning("Brackets do not close at end of file!!");

				return;
			}
		}

		std::vector<std::string> values = lexer.NextValueList();
		std::string condition = lexer.NextCondition();

		QPCBlock* subBlock = new QPCBlock(block, key, values, condition, lexer.m_linei);

		char nextSymbol = lexer.NextSymbol();

		if (nextSymbol == '{')
		{
			ParseRecursive(lexer, subBlock);
		}

		else if (nextSymbol == '}')
		{
			block->m_items.push_back(subBlock);
			return;
		}

		block->m_items.push_back(subBlock);
	}
}


QPCBlockRoot* ReadFile(std::string &filePath)
{
	QPCLexer lexer = QPCLexer(filePath);
	QPCBlockRoot* qpcFileRoot = new QPCBlockRoot(filePath);

	ParseRecursive(lexer, qpcFileRoot);

	return qpcFileRoot;
}



