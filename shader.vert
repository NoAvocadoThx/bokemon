#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

// This is an example vertex shader. GLSL is very similar to C.
// You can define extra functions if needed, and the main() function is
// called when the vertex shader gets run.
// The vertex shader gets called once per vertex.


layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 aTexCoord;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 modelview;

uniform mat4 projection;


void main()
{
    gl_Position = projection * modelview * vec4(position, 1.0f);
    FragPos = vec3(modelview * vec4(position, 1.0f));
    Normal = mat3(transpose(inverse(modelview))) * normal;  
	TexCoords =vec2( position.x,position.y);
	
    
} 