#version 330 core

layout (location = 0) in vec3 position;
out vec3 TexCoords;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP * vec4(position.x, position.y, position.z, 1.0);
    TexCoords = position;
}