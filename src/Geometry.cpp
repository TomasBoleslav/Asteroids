#include "Geometry.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>

bool geom::pointOnSegment(glm::vec2 p, glm::vec2 q, glm::vec2 r)
{
    if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
        q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y))
        return true;

    return false;
}

int geom::orientation(glm::vec2 p, glm::vec2 q, glm::vec2 r)
{
    // See https://www.geeksforgeeks.org/orientation-3-ordered-points/
    // for details of below formula.
    int val = static_cast<int>((q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y));

    if (val == 0) return 0;  // colinear

    return (val > 0) ? 1 : 2; // clock or counterclock wise
}

bool geom::segmentsIntersect(glm::vec2 p1, glm::vec2 q1, glm::vec2 p2, glm::vec2 q2)
{
    // Find the four orientations needed for general and
    // special cases
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    // General case
    if (o1 != o2 && o3 != o4)
        return true;

    // Special Cases
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1
    if (o1 == 0 && pointOnSegment(p1, p2, q1)) return true;

    // p1, q1 and q2 are colinear and q2 lies on segment p1q1
    if (o2 == 0 && pointOnSegment(p1, q2, q1)) return true;

    // p2, q2 and p1 are colinear and p1 lies on segment p2q2
    if (o3 == 0 && pointOnSegment(p2, p1, q2)) return true;

    // p2, q2 and q1 are colinear and q1 lies on segment p2q2
    if (o4 == 0 && pointOnSegment(p2, q1, q2)) return true;

    return false; // Doesn't fall in any of the above cases
}

bool geom::polygonsIntersect(const std::vector<glm::vec2>& polygon1, const std::vector<glm::vec2>& polygon2)
{
    glm::vec2 previous1 = polygon1.back();
    for (auto&& current1 : polygon1)
    {
        glm::vec2 previous2 = polygon2.back();
        for (auto&& current2 : polygon2)
        {
            if (segmentsIntersect(previous1, current1, previous2, current2))
            {
                return true;
            }
            previous2 = current2;
        }
        previous1 = current1;
    }
    return false;
}

glm::mat4 geom::getModelMatrix(glm::vec2 position, glm::vec2 size, float rotation)
{
    glm::mat4 model = glm::mat4(1.0f);
    // Translate
    model = glm::translate(model, glm::vec3(position, 0.0f));
    // Rotate around center
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
    // Scale
    model = glm::scale(model, glm::vec3(size, 1.0f));
    return model;
}

std::vector<glm::vec2> geom::transformPolygon(const std::vector<glm::vec2>& polygon, glm::mat4 matrix)
{
    std::vector<glm::vec2> newPolygon;
    newPolygon.reserve(polygon.size());
    for (const auto& point : polygon)
    {
        glm::vec2 newPoint = matrix * glm::vec4(point, 0.0, 1.0);
        newPolygon.push_back(newPoint);
    }
    return newPolygon;
}

glm::vec2 geom::getDirection(float angleDeg)
{
    return glm::vec2(
        glm::cos(glm::radians(angleDeg)),
        glm::sin(glm::radians(angleDeg))
    );
}

float geom::clampAngle(float angleDeg)
{
    while (angleDeg < 0.0f)
    {
        angleDeg += 360.0f;
    }
    while (angleDeg > 360.0f)
    {
        angleDeg -= 360.0f;
    }
    return angleDeg;
}