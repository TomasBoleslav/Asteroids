#ifndef SHADER_HPP
#define SHADER_HPP

#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <unordered_map>

class Shader
{
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();
    
    void Use() const;
    void SetMat4(const std::string& name, const glm::mat4& mat);

private:
    unsigned int m_ID;
    mutable std::unordered_map<std::string, int> m_UniformLocationsCache;

    std::string ReadFile(const std::string& path) const;
    unsigned int CompileShader(const std::string& shaderSource, unsigned int shaderType) const;
    void CheckShaderCompileErrors(unsigned int shader, unsigned int shaderType) const;
    std::string GetShaderTypeName(unsigned int shaderType) const;
    unsigned int LinkProgram(unsigned int vertexShader, unsigned int fragmentShader) const;
    void CheckProgramLinkingErrors(unsigned int program) const;

    int GetUniformLocation(const std::string& name) const;
};

#endif