#ifndef SHADER_HPP
#define SHADER_HPP

#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <unordered_map>

class Shader final
{
public:
    Shader() noexcept;
    Shader(const Shader&) = delete;
    Shader(Shader&& other) noexcept;
    Shader& operator=(const Shader&) = delete;
    Shader& operator=(Shader&& other) noexcept;
    ~Shader();

    void create(const std::string& vertexSource, const std::string& fragmentSource);
    
    void use() const;
    void unuse() const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;
    void setInt(const std::string& name, int value) const;

private:
    unsigned int m_programID;
    mutable std::unordered_map<std::string, int> m_uniformLocationsCache;

    unsigned int compileShader(const std::string& source, unsigned int type, const std::string& typeName) const;
    unsigned int linkProgram(unsigned int vertexID, unsigned int fragmentID) const;
    void checkShaderCompileErrors(unsigned int shaderID, unsigned int type, const std::string& typeName) const;
    void checkProgramLinkingErrors(unsigned int programID) const;
    
    int getUniformLocation(const std::string& name) const;
    
    void destroy() const;
};

#endif