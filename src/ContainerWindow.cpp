#include "ContainerWindow.h"


ContainerWindow& ContainerWindow::SetSeparator()
{
    static size_t uniqueSeparatorID = 0;
    std::string label = "separator_" + std::to_string(uniqueSeparatorID++);

    auto pWidget = m_Widgets.find(label);

    if (pWidget == m_Widgets.end())
        m_SortedLabels.push_back(label);

    m_Widgets[label] = [] {
        ImGui::Separator();
    };

    return *this;
}

ContainerWindow& ContainerWindow::SetButton(const std::string& label, const std::function<void()>& callback)
{
    auto pWidget = m_Widgets.find(label);

    if (pWidget == m_Widgets.end())
        m_SortedLabels.push_back(label);

    m_Widgets[label] = [label, callback] {
        // Get the available width for the button
        float availableWidth = ImGui::GetContentRegionAvail().x;

        // Calculate the position to center the button
        float buttonWidth = 100.0f; // Width of the button
        float buttonX = (availableWidth - buttonWidth) * 0.5f; // Calculate the x position

        // Set the button's position
        ImGui::SetCursorPosX(buttonX);

        // Render the centered button
        if (ImGui::Button(label.c_str(), ImVec2(buttonWidth, 0)))
        {
            callback();
        }
    };
    return *this;
}

ContainerWindow& ContainerWindow::SetSliderFloat(const std::string& label, float* value, float minValue, float maxValue)
{
    auto pWidget = m_Widgets.find(label);

    if (pWidget == m_Widgets.end())
        m_SortedLabels.push_back(label);

    m_Widgets[label] = [label, value, minValue, maxValue] {
        ImGui::SliderFloat(AS_PREFIX(label.c_str()), value, minValue, maxValue);
    };
    return *this;
}

ContainerWindow& ContainerWindow::SetCheckbox(const std::string& label, bool* value)
{
    auto pWidget = m_Widgets.find(label);

    if (pWidget == m_Widgets.end())
        m_SortedLabels.push_back(label);

    m_Widgets[label] = [label, value] {
        ImGui::Checkbox(label.c_str(), value);
    };
    return *this;
}

ContainerWindow& ContainerWindow::SetText(const std::string& label, TextColor_ color)
{
    auto pWidget = m_Widgets.find(label);

    if (pWidget == m_Widgets.end())
        m_SortedLabels.push_back(label);

    m_Widgets[label] = [label, color] {
        ImGui::TextColored(Colors[(size_t)color], label.c_str());
    };
    return *this;
}

ContainerWindow& ContainerWindow::SetConsole(const std::string& label, Console::InputStream** ppInputStream)
{
    auto pWidget = m_Widgets.find(label);

    if (pWidget == m_Widgets.end())
        m_SortedLabels.push_back(label);

    Console* pConsole = new Console;
    *ppInputStream = new Console::InputStream(pConsole);

    m_Widgets[label] = [label, pConsole] {
        pConsole->Draw();
    };
    return *this;
}

ContainerWindow& ContainerWindow::SetCombo(const std::string& label, const std::vector<std::string>& items, std::string* pSelectedItem, std::function<void()> callback)
{
    auto pWidget = m_Widgets.find(label);

    if (pWidget == m_Widgets.end())
        m_SortedLabels.push_back(label);
    int selectedIndex = 0;
    int lastSelectedIndex = 0;
    m_Widgets[label] = [label, 
        comboItems = items, 
        selectedIndex = selectedIndex,          // This variable is static, but differnt for every lambda instance
        lastSelectedIndex = lastSelectedIndex,  // This variable is static, but differnt for every lambda instance
        pSelectedItem, callback] () mutable{
        if (comboItems.empty())
        {
            comboItems.push_back("[no items to select]");
        }

        if (ImGui::BeginCombo(AS_PREFIX(FORMAT("%20s", label.c_str()).c_str()), comboItems[selectedIndex].c_str()))
        {
            for (int i = 0; i < comboItems.size(); i++)
            {
                bool isSelected = (i == selectedIndex);
                if (ImGui::Selectable(comboItems[i].c_str(), isSelected))
                {
                    *pSelectedItem = comboItems[i];
                    selectedIndex = i;
                }
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        if (selectedIndex != lastSelectedIndex)
        {
            lastSelectedIndex = selectedIndex;
            callback();
        }
    };
    return *this;
}

ContainerWindow& ContainerWindow::SetForm(const std::string& label, const std::string& type, const TableInfo& tableInfo, Scema* pScema, Console::InputStream* logStream, InputForm::Values** ppValues)
{
    auto pWidget = m_Widgets.find(label);

    if (pWidget == m_Widgets.end())
        m_SortedLabels.push_back(label);

    // TODO: fix memory lick - when reseting widget, old InputForm is not being deleted
    auto pInputForm = new InputForm(type, tableInfo, pScema, logStream, ppValues);

    m_Widgets[label] = [pInputForm]()
    {
        pInputForm->Render();
    };

    return *this;
}

ContainerWindow& ContainerWindow::SetSize(const ImVec2& size)
{
    m_Size = size;
    return *this;
}

ContainerWindow& ContainerWindow::SetPosition(const ImVec2& position)
{
    m_Position = position;
    return *this;
}

void ContainerWindow::Render()
{
    ImGui::SetNextWindowPos(m_Position);
    ImGui::SetNextWindowSize(m_Size);
    ImGui::Begin(m_Title.c_str(), (bool*)0, m_Flags);
    
    for (const auto& label : m_SortedLabels)
    {
        auto widget = m_Widgets.find(label);
        if (widget != m_Widgets.end())
        {
            // Calling a lambda bounded to a widget
            widget->second();
        }
    }

    ImGui::End();
}
