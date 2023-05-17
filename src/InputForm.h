#pragma once
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

#include "imgui/imgui.h"
#include "TableInfo.h"
#include "Console.h"
#include "Scema.h"

class InputForm
{
public:
	using Values = std::vector<std::pair<std::string, std::string>>;

public:
	InputForm(const std::string& type, const TableInfo& tableReflection, Scema* pScema, Console::InputStream* pLogStream, Values** ppValues);

	void Render();

public:
	static std::vector<std::string> types;

private:


private:
	std::unique_ptr<Values> m_Values;
	std::unique_ptr<Values> m_NewValues;
	std::string m_TableName;
	std::string m_Type;

	Scema* p_Scema;
	Console::InputStream* p_LogStream;
	static const size_t c_BufferSize = 128;
};

