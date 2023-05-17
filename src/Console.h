#pragma once
#include <string>
#include <vector>
#include "Utils.h"

class Console
{
	using LogsContainer = std::vector<std::pair<std::string, TextColor_>>;
public:
	class InputStream
	{
	public:
		InputStream(Console* pConsole)
			:
			p_Console(pConsole)
		{}

		void SetConsole(Console* pConsole)
		{
			p_Console = pConsole;
		}
		void operator<<(const std::string& line)
		{
			p_Console->AddLog(line);
		}
		void Log(const std::string& line)
		{
			p_Console->AddLog(line);
		}
		void ErrorLog(const std::string& line)
		{
			p_Console->AddLog(line, TextColor_Red);
		}
		void CommandLog(const std::string& line)
		{
			p_Console->AddLog(line, TextColor_Green);
		}
		void Clear()
		{
			p_Console->Clear();
		}

	private:
		Console* p_Console = nullptr;
	};

public:
	Console() = default;
	~Console() = default;

	void AddLog(const std::string& line, TextColor_ color = TextColor_White);
	void Draw();
	void Clear();

private:
	LogsContainer m_Logs;
};

