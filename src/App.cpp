#include "App.h"
#include "Utils.h"
#include <random>

#include "imgui/imgui.h"

App::App()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Most of the magic numbers are gethered here
    m_Settings = { 
        .relMenuWndSize         = {0.4f, 1.0f},
        .relConsoleWndSize      = {0.6f, 0.9f},
        .relConsoleCtrlWndSize  = {0.6f, 0.1f},
    };
    m_SupportedOperation = InputForm::types;

    m_Window = std::make_unique<Window>(1280, 720, "DBMS");

    m_Menu = std::make_unique<Menu>(m_Window->Get());
    m_Menu->Render();
    m_ImGuiIO = &ImGui::GetIO();
    ImVec2 display = m_ImGuiIO->DisplaySize;

    m_Menu->AddWindow("Console control",
        ImVec2(0, 0),
        ImVec2(display.x * m_Settings.relConsoleCtrlWndSize.x, display.y * m_Settings.relConsoleCtrlWndSize.y))
        .SetButton("Clear", [this] { m_LogStream->Clear(); });

    Console::InputStream* pInputStream = nullptr;
    m_Menu->AddWindow("Console",
        ImVec2(0, display.y * m_Settings.relConsoleCtrlWndSize.y),
        ImVec2(display.x * m_Settings.relConsoleWndSize.x, display.y * m_Settings.relConsoleWndSize.y),
        ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoResize)
        .SetConsole("Output", &pInputStream);
    m_LogStream.reset(pInputStream);

    // Connecting to MySQL database
    m_Scema = std::make_unique<Scema>("soccer_cup", m_LogStream.get());
    m_Tables = m_Scema->EnumerateTables();
    m_SelectedTable =  m_Tables.at(0);
    m_SelectedOperation = m_SupportedOperation.at(0);

    m_Menu->AddWindow("Menu",
        ImVec2(display.x * m_Settings.relConsoleWndSize.x, 0),
        ImVec2(display.x * m_Settings.relMenuWndSize.x, display.y * m_Settings.relMenuWndSize.y))
        .SetSeparator()
        .SetCombo("Table", m_Tables, &m_SelectedTable, 
            [this]{ m_Menu->AlterWindow("Menu")
            .SetForm("Form",
                m_SelectedOperation, 
                m_Scema->GetTableInfo(m_SelectedTable),
                m_Scema.get(),
                m_LogStream.get(), 
                &m_InsertionFormValues); })
        .SetSeparator()
        .SetCombo("Operation", m_SupportedOperation, &m_SelectedOperation, 
            [this] { m_Menu->AlterWindow("Menu")
            .SetForm("Form",
                m_SelectedOperation,
                m_Scema->GetTableInfo(m_SelectedTable),
                m_Scema.get(),
                m_LogStream.get(),
                &m_InsertionFormValues);})
        .SetSeparator()
        .SetForm("Form",
            m_SelectedOperation,
            m_Scema->GetTableInfo(m_SelectedTable),
            m_Scema.get(),
            m_LogStream.get(),
            &m_InsertionFormValues)
        .SetSeparator();

    Window::SetDropCallBack([this](GLFWwindow* window, int count, const char** paths) { /* TODO: add callback*/});

    Window::SetFramebufferSizeCallBack([this](GLFWwindow* window, int width, int height) {
        m_Menu->AlterWindow("Console control")
            .SetPosition(ImVec2(0, 0))
            .SetSize(ImVec2(width * m_Settings.relConsoleCtrlWndSize.x, height * m_Settings.relConsoleCtrlWndSize.y));
        
        m_Menu->AlterWindow("Console")
            .SetPosition(ImVec2(0, height * m_Settings.relConsoleCtrlWndSize.y))
            .SetSize(ImVec2(width * m_Settings.relConsoleWndSize.x, height * m_Settings.relConsoleWndSize.y));

        m_Menu->AlterWindow("Menu")
            .SetPosition(ImVec2(width * m_Settings.relConsoleWndSize.x, 0))
            .SetSize(ImVec2(width * m_Settings.relMenuWndSize.x, height * m_Settings.relMenuWndSize.y));
        });

    // ImGui style
    //Menu::EmbraceTheDarkness();
    utils::ImGuiTheme1();

}

App::~App()
{
    glfwTerminate();
}

void App::Run()
{
    m_Clock.Restart();
    while (m_Window->Open())
    {
        float deltaTime = m_Clock.Stamp();

        // Processing user input
        ProcessEvents(deltaTime);

        // Rendering stuff goes here
        DoFrame(deltaTime);

        PollEvents(deltaTime);
        m_Window->SwapBuffers();
    }
}

void App::DoFrame(float dt)
{
    m_Window->Clear(0.105f, 0.105f, 0.105f);

    // Draw OpenGL stuff here
    
    // Draw ImGui stuff here
    m_Menu->Render();

    m_FrameCounter++;
}

void App::ProcessEvents(float dt)
{   
    // Keyboard input
    if (glfwGetKey(m_Window->Get(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        m_Window->Close();

    // Ignore mouse input when it's captured by ImGui
    if (m_ImGuiIO->WantCaptureMouse)
        return;

    // Mouse input
}

void App::PollEvents(float dt)
{
    glfwPollEvents();
}
