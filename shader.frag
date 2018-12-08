#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 color;
uniform sampler2D myTextureSampler;

void main()
{
    // Color everything a hot pink color. An alpha of 1.0f means it is not transparent.
    color = texture(myTextureSampler, TexCoords);

}