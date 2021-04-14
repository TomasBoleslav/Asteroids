#ifndef TEXTURE2D_HPP
#define TEXTURE2D_HPP

#include <glm/vec2.hpp>

/**
* Represents a texture.
* Acts only as an observer (does not contain any image data).
* Generated textures should be given to ResourceManager which will free them correctly.
*/
class Texture2D final
{
public:
    // Settings for texture generation.
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
    Texture2D();

    // Generate a new texture with the given data and default settings.
    void generate(unsigned int width, unsigned int height, unsigned char* data);

    // Generate a new texture with the given data and settings.
    void generate(unsigned int width, unsigned int height, unsigned char* data, const Settings& settings);

    // Binds the texture for the next draw call.
    void bind() const;

    void unbind() const;

    unsigned int getID() const;

    // Check if ID of this texture is valid (a texture with this ID was created).
    // This will not work if the texture was freed manually or by ResourceManager.
    bool isValid() const;

private:
    unsigned int m_textureID;

    unsigned int createTexture(unsigned int width, unsigned int height, unsigned char* data, const Settings& settings) const;
};

#endif