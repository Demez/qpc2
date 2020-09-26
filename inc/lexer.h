#pragma once

#include "util.h"
#include "conditions.h"
#include <iostream>
#include <fstream>
#include <string>
// #include <list>
#include <vector>


class QPCBlock
{
public:
	QPCBlock(std::string key, std::vector<std::string> values, std::string condition, int lineNum = 0);
	QPCBlock(QPCBlock* parent, std::string key, std::vector<std::string> values, std::string condition, int lineNum = 0);
	QPCBlock(bool valid = true);
	~QPCBlock();

	static QPCBlock& GetInvalid();

	inline bool IsValid()
	{
		return m_valid;
	}

	inline bool SolveCondition(StringMap &macros)
	{
		return ::SolveCondition(macros, m_condition);
	}

	bool                            m_valid = false;
	int                             m_lineNum;
	QPCBlock*                       m_parent;
	std::string                     m_key;
	std::vector<std::string>        m_values;
	std::string                     m_condition;
	std::vector<QPCBlock*>          m_items;

	std::vector<std::string>        GetList();
	std::string                     GetValue(int index = 0);
	std::vector<QPCBlock*>          GetItemsCond(StringMap &macros);

	virtual const char*             GetFilePath();
	virtual std::string             GetFormattedInfo();
	virtual void                    warning(const char* str, ...);
	// virtual void                    warning(const char* str, va_list args);
};


class QPCBlockRoot: public QPCBlock
{
public:
	QPCBlockRoot(std::string filePath);
	~QPCBlockRoot();

	const char* GetFilePath();
	std::string m_filePath;
};


class QPCLexer
{
public:
	int m_chari = 0;
	int m_linei = 1;
	int m_lineChar = 0;
	int m_fileLen;
	std::string m_filePath;
	std::string m_file;

	QPCLexer(std::string &path);
	~QPCLexer();

	std::string NextKey();
	std::string NextCondition();

	char NextSymbol();

	std::vector<std::string> NextValueList();

private:
	inline char PeekChar(int amount = 1)
	{
		if (m_chari + amount > m_fileLen)
			return '\0';

		return m_file[m_chari + amount];
	}

	// Increment Character Number and line number
	inline void IncChar(int amount = 1)
	{
		m_chari += amount;
		m_lineChar += amount;
	}

	inline void NextLine()
	{
		m_linei++;
		m_lineChar = 0;
	}

	bool QPCLexer::CheckChar(char ch, char ch1, char ch2)
	{
		if (ch == ch1 || ch == ch2)
			return true;
		return false;
	}

	bool CheckItem(char ch)         { return (ch == '{' || ch == '}'); }
	bool CheckQuote(char ch)        { return (ch == '"' || ch == '\''); }
	bool CheckSpace(char ch)        { return (ch == ' ' || ch == '\t'); }
	bool CheckCondition(char ch)    { return (ch == '[' || ch == ']'); }

	bool CheckEscape(char ch);
	bool CheckComment(char ch);

	void SkipComment();

	std::string ReadQuote(char ch);
};


QPCBlockRoot* ReadFile(std::string &filePath);


