#include "ResourceManager.hpp"

#include "Shader.hpp"
#include "Texture2D.hpp"

#include <stb_image.h>

#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>

ResourceManager::ResourceManager()
{
}

ResourceManager::ResourceManager(const std::string& shaderListPath, const std::string& textureListPath)
{
}

void ResourceManager::loadShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath)
{
    if (m_shaders.find(name) != m_shaders.end())
    {
        throw std::runtime_error("Shader with name '" + name + "' already exists.");
        // TODO: throw
    }
    std::string vertexSource = readFile(vertexPath);
    std::string fragmentSource = readFile(fragmentPath);
    m_shaders[name] = std::make_shared<Shader>(vertexSource, fragmentSource);
}

void ResourceManager::loadTexture(const std::string& name, const std::string& path)
{
    if (m_textures.find(name) != m_textures.end())
    {
        throw std::runtime_error("Texture with name '" + name + "' already exists.");
        // TODO: throw
    }
    int width, height, channelsCount;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channelsCount, 0);
    if (!data)
    {
        throw std::ios_base::failure("Failed to load texture at location '" + path +"'.");
        // TODO: throw
    }
    m_textures[name] = std::make_shared<Texture2D>(width, height, data);
    stbi_image_free(data);
}

std::shared_ptr<Shader> ResourceManager::getShader(const std::string& name) const
{
    auto it = m_shaders.find(name);
    if (it == m_shaders.end())
    {
        throw std::logic_error("Resource manager does not have a shader with the name '" + name + "'.");
        // TODO: throw correct type
    }
    return it->second;
}

std::shared_ptr<Texture2D> ResourceManager::getTexture(const std::string& name) const
{
    auto it = m_textures.find(name);
    if (it == m_textures.end())
    {
        throw std::logic_error("Resource manager does not have a texture with the name '" + name + "'.");
        // TODO: throw correct type
    }
    return it->second;
}

std::string ResourceManager::readFile(const std::string& path) const
{
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    file.open(path);
    std::stringstream sstream;
    sstream << file.rdbuf();
    file.close();
    return sstream.str();
}