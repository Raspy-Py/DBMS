#include <unordered_map>
#include <map>
#include <string>
#include <functional>

#include "Console.h"
#include "TableInfo.h"
#include "ContainerWindow.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

class Menu 
{
public:
    Menu(GLFWwindow* window);
    ~Menu();
    
    ContainerWindow& AddWindow(const std::string& label, const ImVec2& position, const ImVec2& size, int flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
    ContainerWindow& AlterWindow(const std::string& label);

    void Render();

    // Imgui styles from @janekb04 
    // https://github.com/ocornut/imgui/issues/707#issuecomment-917151020
    static void EmbraceTheDarkness();

private:
    static std::string LabelPrefix(const std::string& label);

private:
    std::unordered_map<std::string, ContainerWindow> m_ContainerWindows;
};
