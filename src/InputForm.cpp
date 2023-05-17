#include "InputForm.h"

#include "Utils.h"
#include "Exception.h"

std::vector<std::string> InputForm::types = {
    "Insert",
    "Delete",
    "Update"
};

InputForm::InputForm(const std::string& type, const TableInfo& tableInfo, Scema* pScema, Console::InputStream* pLogStream, Values** ppValues)
    :
    p_Scema(pScema), p_LogStream(pLogStream), m_Type(type)
{
    if (p_Scema == nullptr)
    {
        throw EXCEPTION("Error: pScema should not be nullptr.");
    }

    m_TableName = tableInfo.GetName();
    m_Values = std::make_unique<Values>();
	*ppValues = m_Values.get();
    
    if (m_Type == "Update")
        m_NewValues = std::make_unique<Values>();

    const auto& columns = tableInfo.GetColumns();
    for (const auto& column : columns)
    {
        // TODO: validate input by data type
        m_Values->emplace_back(column.name, "");
        m_Values->back().second.reserve(c_BufferSize);

        if (m_Type == "Update")
        {
            m_NewValues->emplace_back("new_" + column.name, "");
            m_NewValues->back().second.reserve(c_BufferSize);
        }
    }
}

void InputForm::Render()
{
    for (auto& [inputLabel, data] : *m_Values)
    {
        char buffer[c_BufferSize]{};
        strcpy(buffer, data.c_str());

        // Render the input text widget
        if (ImGui::InputText(AS_PREFIX(FORMAT("%20s", inputLabel.c_str()).c_str()), buffer, c_BufferSize))
        {
            // Update the string with the modified buffer
            data = buffer;
        }
    }

    if (m_Type == "Update")
    {
        ImGui::SeparatorText("New values:");
        for (auto& [inputLabel, data] : *m_NewValues)
        {
            char buffer[c_BufferSize]{};
            strcpy(buffer, data.c_str());

            // Render the input text widget
            if (ImGui::InputText(AS_PREFIX(FORMAT("%20s", inputLabel.c_str()).c_str()), buffer, c_BufferSize))
            {
                // Update the string with the modified buffer
                data = buffer;
            }
        }
    }

    // Render the centered button
    float availableWidth = ImGui::GetContentRegionAvail().x;
    float buttonWidth = 100.0f;
    float buttonX1 = (availableWidth - (m_Type != "Insert" ? buttonWidth * 2.0f + 10.0f : buttonWidth)) * 0.5f;
    float buttonX2 = availableWidth * 0.5f + 5.0f;
    ImGui::SetCursorPosX(buttonX1);
    if (ImGui::Button(m_Type.c_str(), ImVec2(buttonWidth, 0)))
    {
        if (m_Type == "Insert") p_Scema->Insert(m_TableName, *m_Values);
        else if (m_Type == "Update") p_Scema->Update(m_TableName, *m_Values, *m_NewValues);
        else if (m_Type == "Delete") p_Scema->Delete(m_TableName, *m_Values);
    }

    if (m_Type == "Insert")
        return;

    ImGui::SameLine();
    ImGui::SetCursorPosX(buttonX2);
    if (ImGui::Button("Find", ImVec2(buttonWidth, 0)))
    {
        p_Scema->Find(m_TableName, *m_Values, *(m_Type == "Delete" ? m_Values : m_NewValues));
    }
}
