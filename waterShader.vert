#version 330 core

layout (location = 0) in vec3 position;

out vec4 clipSpace;
out vec2 textureCoords;
out vec3 cameraCoords;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform mat4 modelview;
uniform vec3 cameraPos;

void main(){
clipSpace = projection*view*model*vec4(position.x,0.0,position.y,1.0);

textureCoords = vec2(position.x/2 + 0.5, position.y/2 + 0.5);
}