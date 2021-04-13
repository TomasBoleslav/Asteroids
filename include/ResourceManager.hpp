#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include "Shader.hpp"
#include "Texture2D.hpp"

#include <string>
#include <unordered_map>
#include <memory>

/**
* Loads, stores and frees resources.
* Resources can be retrieved by name (std::string).
*/
class ResourceManager final
{
public:
    // Load a shader program from files.
    // Throws std::ifstream::failure if data could not be read.
    // Throws std::logic_error if shader with given name exists.
    static void loadShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);

    // Load a texture from file.
    // Throws std::ifstream::failure if data could not be read.
    // Throws std::logic_error if texture with given name exists.
    static void loadTexture(const std::string& name, const std::string& path, bool alpha);

    // Add texture to resources.
    // Throws std::logic_error if texture with given name exists.
    static void addTexture(const std::string& name, std::shared_ptr<Texture2D> texture);
    
    // Retrieve a shader by name.
    // Throws std::logic_error if shader with given name does not exist.
    static std::shared_ptr<Shader> getShader(const std::string& name);

    // Retrieve a texture by name.
    // Throws std::logic_error if texture with given name does not exist.
    static std::shared_ptr<Texture2D> getTexture(const std::string& name);

    // Free all resources. Retrieved resources will no longer be valid.
    static void clear();

private:
    ResourceManager() {}

    static std::unordered_map<std::string, std::shared_ptr<Shader>> s_shaders;
    static std::unordered_map<std::string, std::shared_ptr<Texture2D>> s_textures;

    // Read the content of a file. Throws std::ifstream::failure if the file could not be read.
    static std::string readFile(const std::string& path);
};

#endif