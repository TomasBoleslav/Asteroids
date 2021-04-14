#include "ResourceManager.hpp"

#include "Debug.hpp"

#include <stb_image.h>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <stdexcept>

std::unordered_map<std::string, Shader> ResourceManager::s_shaders;
std::unordered_map<std::string, Texture2D> ResourceManager::s_textures;

void ResourceManager::loadShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath)
{
    if (s_shaders.find(name) != s_shaders.end())
    {
        throw std::logic_error("Shader with name '" + name + "' already exists.");
    }
    Shader shader;
    std::string vertexSource = readFile(vertexPath);
    std::string fragmentSource = readFile(fragmentPath);
    shader.generate(vertexSource, fragmentSource);
    s_shaders[name] = shader;
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
        throw std::ios_base::failure("Failed to load texture at location '" + path + "'.");
    }
    Texture2D::Settings settings;
    if (alpha)
    {
        settings.internalFormat = GL_RGBA;
        settings.format = GL_RGBA;
    }
    Texture2D texture;
    texture.generate(width, height, data, settings);
    s_textures[name] = texture;
    stbi_image_free(data);
}

void ResourceManager::addTexture(const std::string& name, const Texture2D& texture)
{
    if (s_textures.find(name) != s_textures.end())
    {
        throw std::logic_error("Texture with name '" + name + "' already exists.");
    }
    if (!texture.isValid())
    {
        throw std::logic_error("Given texture '" + name + "' is not valid.");
    }
    s_textures[name] = texture;
}

const Shader& ResourceManager::getShader(const std::string& name)
{
    auto it = s_shaders.find(name);
    if (it == s_shaders.end())
    {
        throw std::logic_error("No shader with name '" + name + "'.");
    }
    return it->second;
}

const Texture2D& ResourceManager::getTexture(const std::string& name)
{
    auto it = s_textures.find(name);
    if (it == s_textures.end())
    {
        throw std::logic_error("No texture with name '" + name + "'.");
    }
    return it->second;
}

bool ResourceManager::hasTexture(const std::string& name)
{
    return s_textures.find(name) != s_textures.end();
}

void ResourceManager::clear()
{
    for (auto&& pair : s_shaders)
    {
        GL_CALL(glDeleteProgram(pair.second.getID()));
    }
    for (auto&& pair : s_textures)
    {
        unsigned int textureID = pair.second.getID();
        GL_CALL(glDeleteTextures(1, &textureID));
    }
    s_shaders.clear();
    s_textures.clear();
}

std::string ResourceManager::readFile(const std::string& path)
{
    std::ifstream file(path);
    if (!file)
    {
        throw std::ios_base::failure("Failed to read file '" + path + "'");
    }
    std::stringstream sstream;
    sstream << file.rdbuf();
    file.close();
    return sstream.str();
}