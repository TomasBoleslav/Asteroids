#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

class Shader
{
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    void use() const;
    void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3) const;
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