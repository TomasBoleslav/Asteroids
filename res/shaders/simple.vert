#version 330 core

layout (location = 0) in vec2 in_vertex;
layout (location = 1) in vec2 in_texCoords;

out vec2 texCoords;

uniform mat4 u_model;
uniform mat4 u_projection;

void main()
{
    gl_Position = u_projection * u_model * vec4(in_vertex.xy, 0.0, 1.0);
    texCoords = in_texCoords;
}