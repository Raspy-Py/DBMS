#include "Console.h"

void Console::AddLog(const std::string& line, TextColor_ color)
{
	m_Logs.emplace_back(line, color);
}

void Console::Draw()
{
	ImVec2 sectionPos = ImGui::GetCursorScreenPos();
	ImVec2 sectionSize(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y); 
	ImVec2 contentPos = ImVec2(sectionPos.x + 10.0f, sectionPos.y + 10.0f);

	ImGui::SetCursorScreenPos(contentPos);

	for (const auto& [log, colorindex] : m_Logs)
	{
		ImGui::SetCursorPosX(contentPos.x);
		ImGui::TextColored(Colors[(size_t)colorindex], log.c_str());
	}
}

void Console::Clear()
{
	m_Logs.clear();
}
