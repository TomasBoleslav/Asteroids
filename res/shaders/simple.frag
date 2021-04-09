#version 330 core

in vec2 texCoords;

out vec4 fragColor;

uniform sampler2D u_texture;
uniform vec3 u_color;

void main()
{    
    fragColor = vec4(u_color, 1.0) * texture(u_texture, texCoords);
}