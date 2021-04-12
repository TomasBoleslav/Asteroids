#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

#include <vector>

namespace geom
{
    static const glm::vec2 zeroVector = glm::vec2(0.0f);
    bool pointInPolygon(const std::vector<glm::vec2>& polygon, glm::vec2 point);

    bool onSegment(glm::vec2 p, glm::vec2 q, glm::vec2 r);
    int orientation(glm::vec2 p, glm::vec2 q, glm::vec2 r);
    bool doIntersect(glm::vec2 p1, glm::vec2 q1, glm::vec2 p2, glm::vec2 q2);

    bool doPolygonsIntersect(const std::vector<glm::vec2>& polygon1, const std::vector<glm::vec2>& polygon2);

    glm::mat4 getModelMatrix(glm::vec2 position, glm::vec2 size, float rotation);
    glm::vec2 getCirclePoint(glm::vec2 center, float radius, float angle);
}

#endif