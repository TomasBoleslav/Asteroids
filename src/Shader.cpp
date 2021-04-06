#include "Shader.hpp"

#include "Errors.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
    unsigned int vertexShader = compileShader(readFile(vertexPath), GL_VERTEX_SHADER, "vertex");
    unsigned int fragmentShader = compileShader(readFile(fragmentPath), GL_FRAGMENT_SHADER, "fragment");
    m_programID = linkProgram(vertexShader, fragmentShader);
    GL_CALL(glDeleteShader(vertexShader));
    GL_CALL(glDeleteShader(fragmentShader));
}

Shader::~Shader()
{
    GL_CALL(glDeleteProgram(m_programID));
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
    int location = getUniformLocation(name);
    GL_CALL(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat)));
}

void Shader::setInt(const std::string& name, int value) const
{
    int location = getUniformLocation(name);
    GL_CALL(glUniform1i(getUniformLocation(name), value));
}

// TODO: std::ifstream::failure (std::io_base::failure) exception may be thrown
std::string Shader::readFile(const std::string& path) const
{
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    file.open(path);
    std::stringstream sstream;
    sstream << file.rdbuf();
    file.close();
    return sstream.str();
}

unsigned int Shader::compileShader(const std::string& shaderSource, unsigned int shaderType, const std::string& shaderTypeName) const
{
    const char* c_str = shaderSource.c_str();
    GL_CALL(unsigned int shader = glCreateShader(shaderType));
    GL_CALL(glShaderSource(shader, 1, &c_str, nullptr));
    GL_CALL(glCompileShader(shader));
    checkShaderCompileErrors(shader, shaderType, shaderTypeName);
    return shader;
}

void Shader::checkShaderCompileErrors(unsigned int shader, unsigned int shaderType, const std::string& shaderTypeName) const
{
    int success = 0;
    GL_CALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
    if (!success)
    {
        int logLength = 0;
        GL_CALL(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength));

        std::string infoLog(logLength, '\0');
        GL_CALL(glGetShaderInfoLog(shader, logLength, nullptr, &infoLog[0]));
        throw std::runtime_error("Failed to compile " + shaderTypeName + " shader:\n" + infoLog);
        // TODO: throw
    }
}

unsigned int Shader::linkProgram(unsigned int vertexShader, unsigned int fragmentShader) const
{
    GL_CALL(unsigned int shaderProgram = glCreateProgram());
    GL_CALL(glAttachShader(shaderProgram, vertexShader));
    GL_CALL(glAttachShader(shaderProgram, fragmentShader));
    GL_CALL(glLinkProgram(shaderProgram));
    checkProgramLinkingErrors(shaderProgram);
    return shaderProgram;
}

void Shader::checkProgramLinkingErrors(unsigned int program) const
{
    int success = 0;
    GL_CALL(glGetProgramiv(program, GL_LINK_STATUS, &success));
    if (!success)
    {
        int logLength = 0;
        GL_CALL(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength));

        std::string infoLog(logLength, '\0');
        GL_CALL(glGetProgramInfoLog(program, logLength, nullptr, &infoLog[0]));
        throw std::runtime_error("Failed to link program:\n" + infoLog);
        // TODO: throw
    }
}

int Shader::getUniformLocation(const std::string& name) const
{
    auto it = m_uniformLocationsCache.find(name);
    if (it != m_uniformLocationsCache.end())
    {
        return it->second;
    }
    GL_CALL(int location = glGetUniformLocation(m_programID, name.c_str()));
    m_uniformLocationsCache[name] = location;
    return location;
}