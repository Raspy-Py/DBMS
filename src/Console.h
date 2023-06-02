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
			InfoLog(line.c_str());
		}
		void Log(const std::string& line)
		{
			p_Console->AddLog(line);
			InfoLog(line.c_str());
		}
		void ErrorLog(const std::string& line)
		{
			p_Console->AddLog(line, TextColor_Red);
			Error(line.c_str());
		}
		void CommandLog(const std::string& line)
		{
			p_Console->AddLog(line, TextColor_Green);
			InfoLog(line.c_str());
		}
		void Clear()
		{
			p_Console->Clear();
		}
		void ScrollDown()
		{
			p_Console->ScrollDown();
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
	void ScrollDown();

private:
	LogsContainer m_Logs;
	int m_ScrolledDown = 0;
};

