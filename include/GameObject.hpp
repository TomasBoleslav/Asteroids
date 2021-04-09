#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

#include "Texture2D.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <vector>
#include <memory>

class GameObject
{
public:
    glm::vec2 position;
    glm::vec2 size;
    glm::vec3 color;
    float rotation;
    std::shared_ptr<Texture2D> texture;
    
    GameObject();

    void draw(const Renderer& renderer, const std::shared_ptr<Shader>& shader);

};

#endif