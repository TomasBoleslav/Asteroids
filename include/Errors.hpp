
#ifndef ERRORS_HPP
#define ERRORS_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <cassert>
#include <iostream>

namespace err
{
	void ClearAllErrors()
	{
		while (glGetError() != GL_NO_ERROR);
	}

	bool LogCall(const char* function, const char* file, int line)
	{
		bool success = false;
		unsigned int errorCode;
		while ((errorCode = glGetError()) != GL_NO_ERROR)
		{
			success = true;
			std::string error;
			switch (errorCode)
			{
			case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
			case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
			case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
			case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
			case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
			case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
			case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
			}
			std::cerr << "[OpenGL Error] " << errorCode << " " << error << function <<
				" File: " << file << ", Line: " << line << std::endl;
		}
		return success;
	}
}

#define GLCall(func) err::ClearAllErrors()\
	func;\
	assert(err::LogCall(#func, __FILE__, __LINE__))

#endif
