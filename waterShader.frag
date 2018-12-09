#version 330 core

in vec2 textureCoods;
in vec4 clipSpace;

out vec4 color;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;

void main(){

vec4 reflection = texture(reflectionTexture, textureCoods);
vec4 refraction = texture(refractionTexture, textureCoods);
color = vec4(0.5,0.5,0.3,1.0);
}