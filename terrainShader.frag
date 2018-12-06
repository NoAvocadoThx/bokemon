#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec2 TexCoords;
in vec3 fragNormal;
in vec3 fragposition;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 color;
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    vec2 textcd=TexCoords*20.0f;
    float height =(fragposition.y+100.0f)/120.0f;
	float h1=160.0f/20.0f;
	float d = 160.0f/30.0f;
	vec4 blend = texture(texture1, textcd);
	
	float backtexture= 1-(blend.r+blend.g+blend.b);
	if(height<=0.83){
	color =texture(texture1,textcd)*0.9;

	}
	else if (height>=0.83&&height<=0.84){
	color =(texture(texture1,textcd)*0.3+texture(texture2,textcd)*0.6);
	}
	else{
	color=texture(texture2,textcd)*0.9;
	}
    // Color everything a hot pink color. An alpha of 1.0f means it is not transparent.
	//vec2 textcd=TexCoords*20.0f;
   
	
}