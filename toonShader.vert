#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 projection;
uniform mat4 modelview;
uniform mat4 model;

out vec3 world_pos;
out vec3 world_normal;

void main()
{
	world_pos = mat3(model) * position;//careful here
	world_normal = normalize(mat3(model) * normal);
	gl_Position = projection*modelview*vec4(position,1);
}