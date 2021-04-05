#ifndef ERRORS_HPP
#define ERRORS_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <cassert>
#include <iostream>

namespace err
{
	void ClearAllErrors();
	bool LogCall(const char* function, const char* file, int line);
}

#define GL_CALL(func) err::ClearAllErrors();\
	func;\
	assert(err::LogCall(#func, __FILE__, __LINE__))

#endif
