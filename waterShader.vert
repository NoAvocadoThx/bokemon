#version 330 core

layout (location = 0) in vec3 position;

out vec4 clipSpace;
out vec2 textureCoords;
out vec3 toCam;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform mat4 modelview;
uniform vec4 clippingPlane;
uniform vec3 camPos;
uniform mat4 reflection;
const float tiling =12.0;

void main(){
clipSpace = (projection)*((view))*model*vec4(position.x,0,position.y,1.0);
gl_Position = clipSpace;
	vec4 worldPos = modelview * vec4(position, 1.0f);
	gl_ClipDistance[0] = dot(worldPos, clippingPlane);
textureCoords = vec2(position.x/2 + 0.5, position.y/2 + 0.5)*tiling;
toCam = camPos-clipSpace.xyz;
}