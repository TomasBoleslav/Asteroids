#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include "Shader.hpp"
#include "Texture2D.hpp"

#include <string>
#include <unordered_map>
#include <memory>

class ResourceManager final
{
public:
    // TODO: create with 2 file paths: file with shader names, file with texture names
    // TODO: static class or not?
    ResourceManager();
    ResourceManager(const std::string& shaderListPath, const std::string& textureListPath);
    // TODO: destructor - clear resources or not?

    void loadShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);
    void loadTexture(const std::string& name, const std::string& path);
    void addTexture(const std::string& name, std::shared_ptr<Texture2D> texture);
    
    std::shared_ptr<Shader> getShader(const std::string& name) const;
    std::shared_ptr<Texture2D> getTexture(const std::string& name) const;

    void clear();

private:
    std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaders;
    std::unordered_map<std::string, std::shared_ptr<Texture2D>> m_textures;

    // TODO: std::ifstream::failure (std::io_base::failure) exception may be thrown
    std::string readFile(const std::string& path) const;
};

#endif