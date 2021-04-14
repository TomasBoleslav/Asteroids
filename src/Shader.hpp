#ifndef SHADER_HPP
#define SHADER_HPP

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <string>
#include <unordered_map>

/**
* Represents a GLSL shader program for drawing with a graphics card.
* Acts as an observer, ResourceManager will free generated shaders correctly.
*/
class Shader final
{
public:
    Shader();

    // Generate a shader from vertex shader and fragment shader source code.
    // Throws std::logic_error if the shader compilation failed.
    void generate(const std::string& vertexSource, const std::string& fragmentSource);

    // Use program for the next draw call.
    void use() const;
    void unuse() const;

    // Set matrix4x4 uniform.
    void setMat4(const std::string& name, const glm::mat4& mat) const;

    // Set vector3 uniform.
    void setVec3(const std::string& name, const glm::vec3& vec) const;

    unsigned int getID() const;

private:
    unsigned int m_programID;

    unsigned int compileShader(const std::string& source, unsigned int type, const std::string& typeName) const;
    unsigned int linkProgram(unsigned int vertexID, unsigned int fragmentID) const;
    void checkShaderCompileErrors(unsigned int shaderID, unsigned int type, const std::string& typeName) const;
    void checkProgramLinkingErrors(unsigned int programID) const;
};

#endif