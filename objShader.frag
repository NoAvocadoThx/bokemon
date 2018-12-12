#version 330 core

// This is a sample fragment shader.

in vec3 TexCoords;
out vec4 color;

uniform vec4 c;

void main()
{
    color = c;
}