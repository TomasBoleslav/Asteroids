#ifndef ERRORS_HPP
#define ERRORS_HPP

#include <string>
#include <stdexcept>

/**
* Contains functions for debugging program using OpenGL.
*/
namespace debug
{
	// Check for OpenGL errors caused after calling the given function and print them to std::err.
	bool LogCall(const char* function, const char* file, int line);
}

// Call an OpenGL function and check for errors.
#define GL_CALL(func) func;\
	if (!debug::LogCall(#func, __FILE__, __LINE__))\
		throw std::logic_error("OpenGL function call failed.")

#endif
