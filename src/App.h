#pragma once
#include "SafeWindows.h"
#include "Exception.h"
#include "Window.h"
#include "Clock.h"
#include "Filter.h"
#include "Menu.h"
#include "Console.h"
#include "Scema.h"
#include "InputForm.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <memory>
#include <vector>
#include <mysql/jdbc.h>

class App
{
	struct Settings
	{
		ImVec2 relMenuWndSize;
		ImVec2 relConsoleWndSize;
		ImVec2 relConsoleCtrlWndSize;
	};
public:
	App();
	~App();

	void Run();

private:
	void DoFrame(float dt);
	void ProcessEvents(float dt);
	void PollEvents(float dt);

private:
	size_t m_FrameCounter;
	Clock m_Clock;
	Settings m_Settings;

	std::unique_ptr<Menu> m_Menu;
	std::unique_ptr<Window> m_Window;
	std::unique_ptr<Console::InputStream> m_LogStream;
	InputForm::Values* m_InsertionFormValues = nullptr;
	std::unique_ptr<Scema> m_Scema;

	std::vector<std::string> m_Tables;
	std::string m_SelectedTable;

	std::vector<std::string> m_SupportedOperation;
	std::string m_SelectedOperation;

	ImGuiIO* m_ImGuiIO;
	const size_t c_TextureSize = 128;
};
