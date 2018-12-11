#version 330 core

in vec2 textureCoods;
in vec4 clipSpace;

out vec4 color;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform float flow;
uniform sampler2D dudv;

void main(){

vec4 reflection = texture(reflectionTexture, textureCoods);
vec4 refraction = texture(refractionTexture, textureCoods);
color = mix(reflection,refraction,0.5f);
}