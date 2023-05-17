#pragma once
#include <exception>
#include <string>

#define EXCEPTION(msg) Exception(__LINE__, __FILE__, msg)

class Exception : public std::exception
{
public:
	Exception(int line, const char* file, const std::string& msg);

	char const* what() const override;
private:
	int m_Line;
	std::string m_File;
	std::string m_Msg;
	mutable std::string m_WhatBuffer;
};

