#include "ResourceManager.hpp"

#include "Debug.hpp"

#include <stb_image.h>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <stdexcept>

std::unordered_map<std::string, std::shared_ptr<Shader>> ResourceManager::s_shaders;
std::unordered_map<std::string, std::shared_ptr<Texture2D>> ResourceManager::s_textures;

void ResourceManager::loadShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath)
{
    if (s_shaders.find(name) != s_shaders.end())
    {
        throw std::logic_error("Shader with name '" + name + "' already exists.");
    }
    std::string vertexSource = readFile(vertexPath);
    std::string fragmentSource = readFile(fragmentPath);
    s_shaders[name] = std::make_shared<Shader>(vertexSource, fragmentSource);
}

void ResourceManager::loadTexture(const std::string& name, const std::string& path, bool alpha)
{
    if (s_textures.find(name) != s_textures.end())
    {
        throw std::logic_error("Texture with name '" + name + "' already exists.");
    }
    int width, height, channelsCount;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channelsCount, 0);
    if (!data)
    {
        throw std::ios_base::failure("Failed to load texture at location '" + path +"'.");
    }
    Texture2D::Settings settings;
    if (alpha)
    {
        settings.internalFormat = GL_RGBA;
        settings.format = GL_RGBA;
    }
    s_textures[name] = std::make_shared<Texture2D>(width, height, data, settings);
    stbi_image_free(data);
}

void ResourceManager::addTexture(const std::string& name, std::shared_ptr<Texture2D> texture)
{
    if (s_textures.find(name) != s_textures.end())
    {
        throw std::logic_error("Texture with name '" + name + "' already exists.");
    }
    s_textures[name] = texture;
}

std::shared_ptr<Shader> ResourceManager::getShader(const std::string& name)
{
    auto it = s_shaders.find(name);
    if (it == s_shaders.end())
    {
        return nullptr;
    }
    return it->second;
}

std::shared_ptr<Texture2D> ResourceManager::getTexture(const std::string& name)
{
    auto it = s_textures.find(name);
    if (it == s_textures.end())
    {
        return nullptr;
    }
    return it->second;
}

std::string ResourceManager::readFile(const std::string& path)
{
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    file.open(path);
    std::stringstream sstream;
    sstream << file.rdbuf();
    file.close();
    return sstream.str();
}

void ResourceManager::clear()
{
    for (auto&& pair : s_shaders)
    {
        GL_CALL(glDeleteProgram(pair.second->getID()));
    }
    for (auto&& pair : s_textures)
    {
        unsigned int textureID = pair.second->getID();
        GL_CALL(glDeleteTextures(1, &textureID));
    }
    s_shaders.clear();
    s_textures.clear();
}