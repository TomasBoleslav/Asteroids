#include "Texture2D.hpp"

#include "Errors.hpp"

#include <glad/glad.h>

Texture2D::Texture2D() noexcept
    : m_textureID(0), m_width(0), m_height(0), m_settings(Settings())
{
}

Texture2D::Texture2D(Texture2D&& other) noexcept
    : m_textureID(other.m_textureID), m_width(other.m_width), m_height(other.m_height), m_settings(other.m_settings)
{
    other.m_textureID = 0;
    other.m_width = 0;
    other.m_height = 0;
}

Texture2D& Texture2D::operator=(Texture2D&& other) noexcept
{
    m_textureID = other.m_textureID;
    m_width = other.m_width;
    m_height = other.m_height;
    m_settings = other.m_settings;

    other.m_textureID = 0;
    other.m_width = 0;
    other.m_height = 0;

    return *this;
}

Texture2D::~Texture2D()
{
    destroy();
}

void Texture2D::create(unsigned int width, unsigned int height, unsigned char* data)
{
    destroy();
    m_textureID = generateTexture(width, height, data, m_settings);
    m_width = width;
    m_height = height;
}

void Texture2D::create(unsigned int width, unsigned int height, unsigned char* data, const Settings& settings)
{
    destroy();
    m_textureID = generateTexture(width, height, data, settings);
    m_width = width;
    m_height = height;
    m_settings = settings;
}

void Texture2D::bind() const
{
    GL_CALL(glActiveTexture(GL_TEXTURE0));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, m_textureID));
}

void Texture2D::unbind() const
{
    GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
}

unsigned int Texture2D::generateTexture(unsigned int width, unsigned int height, unsigned char* data, const Settings& settings) const
{
    unsigned int textureID;
    GL_CALL(glGenTextures(1, &textureID));
    GL_CALL(glActiveTexture(GL_TEXTURE0));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, textureID));

    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, settings.wrapS));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, settings.wrapT));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, settings.filterMin));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, settings.filterMag));
    GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, settings.internalFormat, width, height, 0, settings.format, GL_UNSIGNED_BYTE, data));
    GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));

    GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
    return textureID;
}

void Texture2D::destroy() const
{
    if (m_textureID != 0)
    {
        GL_CALL(glDeleteTextures(1, &m_textureID));
    }
}

Texture2D::Settings::Settings()
    : internalFormat(GL_RGB), format(GL_RGB), wrapS(GL_REPEAT), wrapT(GL_REPEAT), filterMin(GL_LINEAR), filterMag(GL_LINEAR)
{
}
