#include "Shader.hpp"

#include "Debug.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <stdexcept>

Shader::Shader() : m_programID(0)
{
}

void Shader::generate(const std::string& vertexSource, const std::string& fragmentSource)
{
    unsigned int vertexID = compileShader(vertexSource, GL_VERTEX_SHADER, "vertex");
    unsigned int fragmentID = compileShader(fragmentSource, GL_FRAGMENT_SHADER, "fragment");
    m_programID = linkProgram(vertexID, fragmentID);
    GL_CALL(glDeleteShader(vertexID));
    GL_CALL(glDeleteShader(fragmentID));
}

void Shader::use() const
{
    GL_CALL(glUseProgram(m_programID));
}

void Shader::unuse() const
{
    GL_CALL(glUseProgram(0));
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
    GL_CALL(int location = glGetUniformLocation(m_programID, name.c_str()));
    GL_CALL(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat)));
}

void Shader::setVec3(const std::string& name, const glm::vec3& vec) const
{
    GL_CALL(int location = glGetUniformLocation(m_programID, name.c_str()));
    GL_CALL(glUniform3f(location, vec.x, vec.y, vec.z));
}

unsigned int Shader::getID() const
{
    return m_programID;
}

unsigned int Shader::compileShader(const std::string& source, unsigned int type, const std::string& typeName) const
{
    const char* c_str = source.c_str();
    GL_CALL(unsigned int shaderID = glCreateShader(type));
    GL_CALL(glShaderSource(shaderID, 1, &c_str, nullptr));
    GL_CALL(glCompileShader(shaderID));
    checkShaderCompileErrors(shaderID, type, typeName);
    return shaderID;
}

unsigned int Shader::linkProgram(unsigned int vertexID, unsigned int fragmentID) const
{
    GL_CALL(unsigned int programID = glCreateProgram());
    GL_CALL(glAttachShader(programID, vertexID));
    GL_CALL(glAttachShader(programID, fragmentID));
    GL_CALL(glLinkProgram(programID));
    checkProgramLinkingErrors(programID);
    return programID;
}

void Shader::checkShaderCompileErrors(unsigned int shaderID, unsigned int type, const std::string& typeName) const
{
    int success = 0;
    GL_CALL(glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success));
    if (!success)
    {
        int logLength = 0;
        GL_CALL(glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength));

        std::string infoLog(logLength, '\0');
        GL_CALL(glGetShaderInfoLog(shaderID, logLength, nullptr, &infoLog[0]));
        throw std::logic_error("Failed to compile " + typeName + " shader:\n" + infoLog);
    }
}

void Shader::checkProgramLinkingErrors(unsigned int programID) const
{
    int success = 0;
    GL_CALL(glGetProgramiv(programID, GL_LINK_STATUS, &success));
    if (!success)
    {
        int logLength = 0;
        GL_CALL(glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength));

        std::string infoLog(logLength, '\0');
        GL_CALL(glGetProgramInfoLog(programID, logLength, nullptr, &infoLog[0]));
        throw std::logic_error("Failed to link program:\n" + infoLog);
    }
}