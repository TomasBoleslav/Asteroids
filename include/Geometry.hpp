#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

/*
Source for checking line intersection:
https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
*/

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

#include <vector>

namespace geom
{
    static const glm::vec2 zeroVector = glm::vec2(0.0f);

    // Given three colinear points p, q, r, the function checks if
    // point q lies on line segment 'pr'
    bool pointOnSegment(glm::vec2 p, glm::vec2 q, glm::vec2 r);

    // Find orientation of ordered triplet (p, q, r).
    // The function returns following values
    // 0 --> p, q and r are colinear
    // 1 --> Clockwise
    // 2 --> Counterclockwise
    int orientation(glm::vec2 p, glm::vec2 q, glm::vec2 r);

    // Check if line segment 'p1q1' and 'p2q2' intersect.
    bool segmentsIntersect(glm::vec2 p1, glm::vec2 q1, glm::vec2 p2, glm::vec2 q2);

    // Check if polygons intersect by checking line segments intersection (does not work for polygon inside polygon).
    bool polygonsIntersect(const std::vector<glm::vec2>& polygon1, const std::vector<glm::vec2>& polygon2);

    // Get model matrix for normalized shape (coordinates between 0.0 and 1.0) with the target size.
    glm::mat4 getModelMatrix(glm::vec2 position, glm::vec2 size, float rotation);

    // Transform polygon by given matrix.
    std::vector<glm::vec2> transformPolygon(const std::vector<glm::vec2>& polygon, glm::mat4 matrix);

    // Get a direction for given angle as a normalized vector.
    glm::vec2 getDirection(float angleDeg);

    // Clamp angle between [0.0, 360.0] while preserving the direction.
    float clampAngle(float angleDeg);
}

#endif