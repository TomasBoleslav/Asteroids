#ifndef TEXTURE2D_HPP
#define TEXTURE2D_HPP

#include <glm/vec2.hpp>

class Texture2D final
{
public:
    struct Settings final
    {
        unsigned int internalFormat;
        unsigned int format;
        unsigned int wrapS;
        unsigned int wrapT;
        unsigned int filterMin;
        unsigned int filterMag;

        Settings();
    };
    Texture2D() = delete;
    Texture2D(const Texture2D&) = delete;
    Texture2D& operator=(const Texture2D&) = delete;

    Texture2D(unsigned int width, unsigned int height, unsigned char* data);
    Texture2D(unsigned int width, unsigned int height, unsigned char* data, const Settings& settings);

    void bind() const;
    void unbind() const;

    unsigned int getID() const;
    const Settings& getSettings() const;
    glm::vec2 getSize();

private:
    unsigned int m_textureID;
    unsigned int m_width, m_height;
    Settings m_settings;

    unsigned int generateTexture(unsigned int width, unsigned int height, unsigned char* data, const Settings& settings) const;
};

#endif