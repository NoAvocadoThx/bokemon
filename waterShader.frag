#version 330 core

in vec2 textureCoords;
in vec4 clipSpace;
in vec3 toCam;

out vec4 color;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform float flow;
uniform sampler2D dudv;

void main(){
    vec2 ndc = (clipSpace.xy / clipSpace.w) / 2.0f + 0.5f;
	vec2 refractCoords = vec2(ndc.x, ndc.y);
	vec2 reflectCoords = vec2(ndc.x, -ndc.y);

	vec2 distortion1 = (texture(dudv, vec2(textureCoords.x + flow, textureCoords.y)).rg * 2.0 - 1.0) * 0.01;
	vec2 distortion2 = (texture(dudv, vec2(-textureCoords.x + flow, textureCoords.y + flow)).rg * 2.0 - 1.0) * 0.01;
	vec2 totalDistort = distortion1 + distortion2;

	refractCoords += totalDistort;
	refractCoords = clamp(refractCoords, 0.001, 0.999);
	reflectCoords += totalDistort;
	reflectCoords.x = clamp(reflectCoords.x, 0.001, 0.999);
	reflectCoords.y = clamp(reflectCoords.y, -0.999, - 0.001);

	vec3 viewVec = normalize(toCam);
	float refractiveFactor = dot(viewVec, vec3(0.0f, 1.0f, 0.0f));
	
refractiveFactor = pow(refractiveFactor, 0.8f);
//vec4 reflection = texture(reflectionTexture, textureCoords);
//vec4 refraction = texture(refractionTexture, textureCoords);
vec4 colorReflect = texture(reflectionTexture, reflectCoords);
	vec4 colorRefract = texture(refractionTexture, refractCoords);
color = mix(colorReflect, colorRefract, 0.5);
	color = mix(color, vec4(0.0f, 0.3f, 0.5f, 1.0f), 0.2);
//color = mix(reflection,refraction,0.5f);
}