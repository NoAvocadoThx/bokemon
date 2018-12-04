#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec2 TexCoords;
in vec3 fragNormal;
in vec3 fragposition;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 color;
uniform sampler2D myTextureSampler;

void main()
{
    // Color everything a hot pink color. An alpha of 1.0f means it is not transparent.
	vec2 textcd=TexCoords*20.0f;
    color = texture(myTextureSampler, textcd);
	//color = vec4(0.5f,0.1f,0.3f,1.0f);
	
}