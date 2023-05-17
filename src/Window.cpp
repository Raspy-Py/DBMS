#include "Window.h"
#include "Exception.h"

#include <sstream>
#include <fstream>

std::queue<std::string> Window::s_DropedFilesPaths;
std::function<void(GLFWwindow*, int, int)> Window::s_FramebufferSizeCallback = [](GLFWwindow*, int, int)->void {};
std::function<void(GLFWwindow*, int, const char**)> Window::s_DropCallBack = [](GLFWwindow*, int, const char**)->void {};

Window::Window(int width, int height, const char* title, bool fullscreen)
    :
    m_Title(title), m_FullScreen(fullscreen)
{
    if (m_FullScreen)
    {
        auto monitor = glfwGetPrimaryMonitor();
        auto mode = glfwGetVideoMode(monitor);
        m_Width = mode->width;
        m_Height = mode->height;
        m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), glfwGetPrimaryMonitor(), NULL);
    }
    else
    {
        m_Width = width;
        m_Height = height;
        m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), NULL, NULL);
    }

    if (m_Window == NULL)
    {
        throw EXCEPTION("Failed to create window");
        glfwTerminate();
    }
    
    glfwMakeContextCurrent(m_Window);
    glfwSetFramebufferSizeCallback(m_Window, Window::DefaultFramebufferSizeCallBack);
    glfwSetDropCallback(m_Window, Window::DefaultDropCallBack);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw EXCEPTION("Failed to init glad.");
    }

    glViewport(0, 0, m_Width, m_Height);
    glEnable(GL_DEPTH_TEST);
}

Window::~Window()
{
    glfwDestroyWindow(m_Window);
}

GLFWwindow* Window::Get() const
{
	return m_Window;
}

bool Window::Open() const
{
    return !glfwWindowShouldClose(m_Window);
}

float Window::GetAspectRation() const
{
    return static_cast<float>(m_Width) / static_cast<float>(m_Height);
}

void Window::SwapBuffers()
{
    glfwSwapBuffers(m_Window);
}

void Window::Clear(float r, float g, float b)
{
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::Close()
{
    glfwSetWindowShouldClose(m_Window, true);
}

void Window::SetFullScreen(bool flag)
{
    if (m_FullScreen == flag) return;

    m_FullScreen = flag;
    glfwDestroyWindow(m_Window);

    if (m_FullScreen)
    {
        auto monitor = glfwGetPrimaryMonitor();
        auto mode = glfwGetVideoMode(monitor);
        m_Width = mode->width;
        m_Height = mode->height;
        m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), glfwGetPrimaryMonitor(), NULL);
    }
    else
    {
        m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), NULL, NULL);
    }

    if (m_Window == NULL)
    {
        throw EXCEPTION("Failed to create window");
        glfwTerminate();
    }

    glfwMakeContextCurrent(m_Window);
    glfwSetFramebufferSizeCallback(m_Window, Window::DefaultFramebufferSizeCallBack);
    glfwSetDropCallback(m_Window, Window::DefaultDropCallBack);
    glViewport(0, 0, m_Width, m_Height);
}

std::optional<std::string> Window::TryPopDropedFile()
{
    if (s_DropedFilesPaths.empty())
        return std::nullopt;

    std::string filePath = std::move(s_DropedFilesPaths.front());
    s_DropedFilesPaths.pop();

    return std::make_optional(std::move(filePath));
}

void Window::SetFramebufferSizeCallBack(std::function<void(GLFWwindow*, int, int)>&& callback)
{
    s_FramebufferSizeCallback = std::move(callback);
}

void Window::SetDropCallBack(std::function<void(GLFWwindow*, int, const char**)>&& callback)
{
    s_DropCallBack = std::move(callback);
}

void Window::DefaultFramebufferSizeCallBack(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    s_FramebufferSizeCallback(window, width, height);
}

void Window::DefaultDropCallBack(GLFWwindow* window, int count, const char** paths)
{
    for (int i = 0; i < count; i++)
    {
        Window::s_DropedFilesPaths.emplace(paths[i]);
    }
    s_DropCallBack(window, count, paths);
}
