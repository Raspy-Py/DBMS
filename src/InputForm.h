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
	InputForm(const std::string& type, 
		const TableInfo& tableReflection, 
		Scema* pScema, 
		Console::InputStream* pLogStream, 
		Values** ppValues);

	void Render();

public:
	static std::vector<std::string> types;

private:
	static int PasteCallback(ImGuiInputTextCallbackData* data)
	{
		// Get the pasted text from the clipboard
		const char* clipboardText = ImGui::GetClipboardText();

		// Copy the pasted text to our input buffer
		strncpy(s_InputBuffer, clipboardText, c_BufferSize - 1);
		m_DirectQuery = s_InputBuffer;
		// Return 0 to indicate that we have handled the pasting
		return 0;
	}

private:
	std::unique_ptr<Values> m_Values;
	std::unique_ptr<Values> m_NewValues;
	std::string m_TableName;
	std::string m_Type;
	static std::string m_DirectQuery;

	Scema* p_Scema;
	Console::InputStream* p_LogStream;
	static const size_t c_BufferSize = 128;
	static char s_InputBuffer[c_BufferSize];
};
