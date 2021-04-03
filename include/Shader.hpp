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
    static const int c_LogSize = 512;
    unsigned int m_ID;

    std::string readFile(const std::string& path) const;
    unsigned int compileShader(const std::string& shaderSource, unsigned int shaderType) const;
    unsigned int linkProgram(unsigned int vertexShader, unsigned int fragmentShader) const;
    void checkShaderCompileErrors(unsigned int shader) const;
    void checkProgramLinkingErrors(unsigned int program) const;
    std::string getShaderType(unsigned int shaderType) const;
};

#endif