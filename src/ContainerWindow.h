#pragma once
#include <functional>
#include <vector>
#include <string>
#include <map>

#include "glad/glad.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "TableInfo.h"
#include "InputForm.h"
#include "Console.h"
#include "Utils.h"`

class ContainerWindow
{
public:
    ContainerWindow() = default;
    ContainerWindow(const std::string& label, const ImVec2& position, const ImVec2& size = ImVec2(100, 100), int flags = 0)
        :
        m_Title(label), m_Position(position), m_Size(size), m_Flags(flags)
    {}

    // Widget setters
    ContainerWindow& SetSeparator();
    ContainerWindow& SetButton(const std::string& label, const std::function<void()>& callback);
    ContainerWindow& SetSliderFloat(const std::string& label, float* value, float minValue, float maxValue);
    ContainerWindow& SetCheckbox(const std::string& label, bool* value);
    ContainerWindow& SetText(const std::string& label, TextColor_ color = TextColor_::TextColor_White);
    ContainerWindow& SetConsole(const std::string& label, Console::InputStream** ppInputStream);
    ContainerWindow& SetCombo(const std::string& label, const std::vector<std::string>& items, std::string* pSelectedItem, std::function<void()> callback = []{});
    ContainerWindow& SetForm(const std::string& label, const std::string& type, const TableInfo& tableInfo, Scema* pScema, Console::InputStream* logStream, InputForm::Values** ppValues);

    // Property setters
    ContainerWindow& SetSize(const ImVec2& size);
    ContainerWindow& SetPosition(const ImVec2& position);

    void Render();

private:
    int m_Flags;
    ImVec2 m_Size;
    ImVec2 m_Position;
    std::string m_Title;

    std::vector<std::string> m_SortedLabels;
    std::map<std::string, std::function<void()>> m_Widgets;

    static std::vector<ImVec4> s_Colors;
};

