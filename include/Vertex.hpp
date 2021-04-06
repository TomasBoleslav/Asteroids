#ifndef VERTEX_HPP
#define VERTEX_HPP

struct Position
{
    float x;
    float y;
    float z;

    Position(float x, float y, float z) : x(x), y(y), z(z) {}
};

struct Color
{
    float r;
    float g;
    float b;

    Color(float r, float g, float b) : r(r), g(g), b(b) {}
};

struct TexCoord
{
    float s;
    float t;

    TexCoord(float s, float t) : s(s), t(t) {}
};

struct Vertex
{
    Position pos;
    Color color;
    TexCoord tex;

    Vertex(Position pos, Color color, TexCoord tex) : pos(pos), color(color), tex(tex) {}
};

#endif