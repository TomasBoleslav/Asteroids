#include "Shader.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
    unsigned int vertexShader = CompileShader(ReadFile(vertexPath), GL_VERTEX_SHADER);
    unsigned int fragmentShader = CompileShader(ReadFile(fragmentPath), GL_FRAGMENT_SHADER);
    m_ID = LinkProgram(vertexShader, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
    glDeleteProgram(m_ID);
}

void Shader::Use() const
{
    glUseProgram(m_ID);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
}

std::string Shader::ReadFile(const std::string& path) const
{
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        file.open(path);
        std::stringstream sstream;
        sstream << file.rdbuf();
        file.close();
        return sstream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cerr << "Could not read file " << path << std::endl;
        return ""; // TODO: throw
    }
}

unsigned int Shader::CompileShader(const std::string& shaderSource, unsigned int shaderType) const
{
    const char* c_str = shaderSource.c_str();
    unsigned int shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &c_str, nullptr);
    glCompileShader(shader);
    CheckShaderCompileErrors(shader, shaderType);
    return shader;
}

void Shader::CheckShaderCompileErrors(unsigned int shader, unsigned int shaderType) const
{
    int success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        int logLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

        std::string infoLog(logLength, '\0');
        glGetShaderInfoLog(shader, logLength, nullptr, &infoLog[0]);
        std::cerr
            << "Failed to compile " << GetShaderTypeName(shaderType) << " shader:" << std::endl
            << infoLog << std::endl;
        // TODO: throw
    }
}

std::string Shader::GetShaderTypeName(unsigned int shaderType) const
{
    switch (shaderType)
    {
    case GL_VERTEX_SHADER: return "vertex"; break;
    case GL_FRAGMENT_SHADER: return "fragment"; break;
    }
    return "";
}

unsigned int Shader::LinkProgram(unsigned int vertexShader, unsigned int fragmentShader) const
{
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    CheckProgramLinkingErrors(shaderProgram);
    return shaderProgram;
}

void Shader::CheckProgramLinkingErrors(unsigned int program) const
{
    int success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        int logLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

        std::string infoLog(logLength, '\0');
        glGetProgramInfoLog(program, logLength, nullptr, &infoLog[0]);
        std::cerr
            << "Failed to link program:" << std::endl
            << infoLog << std::endl;
        // TODO: throw
    }
}

int Shader::GetUniformLocation(const std::string& name) const
{
    auto it = m_UniformLocationsCache.find(name);
    if (it != m_UniformLocationsCache.end())
    {
        return it->second;
    }
    int location = glGetUniformLocation(m_ID, name.c_str());
    m_UniformLocationsCache[name] = location;
    return location;
}