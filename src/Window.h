#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <optional>
#include <functional>
#include <string>
#include <memory>
#include <queue>

class Window
{
public:
	Window(int width, int height, const char* title, bool fullscreen = false);
	~Window();

	GLFWwindow* Get() const;
	bool Open() const;
	float GetAspectRation() const;
	void SwapBuffers();
	void Clear(float r, float g, float b);
	void Close();
	void SetFullScreen(bool flag);
	std::optional<std::string> TryPopDropedFile();

public:
	static void SetFramebufferSizeCallBack(std::function<void(GLFWwindow*, int, int)>&& callback);
	static void SetDropCallBack(std::function<void(GLFWwindow*, int, const char**)>&& callback);
	
private:
	static void DefaultFramebufferSizeCallBack(GLFWwindow* window, int width, int height);
	static void DefaultDropCallBack(GLFWwindow* window, int count, const char** paths);

private:
	int m_Width;
	int m_Height;
	bool m_FullScreen;
	std::string m_Title;
	GLFWwindow* m_Window;

	static std::queue<std::string> s_DropedFilesPaths;
	static std::function<void(GLFWwindow*, int, int)> s_FramebufferSizeCallback;
	static std::function<void(GLFWwindow*, int, const char**)> s_DropCallBack;
};

