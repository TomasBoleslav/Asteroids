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
    static void loadShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);
    static void loadTexture(const std::string& name, const std::string& path, bool alpha);
    static void addTexture(const std::string& name, std::shared_ptr<Texture2D> texture);
    
    static std::shared_ptr<Shader> getShader(const std::string& name);
    static std::shared_ptr<Texture2D> getTexture(const std::string& name);

    static void clear();

private:
    ResourceManager();

    static std::unordered_map<std::string, std::shared_ptr<Shader>> s_shaders;
    static std::unordered_map<std::string, std::shared_ptr<Texture2D>> s_textures;

    // TODO: std::ifstream::failure (std::io_base::failure) exception may be thrown
    static std::string readFile(const std::string& path);
};

#endif