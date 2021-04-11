#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <glm/vec2.hpp>

#include <vector>

namespace geom
{
    static const glm::vec2 zeroVector = glm::vec2(0.0f);
    bool pointInPolygon(const std::vector<glm::vec2>& polygon, glm::vec2 point);
    bool pointInPolygon2(const std::vector<glm::vec2>& polygon, glm::vec2 point);
    bool polygonsIntersect(const std::vector<glm::vec2>& polygon1, const std::vector<glm::vec2>& polygon2);
    bool segmentsIntersect(glm::vec2 a1, glm::vec2 b1, glm::vec2 a2, glm::vec2 b2);
    float cross(glm::vec2 v1, glm::vec2 v2);


    bool onSegment(glm::vec2 p, glm::vec2 q, glm::vec2 r);
    int orientation(glm::vec2 p, glm::vec2 q, glm::vec2 r);
    bool doIntersect(glm::vec2 p1, glm::vec2 q1, glm::vec2 p2, glm::vec2 q2);

    bool doPolygonsIntersect(const std::vector<glm::vec2>& polygon1, const std::vector<glm::vec2>& polygon2);
}

#endif