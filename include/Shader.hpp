#ifndef SHADER_HPP
#define SHADER_HPP

#include <glm/gtc/matrix_transform.hpp>

#include <string>

class Shader
{
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();
    
    inline unsigned int getID() { return m_ID; }
    void use() const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;

private:
    unsigned int m_ID;

    std::string readFile(const std::string& path) const;
    unsigned int compileShader(const std::string& shaderSource, unsigned int shaderType) const;
    void checkShaderCompileErrors(unsigned int shader, unsigned int shaderType) const;
    std::string getShaderTypeName(unsigned int shaderType) const;
    unsigned int linkProgram(unsigned int vertexShader, unsigned int fragmentShader) const;
    void checkProgramLinkingErrors(unsigned int program) const;
};

#endif