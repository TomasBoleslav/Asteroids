#include "Errors.hpp"

bool err::LogCall(const char* function, const char* file, int line)
{
	bool success = true;
	unsigned int errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		success = false;
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
		if (error.empty())
		{
			error = std::to_string(errorCode);
		}
		std::cerr << "[OpenGL Error] (" << error << ") " << function << ", Location: "
			<< file << " (" << line << ")" << std::endl;
	}
	return success;
}