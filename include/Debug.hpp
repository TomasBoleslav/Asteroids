#ifndef ERRORS_HPP
#define ERRORS_HPP

#include <string>
#include <stdexcept>

namespace debug
{
	bool LogCall(const char* function, const char* file, int line);
}

#define GL_CALL(func) func;\
	if (!debug::LogCall(#func, __FILE__, __LINE__))\
		throw std::logic_error("OpenGL function call failed.")

#endif
