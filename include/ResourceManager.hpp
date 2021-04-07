#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include "Shader.hpp"
#include "Texture2D.hpp"

#include <string>
#include <unordered_map>

class ResourceManager final
{
public:
    // TODO: create with 2 file paths: file with shader names, file with texture names
    ResourceManager();
    ResourceManager(const std::string& shaderListPath, const std::string& textureListPath);

    void loadShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);
    void loadTexture(const std::string& name, const std::string& path);
    
    const Shader& getShader(const std::string& name) const;
    const Texture2D& getTexture(const std::string& name) const;

    void clear();
private:
    std::unordered_map<std::string, Shader> m_shaders;
    std::unordered_map<std::string, Texture2D> m_textures;

    // TODO: std::ifstream::failure (std::io_base::failure) exception may be thrown
    std::string readFile(const std::string& path) const;
};

#endif