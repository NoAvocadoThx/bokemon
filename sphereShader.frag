#version 330 core

out vec4 color;
uniform vec3 pickColor;
void main()
{
    		color = vec4(pickColor, 1.0f);
}
