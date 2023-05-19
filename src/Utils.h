#pragma once
#include "imgui/imgui.h"
#include "loguru/loguru.hpp"

#include <string>
#include <chrono>

#define AS_PREFIX(label)	utils::LabelPrefix(label).c_str()
#define AS_PREFIX_C(label)	utils::LabelPrefix(label)
#define FORMAT(fmt, ...)	utils::FormatText(fmt, __VA_ARGS__)
#define TIME_STAMP()		utils::GetCurrentTime().c_str()

#define ENABLE_FILE_LOGGING 1

#if ENABLE_FILE_LOGGING
	#define InfoLog(...)	LOG_F(INFO, __VA_ARGS__)
	#define Warning(...)	LOG_F(WARNING, __VA_ARGS__)
	#define Error(...)		LOG_F(ERROR, __VA_ARGS__)						
#else
	#define InfoLog(...)
	#define Warning(...)
	#define Error(...)		
#endif

enum TextColor_
{
	TextColor_White = 0,
	TextColor_Red = 1,
	TextColor_Green = 2,
	TextColor_Blue = 3,
	TextColor_Yellow = 4,
};

const std::vector<ImVec4> Colors = {
	{1.0f, 1.0f, 1.0f, 1.0f}, // white
	{1.0f, 0.0f, 0.0f, 1.0f}, // red
	{0.0f, 1.0f, 0.0f, 1.0f}, // green
	{0.0f, 0.0f, 1.0f, 1.0f}, // blue
	{1.0f, 1.0f, 0.0f, 1.0f}, // yellow
};

namespace utils
{
	// Init loguru
	void LogsInit();

	// Adding label text to widget as a prefix
	std::string LabelPrefix(const char* label);

	// C-style text formating
	std::string FormatText(const char* fmt, ...);

	// Returns current time in format HH:MM:SS
	std::string GetCurrentTime();

	// Imgui styles from @janekb04 
	// https://github.com/ocornut/imgui/issues/707#issuecomment-917151020
	void EmbraceTheDarkness();

	// Imgui styles from @Maksasj 
	// https://github.com/ocornut/imgui/issues/707#issuecomment-1494706165
	void ImGuiTheme1();
};