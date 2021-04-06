#ifndef ERRORS_HPP
#define ERRORS_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <cassert>
#include <iostream>

namespace err
{
	bool LogCall(const char* function, const char* file, int line);
}

#define GL_CALL(func) func;\
	if (!err::LogCall(#func, __FILE__, __LINE__))\
		throw std::runtime_error("OpenGL function call failed.")

#endif
