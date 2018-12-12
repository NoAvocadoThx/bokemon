#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection; 
uniform mat4 modelview; 
uniform vec4 clippingPlane;

void main()
{    
    
    TexCoords = aPos;
    gl_Position = projection * modelview * vec4(aPos, 1.0);
	vec4 worldPos = mat4(1.0f)*vec4(aPos,1.0f);
	//gl_ClipDistance[0]=dot(worldPos,clippingPlane);
}