#version 330
 in vec3 Normal;
in vec3 FragPos;

 
out vec4 outputF;
 
void main()
{
    outputF = vec4(0.4f, 0.6f, 0, 1.0f);
}