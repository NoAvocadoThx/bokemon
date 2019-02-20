#version 330 core

in vec3 Normal;
in vec3 FragPos;

uniform mat4 projection;
uniform mat4 modelview;

out vec4 color;

void main()
{

   
        vec3 norm = normalize(Normal);
        color = vec4(norm.x, norm.y, norm.z, 1.0f);
    
    
}

