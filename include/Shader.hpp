#ifndef SHADER_HPP
#define SHADER_HPP

#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <unordered_map>

class Shader final
{
public:
    Shader() = delete;
    Shader(const Shader&) = delete;
    Shader(Shader&&) = delete;
    Shader& operator=(const Shader&) = delete;
    Shader& operator=(Shader&&) = delete;

    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();
    
    void use() const;
    void unuse() const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;
    void setInt(const std::string& name, int value) const;

private:
    unsigned int m_programID;
    mutable std::unordered_map<std::string, int> m_uniformLocationsCache;

    std::string readFile(const std::string& path) const;
    unsigned int compileShader(const std::string& shaderSource, unsigned int shaderType, const std::string& shaderTypeName) const;
    void checkShaderCompileErrors(unsigned int shader, unsigned int shaderType, const std::string& shaderTypeName) const;
    unsigned int linkProgram(unsigned int vertexShader, unsigned int fragmentShader) const;
    void checkProgramLinkingErrors(unsigned int program) const;

    int getUniformLocation(const std::string& name) const;
};

#endif