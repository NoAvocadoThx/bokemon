
#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 color;

uniform vec3 cameraPosition;

void main()
{
	float intensity;

	//vec3 lightDirection = FragPos - vec3(100.0, 100.0, 100.0);
	//vec3 lightDirection = FragPos;
	vec3 lightDirection = vec3(25.0,25.0,25.0);
	intensity = dot(normalize(lightDirection), normalize(Normal));
	//intensity = dot(lightDirection, normalize(Normal));

	if (intensity > 0.95)
		color = vec4(1.0,0.5,0.5,1.0);
	else if (intensity > 0.5)
		color = vec4(0.6,0.3,0.3,1.0);
	else if (intensity > 0.25)
		color = vec4(0.4,0.2,0.2,1.0);
	else if (intensity > 0.01)
		color = vec4(0.2,0.1,0.1,1.0);
		
	float edge = dot(normalize(cameraPosition), normalize(Normal));
	if(edge < 0.01)
	{
	    color = vec4(0.0, 0.0, 0.0, 1.0);
	}
	
}

/*
struct Material
{
	vec3 color_diff;
	vec3 color_spec;
	vec3 color_ambi;
	float spec_shine;
};

struct Light
{
	vec3 light_color;
	vec3 light_dir;
};

uniform Material material;
uniform Light light;
uniform vec3 camPos;

in vec3 surfaceNormal;
in vec3 baseNormal;
in vec3 vertex;

out vec4 color;

void main()
{
	vec3 c_diff, c_spec, c_ambi;
	vec3 c_l = light.light_color;
	vec3 N = normalize(baseNormal);
	vec3 L = -normalize(light.light_dir);
	vec3 E = normalize(camPos - vertex);

	// Silhouette edges
	float edge = max(0.0f, dot(N, E));
	if (edge < 0.3f)
	{
		color = vec4(0, 0, 0, 1);
	}
	else
	{
		const float A = 0.1;
		const float B = 0.3;
		const float C = 0.6;
		const float D = 1.0;

		float diff = max(0.0f, dot(N, L));
		if (diff < A) diff = 0.0;
		else if (diff < B) diff = B;
		else if (diff < C) diff = C;
		else diff = D;

		vec3 R = normalize( 2 * dot(L, N) * N - L );
		float spec = pow( max(0.0f, dot( R, E )), material.spec_shine );
		spec = step(0.5f, spec);
		
		vec3 matColor = c_l * (material.color_diff * diff + material.color_spec * spec + material.color_ambi);
		color = vec4(matColor.x, matColor.y, matColor.z, 1.0);
	}
	color = vec4(0,0,1,1);
}
*/