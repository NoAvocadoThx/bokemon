#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

// This is an example vertex shader. GLSL is very similar to C.
// You can define extra functions if needed, and the main() function is
// called when the vertex shader gets run.
// The vertex shader gets called once per vertex.


layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 TexCoord;

uniform mat4 projection;
uniform mat4 modelview;
uniform vec4 clippingPlane;


out vec2 TexCoords;
out vec3 fragNormal;
out vec3 fragposition;

void main()
{
    gl_Position = projection * modelview * vec4(position, 1.0);
	gl_ClipDistance[0]=0;
    TexCoords = TexCoord;
    fragNormal = normal;
    fragposition = position;
	vec4 worldPos = mat4(1.0f)*vec4(position,1.0f);
	gl_ClipDistance[0]=dot(worldPos,clippingPlane);
}