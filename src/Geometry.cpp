#include "Geometry.hpp"

#include <cstdlib>
#include <glm/trigonometric.hpp>
#include <glm/geometric.hpp> 
#include <algorithm>

bool geom::pointInPolygon(const std::vector<glm::vec2>& polygon, glm::vec2 point)
{
    bool inside = false;
    float x = point.x;  // ray from (x,y) to right (x as horizontal axis)
    float y = point.y;
    const float epsilon = 0.01f;
    glm::vec2 prev = polygon.back();
    for (auto&& curr : polygon)
    {
        glm::vec2 upper, lower;
        if (prev.y < curr.y)
        {
            upper = curr;
            lower = prev;
        }
        else
        {
            upper = prev;
            lower = curr;
        }
        if (lower.y <= y && y < upper.y)
        {
            if (y - lower.y < epsilon)
            {
                lower.y -= epsilon;
            }
            if (upper.y - y < epsilon)
            {
                upper.y += epsilon;
            }
            // nepouzivam upper a lower
            if ((x - upper.x) * (upper.y - lower.y) < (upper.x - lower.x) * (upper.y - y))
            {
                inside = !inside;   // ray intersects with line segment
            }
        }
        /*
        if ((curr.y <= y && y < prev.y) ||
            (prev.y <= y && y < curr.y))                                    // prev and curr on different sides of half-plane
        {
            if ((x - curr.x) * (prev.y - curr.y) < (prev.x - curr.x) * (y - curr.y))
            {
            inside = !inside;   // ray intersects with line segment
            }
        }*/
        prev = curr;
    }
    return inside;
}

bool geom::pointInPolygon2(const std::vector<glm::vec2>& polygon, glm::vec2 point)
{
    glm::atan(glm::vec2());
    bool inside = false;

    return inside;
}

float geom::cross(glm::vec2 v1, glm::vec2 v2)
{
    return v1.x * v2.y - v1.y * v2.x;
}


bool geom::polygonsIntersect(const std::vector<glm::vec2>& polygon1, const std::vector<glm::vec2>& polygon2)
{
    return false;
}

bool geom::segmentsIntersect(glm::vec2 a1, glm::vec2 b1, glm::vec2 a2, glm::vec2 b2)
{
    // nevyhoda - musim jeste zkontrolovat, jestli neni 1 uvnitr druheho
    return false;
}

bool geom::onSegment(glm::vec2 p, glm::vec2 q, glm::vec2 r)
{
    if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
        q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y))
        return true;

    return false;
}

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int geom::orientation(glm::vec2 p, glm::vec2 q, glm::vec2 r)
{
    // See https://www.geeksforgeeks.org/orientation-3-ordered-points/
    // for details of below formula.
    int val = (q.y - p.y) * (r.x - q.x) -
        (q.x - p.x) * (r.y - q.y);

    if (val == 0) return 0;  // colinear

    return (val > 0) ? 1 : 2; // clock or counterclock wise
}

// The main function that returns true if line segment 'p1q1'
// and 'p2q2' intersect.
bool geom::doIntersect(glm::vec2 p1, glm::vec2 q1, glm::vec2 p2, glm::vec2 q2)
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
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;

    // p1, q1 and q2 are colinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;

    // p2, q2 and p1 are colinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;

    // p2, q2 and q1 are colinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;

    return false; // Doesn't fall in any of the above cases
}

bool geom::doPolygonsIntersect(const std::vector<glm::vec2>& polygon1, const std::vector<glm::vec2>& polygon2)
{
    glm::vec2 previous1 = polygon1.back();
    for (auto&& current1 : polygon1)
    {
        glm::vec2 previous2 = polygon2.back();
        for (auto&& current2 : polygon2)
        {
            if (doIntersect(previous1, current1, previous2, current2))
            {
                return true;
            }
            previous2 = current2;
        }
        previous1 = current1;
    }
    return false;
}