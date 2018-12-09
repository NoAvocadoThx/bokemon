//Toon Shading Fragment Shader
#version 330
layout(location = 0) out vec4 out_color;
 
uniform vec3 light_position;
uniform vec3 eye_position;
uniform int material_shininess;
uniform float material_kd;
uniform float material_ks;
 
in vec3 world_pos;
in vec3 world_normal;
 
const vec3 ambinetColor = vec3(0.90,0.0,0.20);
 
//number of levels
//for diffuse color
const int levels = 5;
const float scaleFactor = 1.0 / levels;
const vec3 diffuseColor = vec3(0.30,0.80,0.10);
 
void main()
{ 
 vec3 L = normalize( light_position - world_pos);
 vec3 V = normalize( eye_position - world_pos);
 
 float diffuse = max(0, dot(L,world_normal));
 diffuseColor = diffuseColor * material_kd * floor(diffuse * levels) * scaleFactor;
 
 vec3 H = normalize(L + V);
 
 float specular = 0.0;
 
 if( dot(L,world_normal) > 0.0)
 {
   specular = material_ks * pow( max(0, dot( H, world_normal)), material_shininess);
 }
 
 //limit specular
 float specMask = (pow(dot(H, world_normal), material_shininess) > 0.4) ? 1 : 0;
 
 float edgeDetection = (dot(V, world_normal) > 0.2) ? 1 : 0;
 
 color = edgeDetection * (color + diffuseColor + specular * specMask);
 
 out_color = vec4(color,1);
}